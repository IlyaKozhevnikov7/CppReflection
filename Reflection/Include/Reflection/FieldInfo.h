#pragma once

#include "Reflection/MemberInfo.h"
#include "Reflection/ParameterType.h"

namespace Reflection
{
	class Type;

	class REFLECTION_API FieldInfo : public MemberInfo
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

		[[deprecated]] size_t GetOffset() const
		{
			return m_ValueOffset;
		}

		bool IsStatic() const
		{
			return m_IsStatic;
		}

		const void* GetValue(void* object) const
		{
			return GetValueInternal((int8_t*)object);
		}

		template<typename TReturn, typename T = void>
		TReturn* GetValue(T* object = nullptr) const
		{
			static_assert(IsReflectable<T>);

			if (object == nullptr && IsStatic() == false)
				return nullptr;
					
			return (TReturn*)GetValueInternal((int8_t*)object);
		}

	private:

		void* GetValueInternal(int8_t* object) const;

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
}
