#pragma once

#include "Reflection/TypePtr.h"
#include "Reflection/Platform/Current.h"
#include <vector>

namespace Reflection
{
	class REFLECTION_API Assembly
	{
	private:

		const char*					m_Name;
		const AssemblyHandle		m_Handle;
		const std::vector<TypePtr>	m_Types;

	public:
		
		Assembly(const char* name, std::initializer_list<TypePtr> types);

		const char* GetName() const
		{
			return m_Name;
		}

		std::span<const TypePtr> GetTypes() const
		{
			return m_Types;
		}

		TypePtr GetType(const char* name, const char* namespaceName = nullptr) const;

		static const Assembly* Load(const char* name);

		static void Free(const Assembly* assembly);
	};
}
