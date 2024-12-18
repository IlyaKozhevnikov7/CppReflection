#include "Reflection/Assembly.h"
#include "Reflection/Type.h"

namespace Reflection
{
	Assembly::Assembly(const char* name, std::initializer_list<const Type*> types) :
		m_Name(name),
		m_Handle(Platform::GetThisModule(this)),
		m_Types(types)
	{	
		for (auto type : m_Types)
			const_cast<Type*>(type)->m_Assembly = this;
	}

	const Type* Assembly::GetType(const char* name, const char* namespaceName) const
	{
		if (name == nullptr)
			return nullptr;

		for (auto& type : m_Types)
		{
			if (namespaceName != nullptr
				&& type->GetNamespace() != nullptr
				&& strcmp(type->GetNamespace(), namespaceName) != 0)
			{
				continue;
			}

			if (strcmp(type->GetName(), name) == 0)
				return type;
		}

		return nullptr;
	}

	const Assembly* Assembly::Load(const char* name)
	{
		using GetAssemblySignature = const Assembly*(*)();

		auto handle = Platform::LoadLibrary(name);

		if (handle == nullptr)
			return nullptr;

		const auto function = (GetAssemblySignature)Platform::LoadFunction(handle, "__GEN_REFLECTION_GET_ASSEMBLY");

		if (function == nullptr)
		{
			Platform::FreeLibrary(handle);
			return nullptr;
		}

		const auto assembly = function();

		if (assembly == nullptr)
			Platform::FreeLibrary(handle);

		return assembly;
	}

	void Assembly::Free(const Assembly* assembly)
	{
		if (assembly == nullptr)
			return;

		Platform::FreeLibrary(assembly->m_Handle);
	}
}
