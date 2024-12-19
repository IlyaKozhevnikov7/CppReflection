#pragma once

#include "Reflection/MemberInfo.h"
#include "Reflection/ParameterType.h"

namespace Reflection
{
	class Type;

	class FieldInfo : public MemberInfo
	{
	private:

		const ParameterType	m_ParameterType;
		const size_t		m_ValueOffset;
		const bool			m_IsStatic;

	public:

		const ParameterType& GetParameterType() const
		{
			return m_ParameterType;
		}

		size_t GetOffset() const
		{
			return m_ValueOffset;
		}

		bool IsStatic() const
		{
			return m_IsStatic;
		}

	public:

		template<typename T>
		FieldInfo(std::initializer_list<const Attribute*> attributes, const char* name, size_t valueOffset, BitMask<ParameterFlag> flags, T* type) :
			MemberInfo(attributes, name),
			m_ParameterType(ParameterType::Initializer<T>{}),
			m_ValueOffset(valueOffset),
			m_IsStatic(flags & ParameterFlag::Static) // TODO: remake ctor sign
		{
		}
	};

	class REFLECTION_API FieldRef
	{
	private:

		const FieldInfo*	m_Info;
		const size_t		m_Offset;

	public:

		FieldRef() :
			m_Info(nullptr),
			m_Offset(0)
		{
		}

		FieldRef(const FieldInfo* info, size_t offset) :
			m_Info(info),
			m_Offset(offset)
		{
		}

		const FieldInfo* GetInfo() const
		{
			return m_Info;
		}

		size_t GetOffset() const
		{
			return m_Offset;
		}

		bool IsValid() const
		{
			return m_Info != nullptr;
		}

		void* GetValue(void* object) const
		{
			return GetValueInternal((int8_t*)object);
		}

		template<typename TReturn, typename T = void>
		TReturn* GetValue(T* object = nullptr) const
		{
			static_assert(IsReflectable<T>);

			if (object == nullptr && m_Info->IsStatic() == false)
				return nullptr;

			return (TReturn*)GetValueInternal((int8_t*)object);
		}

	private:

		void* GetValueInternal(int8_t* object) const;
	};
}
