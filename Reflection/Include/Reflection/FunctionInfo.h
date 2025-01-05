#pragma once

#include "Reflection/MemberInfo.h"
#include "Reflection/ParameterType.h"

namespace Reflection
{
	class REFLECTION_API FunctionInfo : public MemberInfo
	{
	public:

		using Handle = size_t*;

	private:

		const Handle						m_Address;
		const ParameterType					m_ReturnType;
		const std::vector<ParameterType>	m_Parameters;

	public:

		const ParameterType& GetReturnType() const
		{
			return m_ReturnType;
		}

		size_t GetParameterCount() const
		{
			return m_Parameters.size();
		}

		std::span<const ParameterType> GetParameters() const
		{
			return m_Parameters;
		}

		template<typename TReturn, typename ...TArgs>
		bool CheckSiganture() const
		{
			if (CheckReturnType<TReturn>() == false)
				return false;

			if constexpr (sizeof...(TArgs) == 0)
			{
				return m_Parameters.empty();
			}
			else
			{
				return m_Parameters.size() == sizeof...(TArgs) && CheckParameterTypes<TArgs...>();
			}
		}

		template<typename TReturn, typename ...TArgs>
		TReturn Invoke(TArgs... args) const
		{
			assert((CheckSiganture<TReturn, TArgs...>()) && "Function signature mismatch");

			using Signature = TReturn(*)(TArgs...);
			return GetAddress<Signature>()(std::forward<TArgs>(args)...);
		}

	protected:

		template<typename TReturn, typename ...TArgs>
		struct FunctionMetaInfo
		{
			using ReturnType = TReturn;

			constexpr static auto ArgsCount = sizeof...(TArgs);

			static void ExcludeParameterTypes(FunctionInfo& info)
			{
				if constexpr (sizeof...(TArgs) > 0)
				{
					ParameterTypesExcluder excluder(info);
					excluder.Exclude<TArgs...>();
				}
			}
		};

	public:

		template<typename TReturn, typename ...TArgs, typename Info = FunctionMetaInfo<TReturn, TArgs...>>
		FunctionInfo(std::initializer_list<const Attribute*> attributes, const char* name, TReturn(*address)(TArgs...), std::initializer_list<const char*> parameterNames) :
			FunctionInfo(attributes, name, reinterpret_cast<const Handle>(address), sizeof...(TArgs), ParameterType::Initializer<Info::template ReturnType>{})
		{
		}

	protected:

		template<typename TReturn>
		FunctionInfo(std::initializer_list<const Attribute*> attributes, const char* name, const Handle address, size_t argsCount, ParameterType::Initializer<TReturn> initializer) :
			MemberInfo(attributes, name),
			m_Address(address),
			m_ReturnType(initializer),
			m_Parameters(argsCount)
		{
		}

		template<typename Siganture>
		Siganture GetAddress() const
		{
			return *(Siganture*)(&m_Address);
		}

		class ParameterTypesExcluder
		{
		private:

			ParameterType* m_Parameters;
			uint32_t m_Index;

		public:

			ParameterTypesExcluder(FunctionInfo& info) :
				m_Parameters(const_cast<ParameterType*>(info.m_Parameters.data())),
				m_Index(0)
			{
			}

			template<typename T, typename ...TArgs>
			void Exclude()
			{
				new(&m_Parameters[m_Index]) ParameterType(ParameterType::Initializer<T>{});
				++m_Index;
				
				if constexpr (sizeof...(TArgs) > 0)
					Exclude<TArgs...>();
			}
		};

	private:

		template<typename T>
		bool CheckReturnType() const
		{
			return m_ReturnType == ParameterType(ParameterType::Initializer<T>{});
		}

		template<typename T, typename ...TOther>
		bool CheckParameterTypes(uint32_t i = 0) const
		{
			const bool result = m_Parameters[i] == ParameterType(ParameterType::Initializer<T>{});
			if (result == false)
				return false;

			if constexpr (sizeof...(TOther) > 0)
				return CheckParameterTypes<TOther...>(i + 1);
		}
	};
}
