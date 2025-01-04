#pragma once

namespace Reflection::Generation
{
	template<typename T>
	inline constexpr size_t SizeOf = sizeof(T);

	template<>
	inline constexpr size_t SizeOf<void> = 0;

	template<typename T, typename U>
	bool IsStaticField(U T::*)
	{
		return false;
	}

	template<typename U>
	bool IsStaticField(U* field)
	{
		return true;
	}

	template<typename T, typename U>
	size_t GetOffsetOfField(U T::* field)
	{
		return reinterpret_cast<size_t>(&(((T*)0)->*field));
	}

	template<typename U>
	size_t GetOffsetOfField(U* field)
	{
		return reinterpret_cast<size_t>(field);
	}

	template<typename T>
	TypePtr GetActualType(void* object)
	{
		return ((T*)object)->GetType();
	}
}

#include "Reflection/Generation/GenericTypeName.h"
