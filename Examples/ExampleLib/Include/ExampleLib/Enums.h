#pragma once

#include "ExampleLib/Common.h"

REFLECTABLE()
enum class Numebers : uint16_t
{
	ONE = -654321,
	TWO = 123,
	THREE = 3
};

namespace SomeNamespace
{
	REFLECTABLE(DisplayName("Fruits Enum"))
		enum Fruits
	{
		APPLE,
		BANANA,
		CHERRY
	};
}
