#include "Reflection/Type.h"
#include "Reflection/Assembly.h"

namespace Reflection
{
	size_t Type::GetParentTypeCountInternal(bool all) const
	{
		size_t count = m_InternalInfo.classInfo.parentTypes.size();

		if (all == false)
			return count;
		
		for (auto parentType : m_InternalInfo.classInfo.parentTypes)
			if (parentType != nullptr)
				count += parentType->GetParentTypeCountInternal(all);

		return count;
	}

	size_t Type::GetParentTypeCount(bool all) const
	{
		if (IsEnum())
			return 0;

		return GetParentTypeCountInternal(all);
	}

	void Type::GetParentTypes(const Type** types, bool all) const
	{
		if (IsEnum() || types == nullptr)
			return;

		size_t i = 0;

		for (auto parentType : m_InternalInfo.classInfo.parentTypes)
		{
			types[i] = parentType;
			++i;

			if (parentType != nullptr)
			{
				parentType->GetParentTypes(types + i);
				i += parentType->GetParentTypeCountInternal(false);
			}
		}
	}

	const FieldInfo* Type::GetField(const char* name) const
	{
		if (IsEnum())
			return nullptr;

		for (auto& info : m_InternalInfo.classInfo.fieldInfos)
			if (strcmp(info.GetName(), name) == 0)
				return &info;

		for (auto parentType : m_InternalInfo.classInfo.parentTypes)
		{
			if (parentType == nullptr)
				continue;
		
			auto info = parentType->GetField(name);
		
			if (info != nullptr)
				return info;
		}

		return nullptr;
	}

	const MethodInfo* Type::GetMethod(const char* name) const
	{
		if (IsEnum())
			return nullptr;
		
		for (auto& info : m_InternalInfo.classInfo.methodInfos)
			if (strcmp(info.GetName(), name) == 0)
				return &info;

		for (auto parentType : m_InternalInfo.classInfo.parentTypes)
		{
			if (parentType == nullptr)
				continue;

			auto info = parentType->GetMethod(name);

			if (info != nullptr)
				return info;
		}

		return nullptr;
	}

	bool Type::IsA(const Type* type) const
	{
		if (type == this)
			return true;

		for (auto parentType : m_InternalInfo.classInfo.parentTypes)
			if (parentType != nullptr && parentType->IsA(type))
				return true;

		return false;
	}
}
