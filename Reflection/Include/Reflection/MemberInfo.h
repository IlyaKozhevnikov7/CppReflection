#pragma once

#include "Reflection/Common.h"
#include <vector>

namespace Reflection
{
	class Attribute;

	class MemberInfo
	{
	private:

		const char*							m_Name;
		const std::vector<const Attribute*>	m_Attributes;

	public:

		MemberInfo(std::initializer_list<const Attribute*> attributes, const char* name) :
			m_Name(name),
			m_Attributes(attributes)
		{
		}

		const char* GetName() const
		{
			return m_Name;
		}

		const std::vector<const Attribute*>& GetAttributes() const
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
}
