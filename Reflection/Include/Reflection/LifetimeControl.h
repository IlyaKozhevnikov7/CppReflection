#pragma once

#include "Reflection/MemberInfo.h"
#include "Reflection/Internal/ProxyInvoker.h"

namespace Reflection
{
	class ConstructorInfo : public MemberInfoBase
	{
	private:

		Internal::VoidInvoker m_Invoker;

	public:

		std::span<const ParameterType> GetParameters() const
		{
			return m_Invoker.GetParameterTypes();
		}

		template<typename ...TArgs>
		bool CheckParameters() const
		{
			return m_Invoker.CheckParameterTypes<TArgs...>();
		}

		template<typename... TArgs>
		void Construct(void* ptr, TArgs... args) const
		{
			assert((CheckParameters<TArgs...>()) && "Constructor signature mismatch.");
			ArgumentsPack<TArgs...> pack(std::forward<TArgs>(args)...);

			const InvokeInfo info =
			{
				.result = ptr,
				.args = pack.Get()
			};

			m_Invoker.Invoke(&info);
		}

		void ConstructExplicit(const InvokeInfo* info) const
		{
			assert(info->result && "The result must point to the object's memory.");
			m_Invoker.Invoke(info);
		}
			
	public:

		template<typename TSignature>
		ConstructorInfo(std::initializer_list<const Attribute*> attributes, Internal::ProxyInvokeSignature invoker, TSignature signature) :
			MemberInfoBase(attributes),
			m_Invoker(invoker, signature)
		{
		}
	};

	class LifetimeControl
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
				if (info.CheckParameters<TArgs...>())
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

	namespace Generation
	{
		struct ProxyCtor
		{
			template<typename T, typename... _TArgs, size_t... Indices>
			static void InvokeInternal(const InvokeInfo* info, std::tuple<_TArgs...>*, std::index_sequence<Indices...>)
			{
				const void* args = info->args;
				new(info->result) T(__GEN_PASS_ARGS);
			}

			template<typename T, typename TSignature>
			static void Invoke(const InvokeInfo* info)
			{
				using Args = FunctionCoreInfo<TSignature>::Args;
				InvokeInternal<T>(info, (Args*)nullptr, std::make_index_sequence<std::tuple_size_v<Args>>{});
			}
		};

		template<typename T>
		struct ProxyDestructor
		{
			static void Invoke(void* ptr)
			{
				if constexpr (std::is_trivially_destructible_v<T> == false)
				{
					reinterpret_cast<T*>(ptr)->~T();
				}
			}
		};

		template<size_t Size>
		struct EnumProxyCtorImpl {};

		template<> struct REFLECTION_API EnumProxyCtorImpl<1> { static void Invoke(const InvokeInfo* info); };
		template<> struct REFLECTION_API EnumProxyCtorImpl<2> { static void Invoke(const InvokeInfo* info); };
		template<> struct REFLECTION_API EnumProxyCtorImpl<4> { static void Invoke(const InvokeInfo* info); };
		template<> struct REFLECTION_API EnumProxyCtorImpl<8> { static void Invoke(const InvokeInfo* info); };

		template<typename TEnum>
		struct EnumProxyCtor : EnumProxyCtorImpl<sizeof(TEnum)> {};
	}
}
