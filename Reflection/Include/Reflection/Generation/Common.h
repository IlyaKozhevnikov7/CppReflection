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
		return (size_t)(&(((T*)0)->*field));
	}

	template<typename U>
	size_t GetOffsetOfField(U* field)
	{
		return (size_t)(field);
	}

	template<typename T>
	TypePtr GetActualType(void* object)
	{
		return ((T*)object)->GetType();
	}
}

#include "Reflection/Generation/GenericTypeName.h"

namespace Reflection::Generation
{
	template<typename T, typename TSignature>
	struct ProxyCtor;

	template<typename T, typename... TArgs>
	struct ProxyCtor<T, void(TArgs...)>
	{
		static_assert(std::is_abstract_v<T> == false, "An abstract type cannot have a reflectable constructor");

		static void Func(void* ptr, TArgs... args)
		{
			new(ptr) T(std::forward<TArgs>(args)...);
		}
	};

	template<typename T>
	struct ProxyDestructor
	{
		static void Func(void* ptr)
		{
			((T*)ptr)->~T();
		}
	};
}
