#pragma once

#include "Reflection/ParameterType.h"

namespace Reflection
{
	class ArgumentsPacker
	{
	private:

		int8_t* m_Pack;

	private:

		template<size_t Offset, typename TFirst, typename... TOther>
		void Pack(TFirst f, TOther... args)
		{
			*(TFirst*)(m_Pack + Offset) = std::move(f);
			
			if constexpr (sizeof...(TOther) > 0)
			{
				return Pack<Offset + sizeof(TFirst), TOther...>(std::forward<TOther>(args)...);
			}
		}

	public:

		template<typename... TArgs>
		ArgumentsPacker(int8_t* pack, TArgs... args) :
			m_Pack(pack)
		{
			Pack<0, TArgs...>(std::forward<TArgs>(args)...);
		}

		template<typename T, typename... TArgs>
		static constexpr size_t GetSize()
		{
			size_t size = sizeof(T);

			if constexpr (sizeof...(TArgs) > 0)
			{
				size += GetSize<TArgs...>();
			}

			return size;
		}
	};

	struct InvokeInfo
	{
		void* returnData = nullptr;
		const void* args = nullptr;
		void* object = nullptr; // TODO: maybe remove and make that object was a args[0]
	};

	template<typename TSignature>
	struct FunctionCoreInfo;

	template<typename T, typename TReturn, typename... TArgs>
	struct FunctionCoreInfo<TReturn(T::*)(TArgs...)>
	{
		constexpr static size_t ArgsCount = sizeof...(TArgs);
		using ReturnType = TReturn;
		using Args = std::tuple<TArgs...>;
	};

	template<typename TReturn, typename T, typename... TArgs>
	struct FunctionCoreInfo<TReturn(T::*)(TArgs...) const>
	{
		constexpr static size_t ArgsCount = sizeof...(TArgs);
		using ReturnType = TReturn;
		using Args = std::tuple<TArgs...>;
	};

	template<typename TReturn, typename... TArgs>
	struct FunctionCoreInfo<TReturn(*)(TArgs...)>
	{
		constexpr static size_t ArgsCount = sizeof...(TArgs);
		using ReturnType = TReturn;
		using Args = std::tuple<TArgs...>;
	};

	template<typename TSignature>
	struct ParamsExcluder;

	template<typename T, typename TReturn, typename... TArgs>
	struct ParamsExcluder<TReturn(T::*)(TArgs...)>
	{
		static void Exclude(const ParameterType* params)
		{
			if constexpr (sizeof...(TArgs) > 0)
			{
				ParameterType::Excluder(params).Process<0, TArgs...>();
			}
		}
	};

	template<typename TReturn, typename T, typename... TArgs>
	struct ParamsExcluder<TReturn(T::*)(TArgs...) const>
	{
		static void Exclude(const ParameterType* params)
		{
			if constexpr (sizeof...(TArgs) > 0)
			{
				ParameterType::Excluder(params).Process<0, TArgs...>();
			}
		}
	};

	template<typename TReturn, typename... TArgs>
	struct ParamsExcluder<TReturn(*)(TArgs...)>
	{
		static void Exclude(const ParameterType* params)
		{
			if constexpr (sizeof...(TArgs) > 0)
			{
				ParameterType::Excluder(params).Process<0, TArgs...>();
			}
		}
	};

	template<typename... TArgs>
	class ArgumentsPack
	{
	private:

		int8_t m_Pack[ArgumentsPacker::GetSize<TArgs...>()];

	public:

		ArgumentsPack(TArgs... args)
		{
			ArgumentsPacker(m_Pack, std::forward<TArgs>(args)...);
		}

		const void* Get() const
		{
			return m_Pack;
		}
	};

	template<>
	class ArgumentsPack<>
	{
	public:

		ArgumentsPack() = default;

		const void* Get() const
		{
			return nullptr;
		}
	};

	namespace Generation
	{
		template<bool Test, typename TTrue, typename TFalse>
		struct IfState;

		template<typename TTrue, typename TFalse>
		struct IfState<true, TTrue, TFalse>
		{
			using Type = TTrue;
		};

		template<typename TTrue, typename TFalse>
		struct IfState<false, TTrue, TFalse>
		{
			using Type = TFalse;
		};

		template<bool Test, typename TTrue, typename TFalse>
		using IfState_t = IfState<Test, TTrue, TFalse>::Type;

		template<typename T>
		using RefToPtr_t = IfState_t<std::is_reference_v<T>, std::add_pointer_t<std::remove_reference_t<T>>, T>;

		template<size_t Index, typename... TArgs>
		struct TypeByIndex;

		template<size_t Index, typename TFirst, typename... TOther>
		struct TypeByIndex<Index, TFirst, TOther...>
		{
			using Type = TypeByIndex<(Index - 1), TOther...>::Type;
		};

		template<typename TFirst, typename... TOther>
		struct TypeByIndex<0, TFirst, TOther...>
		{
			using Type = TFirst;
		};

		template<size_t Index, typename... TOther>
		using TypeByIndex_t = TypeByIndex<Index, TOther...>::Type;

		template<typename T, typename TPtr = RefToPtr_t<T>>
		static T PassArgs(const void* bytes, size_t offset)
		{
			return (T)*(TPtr*)((int8_t*)bytes + offset);
		}

		template<>
		static void PassArgs<void, void>(const void* bytes, size_t offset)
		{
		}

		template<size_t Till, size_t I, typename T, typename... TArgs>
		constexpr size_t GetOffset()
		{
			if constexpr (Till == I)
			{
				return 0;
			}

			size_t offset = sizeof(T);

			if constexpr (sizeof...(TArgs) > 0)
			{
				offset += GetOffset<Till, I + 1, TArgs...>();
			}

			return offset;
		}

		template<typename T>
		struct ProxyInvoker;
	}

	namespace Internal
	{
		using ProxyInvokeSignature = void(*)(const InvokeInfo*);

		class VoidInvoker
		{
		private:

			ProxyInvokeSignature m_Invoke;
			const std::vector<ParameterType> m_Params;

		public:

			std::span<const ParameterType> GetParameterTypes() const
			{
				return m_Params;
			}

			template<size_t First, typename... TArgs>
			bool CheckParameterTypes() const
			{
				if constexpr (sizeof...(TArgs) == 0)
				{
					return m_Params.size() == First;
				}
				else
				{
					return m_Params.size() - First == sizeof...(TArgs) && ParameterType::Comparator(m_Params.data() + First).Process<0, TArgs...>();
				}
			}

			void Invoke(const InvokeInfo* info) const
			{
				m_Invoke(info);
			}

			template<typename TSignature>
			VoidInvoker(ProxyInvokeSignature invoker, TSignature signature) :
				m_Invoke(invoker),
				m_Params(FunctionCoreInfo<TSignature>::ArgsCount)
			{
				ParamsExcluder<TSignature>::Exclude(m_Params.data());
			}

			VoidInvoker()
			{
			}
		};

		class Invoker : public VoidInvoker
		{
		private:

			const ParameterType m_ReturnType;

		public:

			ParameterType GetReturnType() const
			{
				return m_ReturnType;
			}

			template<typename T>
			bool CheckReturnType() const
			{
				return m_ReturnType == ParameterType(ParameterType::Initializer<T>{});
			}

			template<typename TSignature>
			Invoker(ProxyInvokeSignature invoker, TSignature signature) :
				VoidInvoker(invoker, signature),
				m_ReturnType(ParameterType::Initializer<FunctionCoreInfo<TSignature>::template ReturnType>{})
			{
			}
		};
	}
}
