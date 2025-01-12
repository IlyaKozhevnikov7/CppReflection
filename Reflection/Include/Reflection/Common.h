#pragma once

#include "Reflection/TypePtr.h"
#include "Reflection/Utils/BitMask.h"
#include "Reflection/GenerationMacros.h"
#include "Reflection/EnumValue.h"

namespace Reflection
{
	class Type;

	template<typename T>
	struct TypeOf
	{
		constexpr static uint32_t NameLength = 0;

		constexpr static const char* GetName()
		{
			return nullptr;
		}

		constexpr static TypePtr Get()
		{
			return nullptr;
		}
	};

	template<typename T>
	inline constexpr bool IsReflectable = false;

	template<typename T>
	struct EnumValues
	{
		constexpr static std::span<const EnumValue> Get()
		{
			return { };
		}
	};
}

#include "Reflection/Generation/Common.h"

REFLECTION_PRIMITIVE_TYPE_FORWARD_DECLARATION(REFLECTION_API, void, "void");
REFLECTION_PRIMITIVE_TYPE_FORWARD_DECLARATION(REFLECTION_API, bool, "bool");
REFLECTION_PRIMITIVE_TYPE_FORWARD_DECLARATION(REFLECTION_API, int8_t, "int8");
REFLECTION_PRIMITIVE_TYPE_FORWARD_DECLARATION(REFLECTION_API, uint8_t, "uint8");
REFLECTION_PRIMITIVE_TYPE_FORWARD_DECLARATION(REFLECTION_API, int16_t, "int16");
REFLECTION_PRIMITIVE_TYPE_FORWARD_DECLARATION(REFLECTION_API, uint16_t, "uint16");
REFLECTION_PRIMITIVE_TYPE_FORWARD_DECLARATION(REFLECTION_API, int32_t, "int32");
REFLECTION_PRIMITIVE_TYPE_FORWARD_DECLARATION(REFLECTION_API, uint32_t, "uint32");
REFLECTION_PRIMITIVE_TYPE_FORWARD_DECLARATION(REFLECTION_API, int64_t, "int32");
REFLECTION_PRIMITIVE_TYPE_FORWARD_DECLARATION(REFLECTION_API, uint64_t, "uint64");
REFLECTION_PRIMITIVE_TYPE_FORWARD_DECLARATION(REFLECTION_API, float, "float");
REFLECTION_PRIMITIVE_TYPE_FORWARD_DECLARATION(REFLECTION_API, double, "double");
REFLECTION_PRIMITIVE_TYPE_FORWARD_DECLARATION(REFLECTION_API, char, "char");
