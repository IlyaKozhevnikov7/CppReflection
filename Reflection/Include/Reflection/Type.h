#pragma once

#include "Reflection/FieldInfo.h"
#include "Reflection/MethodInfo.h"
#include "Reflection/EnumValue.h"

namespace Reflection
{
	enum class TypeFlag : uint8_t
	{
		Primitive	= BIT<1>,
		Abstract	= BIT<2>,
		GenericType	= BIT<3>,
		Enum		= BIT<4>
	};	

	class REFLECTION_API Type : public MemberInfo
	{
		friend class Assembly;
		friend class TypePtr;

	public:

		struct ParentInfo
		{
			const TypePtr	type;
			const size_t	offset;
		};

		using GetActualTypeSignature = TypePtr(*)(void*);
		using CreateInstanceSignature = void*(*)();

	private:
	
		const Assembly*			m_Assembly;
		const size_t			m_Size;
		const char*				m_Namespace;
		const BitMask<TypeFlag>	m_Flags;
		GetActualTypeSignature	m_GetActualType;

	public:

		size_t GetSize() const
		{
			return m_Size;
		}
	
		const char* GetNamespace() const
		{
			return m_Namespace;
		}


		BitMask<TypeFlag> GetFlags() const
		{
			return m_Flags;
		}

		bool IsPrimitive() const
		{
			return m_Flags & TypeFlag::Primitive;
		}

		bool IsAbstract() const
		{
			return m_Flags & TypeFlag::Abstract;
		}

		bool IsGenericType() const
		{
			return m_Flags & TypeFlag::GenericType;
		}

		bool IsEnum() const
		{
			return m_Flags & TypeFlag::Enum;
		}
	
		bool HasParents() const
		{
			return IsEnum()
				? false
				: m_InternalInfo.classInfo.parentInfos.empty() == false;
		}

		std::span<const ParentInfo> GetParentInfos() const
		{
			if (IsEnum())
				return {};

			return m_InternalInfo.classInfo.parentInfos;
		}

		std::span<const FieldInfo> GetFields() const
		{
			if (IsEnum())
				return {};

			return m_InternalInfo.classInfo.fieldInfos;
		}

		std::span<const MethodInfo> GetMethods() const
		{
			if (IsEnum())
				return {};

			return m_InternalInfo.classInfo.methodInfos;
		}

		TypePtr GetEnumBaseType() const
		{
			return IsEnum()
				? m_InternalInfo.enumInfo.base
				: nullptr;
		}

		std::span<const EnumValue> GetEnumValues() const
		{
			if (IsEnum())
				return m_InternalInfo.enumInfo.values;

			return {};
		}

		const FieldInfo* GetField(const char* name) const;

		FieldRef GetFieldRef(const char* name) const;

		const MethodInfo* GetMethod(const char* name) const;

		bool IsA(const Type* type) const;
	
		template<typename T>
		bool IsA() const
		{
			const auto type = TypeOf<T>::GetType();	
			return type != nullptr && IsA(type);
		}

		/*
		*	NOTE: Make sure that ptr is really point at this type object
		*/
		TypePtr GetActualType(void* ptr) const
		{
			return m_GetActualType(ptr);
		}

	private:

		#include "Internal/TypeCore.inl"
		TypeInternalInfo m_InternalInfo;

	public:

		Type() :
			MemberInfo({}, nullptr),
			m_Assembly(nullptr),
			m_Size(0),
			m_Namespace(nullptr),
			m_GetActualType(nullptr)
		{
		}

		/*
		*	Construct class/struct type
		*/
		template<typename T>
		Type(const char* namespaceName, const char* name, std::initializer_list<ParentInfo> parentInfos, std::initializer_list<FieldInfo> fieldInfos, std::initializer_list<MethodInfo> methodInfos, GetActualTypeSignature getActualType, T*) :
			MemberInfo({}, name),
			m_Size(Generation::SizeOf<T>),
			m_Namespace(namespaceName),
			m_Flags(ExcludeFlags<T>::Flags),
			m_GetActualType(getActualType),
			m_InternalInfo(parentInfos, fieldInfos, methodInfos, (T*)nullptr)
		{
		}

		/*
		*	Generic type
		*/
		template<typename T>
		Type(const char* templateName, const char* namespaceName, const char* name, std::initializer_list<ParentInfo> parentInfos, std::initializer_list<FieldInfo> fieldInfos, std::initializer_list<MethodInfo> methodInfos, GetActualTypeSignature getActualType, T*) :
			MemberInfo({}, name),
			m_Size(Generation::SizeOf<T>),
			m_Namespace(namespaceName),
			m_Flags(ExcludeFlags<T>::Flags),
			m_GetActualType(getActualType),
			m_InternalInfo(templateName, parentInfos, fieldInfos, methodInfos, (T*)nullptr)
		{
		}

		/*
		*	Construct enum type
		*/
		template<typename T>
		Type(const char* namespaceName, const char* name, T*) :
			MemberInfo({}, name),
			m_Size(Generation::SizeOf<T>),
			m_Namespace(namespaceName),
			m_Flags(ExcludeFlags<T>::Flags),
			m_GetActualType(nullptr),
			m_InternalInfo((T*)nullptr)
		{
		}
	};
}
