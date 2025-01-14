#pragma once

#include <type_traits>

namespace Reflection
{
	struct InvokeInfo
	{
		void* returnData	= nullptr;
		void* args			= nullptr;
		void* object		= nullptr;
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
		
		/*
		*	Static Invoke
		*/

		template<typename TSignature, typename TReturn, typename... TArgs, size_t... Indices>
		static void StaticInvokeInternal(const InvokeInfo* info, size_t* handle, std::index_sequence<Indices...>)
		{
#define INVOKE (*(TSignature*)&handle)( \
			PassArgs<TypeByIndex_t<Indices, TArgs...>>(info->args, GetOffset<Indices, 0, TArgs...>())...) \

			if constexpr (std::is_void_v<TReturn>)
			{
				INVOKE;
			}
			else
			{
				*(TReturn*)info->returnData = INVOKE;
			}

#undef INVOKE
		}

		template<typename TSignature>
		struct StaticProxyInvoke;

		template<typename TReturn, typename... TArgs>
		struct StaticProxyInvoke<TReturn(*)(TArgs...)>
		{
			static void Invoke(const InvokeInfo* info, size_t* handle)
			{
				using Signature = TReturn(*)(TArgs...);
				StaticInvokeInternal<Signature, TReturn, TArgs...>(info, handle, std::index_sequence_for<TArgs...>{});
			}
		};

		template<typename TSignature, typename T, typename TReturn, typename... TArgs, size_t... Indices>
		static void InstanceInvokeInternal(const InvokeInfo* info, size_t* handle, std::index_sequence<Indices...>)
		{
#define INVOKE ((T*)info->object->*(*(TSignature*)&handle))( \
			PassArgs<TypeByIndex_t<Indices, TArgs...>>(info->args, GetOffset<Indices, 0, TArgs...>())...) \

			if constexpr (std::is_void_v<TReturn>)
			{
				INVOKE;
			}
			else
			{
				*(TReturn*)info->returnData = INVOKE;
			}

#undef INVOKE
		}

		/*
		*	Instance Invoke
		*/

		template<typename T, typename TSignature>
		struct InstanceProxyInvoke;

		template<typename T, typename TReturn, typename... TArgs>
		struct InstanceProxyInvoke<T, TReturn(T::*)(TArgs...)>
		{
			static void Invoke(const InvokeInfo* info, size_t* handle)
			{
				using Signature = TReturn(T::*)(TArgs...);
				InstanceInvokeInternal<Signature, T, TReturn, TArgs...>(info, handle, std::index_sequence_for<TArgs...>{});			
			}
		};

		template<typename T, typename TReturn, typename... TArgs>
		struct InstanceProxyInvoke<T, TReturn(T::*)(TArgs...) const>
		{
			static void Invoke(const InvokeInfo* info, size_t* handle)
			{
				using Signature = TReturn(T::*)(TArgs...) const;
				InstanceInvokeInternal<Signature, T, TReturn, TArgs...>(info, handle, std::index_sequence_for<TArgs...>{});
			}
		};
	}

	namespace Internal
	{
		class ProxyInvoker
		{
		public:

			using Handle = size_t*;
			using ProxyInvokeSignature = void(*)(const InvokeInfo*, Handle);

		private:

			Handle m_Handle;
			ProxyInvokeSignature m_Invoke;

		public:

			void Invoke(const InvokeInfo* info) const
			{
				m_Invoke(info, m_Handle);
			}

			template<typename TReturn, typename... TArgs>
			ProxyInvoker(TReturn(*handle)(TArgs...)) :
				m_Handle(*(size_t**)&handle),
				m_Invoke(&Generation::StaticProxyInvoke<decltype(handle)>::Invoke)
			{
			}

			template<typename T, typename TReturn, typename... TArgs>
			ProxyInvoker(TReturn(T::*handle)(TArgs...)) :
				m_Handle(*(size_t**)&handle),
				m_Invoke(&Generation::InstanceProxyInvoke<T, decltype(handle)>::Invoke)
			{
			}

			template<typename T, typename TReturn, typename... TArgs>
			ProxyInvoker(TReturn(T::*handle)(TArgs...) const) :
				m_Handle(*(size_t**)&handle),
				m_Invoke(&Generation::InstanceProxyInvoke<T, decltype(handle)>::Invoke)
			{
			}
		};
	}
}
