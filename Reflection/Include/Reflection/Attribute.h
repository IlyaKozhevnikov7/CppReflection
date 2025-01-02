#pragma once

#include "Reflection/TypePtr.h"
#include "Generation/ReflectionReflectionInclude.meta.h"

namespace Reflection
{
	REFLECTABLE()
	class REFLECTION_API Attribute
	{
		#include "Generation/Reflection.Attribute.meta.inl"

		template<typename TAttribute, typename ...Args>
		static TAttribute* Create(Args&&... args)
		{
			static_assert(std::is_base_of_v<Attribute, TAttribute>);

			if constexpr (sizeof(TAttribute) == sizeof(Attribute))
			{
				static TAttribute instance(std::forward<Args>(args)...);
				return &instance;
			}
			else
			{
				return new TAttribute(std::forward<Args>(args)...);
			}
		}
	};
}
