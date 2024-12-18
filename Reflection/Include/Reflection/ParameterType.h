#pragma once

#include "Reflection/Common.h"

namespace Reflection
{
	class Type;

	enum class ParameterFlag : uint8_t
	{
		Static				= BIT<1>, // TODO remove
		Const				= BIT<2>,
		Pointer				= BIT<3>,
		PointerToPointer	= BIT<4>,
		Reference			= BIT<5>
	};

	class ParameterType
	{
	private:
	
		const Type*						m_Type;
		const BitMask<ParameterFlag>	m_Flags;
	
	public:
	
		const Type* GetType() const
		{
			return m_Type;
		}
	
		BitMask<ParameterFlag> GetFlags() const
		{
			return m_Flags;
		}
	
		bool IsConst() const
		{
			return m_Flags & ParameterFlag::Const;
		}
	
		bool IsPointer() const
		{
			return m_Flags & ParameterFlag::Pointer;
		}
	
		bool IsPointerToPointer() const
		{
			return m_Flags & ParameterFlag::PointerToPointer;
		}

		bool IsReference() const
		{
			return m_Flags & ParameterFlag::Reference;
		}
	
		bool operator==(const ParameterType& other) const
		{
			return m_Type == other.m_Type
				&& m_Flags == other.m_Flags;
		}

	private:
	
		template<typename T, ParameterFlag Flag>
		struct FlagConditional;
	
		template<typename T>
		struct FlagConditional<T, ParameterFlag::Const> : std::bool_constant<std::is_const_v<T>> {};
	
		template<typename T>
		struct FlagConditional<T, ParameterFlag::Pointer> : std::bool_constant<std::is_pointer_v<T>> {};
	
		template<typename T>
		struct FlagConditional<T, ParameterFlag::PointerToPointer> : std::bool_constant<std::is_pointer_v<std::remove_pointer_t<T>>> {};
	
		template<typename T>
		struct FlagConditional<T, ParameterFlag::Reference> : std::bool_constant<std::is_reference_v<T>> {};
	
		template<typename T>
		struct ExcludeFlags
		{
			template<ParameterFlag Flag>
			constexpr static BitMask<ParameterFlag> FLAG = FlagConditional<T, Flag>::value ? Flag : ParameterFlag(0);
			constexpr static BitMask<ParameterFlag> Flags = FLAG<ParameterFlag::Const> | FLAG<ParameterFlag::Pointer> | FLAG<ParameterFlag::PointerToPointer> | FLAG<ParameterFlag::Reference>;
		};
	
		template<typename T>
		struct RemoveAllPointers : std::conditional_t<std::is_pointer_v<T>, RemoveAllPointers<std::remove_pointer_t<T>>, std::type_identity<T>> {};
	
		template<typename T>
		using RemoveAllPointersType = typename RemoveAllPointers<T>::type;
	
		template<typename T>
		struct ExcludeAbsolute
		{
			using Type = std::remove_cv_t<
				RemoveAllPointersType<
				std::remove_reference_t<T>>>;
		};
	
		template<typename T>
		using AbsoluteType = typename ExcludeAbsolute<T>::Type;
	
	public:
	
		template<typename T>
		struct Initializer
		{
			constexpr static BitMask<ParameterFlag> Flags = ExcludeFlags<T>::Flags;
	
			constexpr static const Type* GetType()
			{
				return TypeOf<AbsoluteType<T>>::Get();
			}
		};
	
		ParameterType() :
			m_Type(nullptr),
			m_Flags(ParameterFlag(0))
		{
		}
	
		template<typename T>
		ParameterType(Initializer<T>) :
			m_Type(Initializer<T>::GetType()),
			m_Flags(Initializer<T>::Flags)
		{
		}
	};
}
