#pragma once

#include "Reflection/Type.h"

namespace Reflection
{
	extern REFLECTION_API void* Cast(void* ptr, TypePtr ptrType, TypePtr toType);

	template<typename TTo, typename TFrom>
	TTo* Cast(TFrom* from)
	{
		if constexpr (std::is_convertible_v<TFrom, TTo>)
		{
			return from;
		}
		else
		{
			static_assert(IsReflectable<TTo>);
			static_assert(IsReflectable<TFrom>);
			
			return reinterpret_cast<TTo*>(Cast(reinterpret_cast<int8_t*>(from), TypeOf<TFrom>::Get(), TypeOf<TTo>::Get()));
		}
	}

	template<typename TTo>
	TTo* Cast(void* from, TypePtr ptrType)
	{
		return reinterpret_cast<TTo*>(Cast(reinterpret_cast<int8_t*>(from), ptrType, TypeOf<TTo>::Get()));
	}
}
