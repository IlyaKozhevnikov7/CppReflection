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

	class REFLECTION_API MethodInfo final : public FunctionInfo
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

		template<typename TReturn = void, typename T = void*, typename ...TArgs>
		TReturn Invoke(T* object, TArgs... args) const
		{
			assert((CheckSiganture<TReturn, TArgs...>()) && "Signature mismatch.");
			assert(IsStatic() == false);

			if (IsConst())
			{
				using Signature = TReturn(T::*)(TArgs...) const;
				return (object->*GetAddress<Signature>())(std::forward<TArgs>(args)...);
			}
			else
			{
				using Signature = TReturn(T::*)(TArgs...);
				return (object->*GetAddress<Signature>())(std::forward<TArgs>(args)...);
			}
		}

		template<typename TReturn = void, typename ...TArgs>
		TReturn InvokeStatic(TArgs... args) const
		{
			assert(IsStatic());
			return FunctionInfo::Invoke<TReturn>(std::forward<TArgs>(args)...);
		}

	private:

		template<typename TSignature>
		struct MethodMetaInfo;

		template<typename T, typename TReturn, typename... TArgs>
		struct MethodMetaInfo<TReturn(T::*)(TArgs...)> : public FunctionMetaInfo<TReturn, TArgs...>
		{
			constexpr static MethodFlags Flags = MethodFlags(0);
		};

		template<typename TReturn, typename T, typename... TArgs>
		struct MethodMetaInfo<TReturn(T::*)(TArgs...) const> : public FunctionMetaInfo<TReturn, TArgs...>
		{
			constexpr static MethodFlags Flags = MethodFlags::Const;
		};

		template<typename TReturn, typename... TArgs>
		struct MethodMetaInfo<TReturn(*)(TArgs...)> : public FunctionMetaInfo<TReturn, TArgs...>
		{
			constexpr static MethodFlags Flags = MethodFlags::Static;
		};

		template<typename TObject, typename TSignature>
		struct ReplaceMethodObjectType;

		/*
		*	Default method
		*/
		template<typename TNew, typename TRet, typename T, typename... TArgs>
		struct ReplaceMethodObjectType<TNew, TRet(T::*)(TArgs...)>
		{
			using Type = TRet(TNew::*)(TArgs...);
		};

		/*
		*	Const method
		*/
		template<typename TNew, typename TRet, typename T, typename... TArgs>
		struct ReplaceMethodObjectType<TNew, TRet(T::*)(TArgs...) const>
		{
			using Type = TRet(TNew::*)(TArgs...) const;
		};

		/*
		*	Noexcept method
		*/
		template<typename TNew, typename TRet, typename T, typename... TArgs>
		struct ReplaceMethodObjectType<TNew, TRet(T::*)(TArgs...) noexcept>
		{
			using Type = TRet(TNew::*)(TArgs...) noexcept;
		};

		/*
		*	Const noexcept method
		*/
		template<typename TNew, typename TRet, typename T, typename... TArgs>
		struct ReplaceMethodObjectType<TNew, TRet(T::*)(TArgs...) const noexcept>
		{
			using Type = TRet(TNew::*)(TArgs...) const noexcept;
		};

		/*
		*	Static method
		*/
		template<typename TNew, typename TRet, typename... TArgs>
		struct ReplaceMethodObjectType<TNew, TRet(*)(TArgs...)>
		{
			using Type = TRet(*)(TArgs...);
		};

	public:

		template<typename T, typename TProxySignature, typename TSignature = ReplaceMethodObjectType<T, TProxySignature>::Type, typename Info = MethodMetaInfo<TSignature>>
		MethodInfo(std::initializer_list<const Attribute*> attributes, const char* name, TSignature address, std::initializer_list<const char*> parameterNames, const T* objectType, const TProxySignature signature, BitMask<MethodFlags> flags) :
			FunctionInfo(attributes, name, *reinterpret_cast<Handle*>(&address), Info::ArgsCount, ParameterType::Initializer<Info::template ReturnType>{}),
			m_ObjectType(TypeOf<T>::Get()),
			m_Flags(flags | Info::Flags)
		{
			Info::ExcludeParameterTypes(*this);
		}
	};
}
