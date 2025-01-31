#pragma once

#ifdef EXAMPLE_LIB_DLL_EXPORT
	#define EXAMPLE_LIB_API __declspec(dllexport)
#else
	#define EXAMPLE_LIB_API __declspec(dllimport)
#endif // EXAMPLE_DLL_DLL_EXPORT

#include "Generation/ExampleLibReflectionInclude.meta.h"
#include "ExampleLib/Attributes.h"