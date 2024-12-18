#pragma once

#include "CppReflectionCore.h"
#include "Reflection/Platform/Current.h"
#include <vector>

namespace Reflection
{
	class Type;

	class REFLECTION_API Assembly
	{
	private:

		const char*						m_Name;
		const AssemblyHandle			m_Handle;
		const std::vector<const Type*>	m_Types;

	public:
		
		Assembly(const char* name, std::initializer_list<const Type*> types);

		const char* GetName() const
		{
			return m_Name;
		}

		std::span<const Type* const> GetTypes() const
		{
			return { m_Types.data(), m_Types.size() };
		}

		const Type* GetType(const char* name, const char* namespaceName = nullptr) const;

		static const Assembly* Load(const char* name);

		static void Free(const Assembly* assembly);
	};
}
