#pragma once

#include "Reflection/FunctionInfo.h"

namespace Reflection
{
	enum class MethodFlags : uint8_t
	{
		Static		= BIT<1>,
		Const		= BIT<2>,
		Virtual		= BIT<3>,
		PureVirtual	= BIT<4>
	};

	class REFLECTION_API MethodInfo : public FunctionInfo
	{
	private:

		const TypePtr				m_ObjectType;
		const BitMask<MethodFlags>	m_Flags;

	public:

		TypePtr GetObjectType() const
		{
			return m_ObjectType;
		}

		bool IsStatic() const
		{
			return m_Flags & MethodFlags::Static;
		}

		bool IsConst() const
		{
			return m_Flags & MethodFlags::Const;
		}

		bool IsVirtual() const
		{
			return m_Flags & MethodFlags::Virtual;
		}

		bool IsPureVirtual() const
		{
			return m_Flags & MethodFlags::PureVirtual;
		}

		/*
		*	If the method is not static, the first argument is always
		*	interpreted as a pointer to an object.
		*/
		template<typename TReturn = void, typename ...TArgs>
		TReturn Invoke(TArgs... args) const
		{
			if (IsStatic() == false)
			{
				CheckInstanceArgument(std::forward<TArgs>(args)...);
			}

			ArgumentsPack<TArgs...> pack(std::forward<TArgs>(args)...);
			InvokeResult<TReturn> result;

			if (IsStatic() == false)
			{
				ConvertToValidInstacne<TArgs...>(pack.Get());
			}

			const InvokeInfo info =
			{
				.result = result.Get(),
				.args = pack.Get()
			};

			FunctionInfo::InvokeExplicit(&info);

			if constexpr (std::is_void_v<TReturn> == false)
			{
				if constexpr (std::is_move_constructible_v<TReturn>)
				{
					return std::move(*reinterpret_cast<TReturn*>(result.Get()));
				}
				else
				{
					return *reinterpret_cast<TReturn*>(result.Get());
				}
			}
		}

	private:

		template<typename T, typename... TOther>
		void CheckInstanceArgument(T instance, TOther... other) const
		{
			assert(std::is_pointer_v<T>
				&& std::is_pointer_v<std::remove_pointer_t<T>> == false
				&& "The first argument to a non-static method must be an instance pointer.");

			using InstanceType = std::remove_const_t<std::remove_pointer_t<T>>;
			assert(IsReflectable<InstanceType> && "The instance argument must be a reflectable type.");
			assert(instance && "The instance argument must be non-null.");
			if constexpr (std::is_pointer_v<T>)
			{
				auto instanceType = TypeOf<InstanceType>::Get()->GetActualType((void*)(instance));
				assert(instanceType && instanceType->IsA(m_ObjectType) && "The type of the instance argument must be based on or equal to the type of the method instance type.");
			}
		}

		template<typename... TArgs>
		void ConvertToValidInstacne(void* args) const
		{
			if constexpr (sizeof...(TArgs) > 0)
			{
				using PtrType = std::tuple<TArgs...>::_This_type;
				using InstanceType = std::remove_const_t<std::remove_pointer_t<PtrType>>;
				auto ptr = *reinterpret_cast<void**>(args);

				if (TypeOf<InstanceType>::Get()->GetActualType(ptr) != m_ObjectType)
					Cast(ptr, TypeOf<PtrType>::Get(), m_ObjectType);
			}
		}

		template<typename TSignature>
		constexpr static MethodFlags Flags = MethodFlags(0);

		template<typename TReturn, typename T, typename... TArgs>
		constexpr static MethodFlags Flags<TReturn(T::*)(TArgs...) const> = MethodFlags::Const;

		template<typename TReturn, typename... TArgs>
		constexpr static MethodFlags Flags<TReturn(*)(TArgs...)> = MethodFlags::Static;

	public:

		template<typename T, typename TSignature>
		MethodInfo(std::initializer_list<const Attribute*> attributes, const char* name, Internal::ProxyInvokeSignature invoke, std::initializer_list<const char*> parameterNames, const T* objectType, const TSignature signature, BitMask<MethodFlags> flags) :
			FunctionInfo(attributes, name, invoke, signature),
			m_ObjectType(TypeOf<T>::Get()),
			m_Flags(flags | Flags<TSignature>)
		{
		}
	};
}
