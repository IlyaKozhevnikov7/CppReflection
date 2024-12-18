#pragma once

#include "Reflection/FieldInfo.h"
#include "Reflection/MethodInfo.h"
#include "Reflection/EnumValue.h"

namespace Reflection
{
	enum class TypeFlag : uint8_t
	{
		Primitive		= BIT<1>,
		Abstract		= BIT<2>,
		TemplateType	= BIT<3>,
		Enum			= BIT<4>
	};

	class REFLECTION_API Type : public MemberInfo
	{
		friend class Assembly;

	public:

		#include "Internal/TypeMemberIterators.inl"

		using GetActualTypeSignature = const Type*(*)(void*);
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

		size_t GetParentTypeCount(bool all = true) const;

		void GetParentTypes(const Type** types, bool all = true) const;

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

		bool IsTemplated() const
		{
			return m_Flags & TypeFlag::TemplateType;
		}

		bool IsEnum() const
		{
			return m_Flags & TypeFlag::Enum;
		}
	
		bool HasParents() const
		{
			return IsEnum()
				? false
				: m_InternalInfo.classInfo.parentTypes.empty() == false;
		}

		const Type* GetEnumBaseType() const
		{
			return IsEnum()
				? m_InternalInfo.enumInfo.base
				: nullptr;
		}

		const std::span<const EnumValue> GetEnumValues() const
		{
			return IsEnum()
				? m_InternalInfo.enumInfo.values
				: std::span<const EnumValue>();
		}

		template<typename TAction>
		void ForEachParent(TAction action, bool all = true) const
		{
			if (IsEnum())
				return;

			for (auto parentType : m_InternalInfo.classInfo.parentTypes)
			{
				action(parentType);

				if (all)
					parentType->ForEachParent(action);
			}
		}

		template<typename TAction>
		void ForEachField(TAction action) const
		{
			if (IsEnum())
				return;

			for (auto parentType : m_InternalInfo.classInfo.parentTypes)
				if (parentType != nullptr)
					parentType->ForEachField(action);

			for (auto& info : m_InternalInfo.classInfo.fieldInfos)
				action(info);
		}

		template<typename TAction>
		void ForEachMethod(TAction action) const
		{
			if (IsEnum())
				return;

			for (auto parentType : m_InternalInfo.classInfo.parentTypes)
				if (parentType != nullptr)
					parentType->ForEachMethod(action);

			for (auto& info : m_InternalInfo.classInfo.methodInfos)
				action(info);
		}

		const FieldInfo* GetField(const char* name) const;

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
		const Type* GetActualType(void* ptr) const
		{
			return m_GetActualType(ptr);
		}

	private:

		size_t GetParentTypeCountInternal(bool all) const;

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
			new(&m_InternalInfo) TypeInternalInfo();
		}

		/*
		*	Construct class/struct type
		*/
		template<typename T>
		Type(const char* namespaceName, const char* name, std::initializer_list<const Type*> parentTypes, std::initializer_list<FieldInfo> fieldInfos, std::initializer_list<MethodInfo> methodInfos, GetActualTypeSignature getActualType, T*) :
			MemberInfo({}, name),
			m_Size(Generation::SizeOf<T>),
			m_Namespace(namespaceName),
			m_Flags(ExcludeFlags<T>::Flags),
			m_GetActualType(getActualType),
			m_InternalInfo(parentTypes, fieldInfos, methodInfos, (T*)nullptr)
		{
		}

		/*
		*	TODO: rename generic
		*	Template type
		*/
		template<typename T>
		Type(const char* templateName, const char* namespaceName, const char* name, std::initializer_list<const Type*> parentTypes, std::initializer_list<FieldInfo> fieldInfos, std::initializer_list<MethodInfo> methodInfos, GetActualTypeSignature getActualType, T*) :
			MemberInfo({}, name),
			m_Size(Generation::SizeOf<T>),
			m_Namespace(namespaceName),
			m_Flags(ExcludeFlags<T>::Flags),
			m_GetActualType(getActualType),
			m_InternalInfo(templateName, parentTypes, fieldInfos, methodInfos, (T*)nullptr)
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
