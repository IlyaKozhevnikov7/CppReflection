#pragma once

#include "Reflection/MemberInfo.h"
#include "Reflection/ParameterType.h"

namespace Reflection
{
	class ConstructorInfo : public MemberInfoBase
	{
	public:

		using Handle = size_t*;

	private:

		const Handle						m_Address;
		const std::vector<ParameterType>	m_Parameters;

	public:

		size_t GetParameterCount() const
		{
			return m_Parameters.size();
		}

		std::span<const ParameterType> GetParameters() const
		{
			return m_Parameters;
		}

		template<typename ...TArgs>
		bool CheckSiganture() const
		{
			if constexpr (sizeof...(TArgs) == 0)
			{
				return m_Parameters.empty();
			}
			else
			{
				return m_Parameters.size() == sizeof...(TArgs) && ParameterType::Comparator(m_Parameters.data()).Process<0, TArgs...>();
			}
		}

		template<typename ...TArgs>
		void Construct(void* ptr, TArgs... args) const
		{
			assert((CheckSiganture<TArgs...>()) && "Constructor signature mismatch");

			using Signature = void(*)(void*, TArgs...);
			((Signature)m_Address)(ptr, std::forward<TArgs>(args)...);
		}

	public:

		template<typename... TArgs>
		ConstructorInfo(std::initializer_list<const Attribute*> attributes, void(*address)(void*, TArgs...)) :
			MemberInfoBase(attributes),
			m_Address(*reinterpret_cast<Handle*>(&address)),
			m_Parameters(sizeof...(TArgs))
		{
			if constexpr (sizeof...(TArgs) > 0)
			{
				ParameterType::Excluder(m_Parameters.data()).Process<0, TArgs...>();
			}
		}


	private:

		template<typename T, typename ...TOther>
		bool CheckParameterTypes(uint32_t i = 0) const
		{
			if ((m_Parameters[i] == ParameterType(ParameterType::Initializer<T>{})) == false)
				return false;

			if constexpr (sizeof...(TOther) > 0)
			{
				// TODO: remake with template index
				return CheckParameterTypes<TOther...>(i + 1);
			}
		}
	};

	class REFLECTION_API LifetimeControl
	{
	public:

		using DestructorSignature = void(*)(void*);

	private:

		const DestructorSignature			m_Destructor;
		const std::vector<ConstructorInfo>	m_ConstructorInfos;

	public:

		bool IsValid() const
		{
			return m_ConstructorInfos.empty() == false;
		}

		std::span<const ConstructorInfo> GetConstructors() const
		{
			return m_ConstructorInfos;
		}

		template<typename... TArgs>
		const ConstructorInfo* GetConstructor() const
		{
			for (auto& info : m_ConstructorInfos)
				if (info.CheckSiganture<TArgs...>())
					return &info;

			return nullptr;
		}

		void Destroy(void* ptr) const
		{
			if (ptr != nullptr && m_Destructor != nullptr)
				m_Destructor(ptr);
		}

	public:

		LifetimeControl(std::initializer_list<ConstructorInfo> constructorInfos, DestructorSignature destructor) :
			m_ConstructorInfos(constructorInfos),
			m_Destructor(destructor)
		{
		}

		LifetimeControl() :
			m_Destructor(nullptr)
		{
		}
	};

	namespace Generaion
	{
		template<size_t Size>
		struct EnumProxyCtorImpl {};

		template<> struct REFLECTION_API EnumProxyCtorImpl<1> { static void Func(void*); };
		template<> struct REFLECTION_API EnumProxyCtorImpl<2> { static void Func(void*); };
		template<> struct REFLECTION_API EnumProxyCtorImpl<4> { static void Func(void*); };
		template<> struct REFLECTION_API EnumProxyCtorImpl<8> { static void Func(void*); };

		template<typename TEnum>
		struct EnumProxyCtor : EnumProxyCtorImpl<sizeof(TEnum)> {};
	}
}
