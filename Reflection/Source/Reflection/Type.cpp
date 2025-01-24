#include "Reflection/Type.h"
#include "Reflection/Assembly.h"

namespace Reflection
{
	const FieldInfo* Type::GetField(const char* name) const
	{
		if (IsEnum())
			return nullptr;

		for (auto& info : m_InternalInfo.classInfo.fieldInfos)
			if (strcmp(info.GetName(), name) == 0)
				return &info;

		for (auto& parent : m_InternalInfo.classInfo.parentInfos)
		{
			if (parent.type == nullptr)
				continue;
			
			auto field = parent.type->GetField(name);
			
			if (field != nullptr)
				return field;
		}

		return nullptr;
	}

	FieldPtr Type::GetFieldPtr(const char* name) const
	{
		if (IsEnum())
			return {};

		for (auto& info : m_InternalInfo.classInfo.fieldInfos)
			if (strcmp(info.GetName(), name) == 0)
				return { &info, 0 };

		for (auto& parent : m_InternalInfo.classInfo.parentInfos)
		{
			if (parent.type == nullptr)
				continue;

			const auto ptr = parent.type->GetFieldPtr(name);

			if (ptr.IsValid())
				return { ptr.GetInfo(), ptr.GetOffset() + parent.offset };
		}

		return {};
	}

	const MethodInfo* Type::GetMethod(const char* name) const
	{
		if (IsEnum())
			return nullptr;
		
		for (auto& info : m_InternalInfo.classInfo.methodInfos)
			if (strcmp(info.GetName(), name) == 0)
				return &info;

		for (auto& parent : m_InternalInfo.classInfo.parentInfos)
		{
			if (parent.type == nullptr)
				continue;

			auto info = parent.type->GetMethod(name);

			if (info != nullptr)
				return info;
		}

		return nullptr;
	}

	bool Type::IsA(const Type* type) const
	{
		if (type == this)
			return true;

		for (auto parent : m_InternalInfo.classInfo.parentInfos)
			if (parent.type != nullptr && parent.type->IsA(type))
				return true;

		return false;
	}
}
