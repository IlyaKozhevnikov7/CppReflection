#pragma once

#include "Reflection/Common.h"
#include <vector>

namespace Reflection
{
	class Attribute;

	class MemberInfoBase
	{
	private:

		const std::vector<const Attribute*>	m_Attributes;

	public:

		MemberInfoBase(std::initializer_list<const Attribute*> attributes) :
			m_Attributes(attributes)
		{
		}

		std::span<const Attribute* const> GetAttributes() const
		{
			return m_Attributes;
		}

		template<typename T>
		const T* GetAttribute() const
		{
			for (auto attribute : m_Attributes)
				if (attribute->GetType() == TypeOf<T>::Get())
					return (T*)attribute;

			return nullptr;
		}

		template<typename T>
		bool HasAttribute() const
		{
			return GetAttribute<T>() != nullptr;
		}
	};

	/*
	*	Used to describe named members
	*/
	class MemberInfo : public MemberInfoBase
	{
	private:

		const char*							m_Name;


	public:

		MemberInfo(std::initializer_list<const Attribute*> attributes, const char* name) :
			MemberInfoBase(attributes),
			m_Name(name)
		{
		}

		const char* GetName() const
		{
			return m_Name;
		}
	};
}
