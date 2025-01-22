#pragma once

#include "Reflection/MemberInfo.h"
#include "Reflection/Internal/ProxyInvoker.h"

namespace Reflection
{
	class REFLECTION_API FunctionInfo : public MemberInfo
	{
	private:

		const Internal::Invoker	m_Invoker;

	public:

		ParameterType GetReturnType() const
		{
			return m_Invoker.GetReturnType();
		}

		std::span<const ParameterType> GetParameterTypes() const
		{
			return m_Invoker.GetParameterTypes();
		}

		template<typename TReturn, typename ...TArgs>
		bool CheckSignature() const
		{
			return m_Invoker.CheckReturnType<TReturn>() && m_Invoker.CheckParameterTypes<TArgs...>();
		}

		void InvokeExplicit(const InvokeInfo* info = nullptr) const
		{
			m_Invoker.Invoke(info);
		}

	protected:

		template<typename TSignature>
		FunctionInfo(std::initializer_list<const Attribute*> attributes, const char* name, Internal::ProxyInvokeSignature invoke, TSignature signature) :
			MemberInfo(attributes, name),
			m_Invoker(invoke, signature)
		{
		}
	};
}
