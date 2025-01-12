#pragma once

#ifdef REFLECTION_DLL_EXPORT
	#define REFLECTION_API __declspec(dllexport)
#else
	#define REFLECTION_API __declspec(dllimport)
#endif // REFLECTION_DLL_EXPORT

#include <stdint.h>
#include <assert.h>
#include <array>
#include <initializer_list>
#include <span>

/*
*	Main macro that is used to indicate that a type or its member uses reflection
*/
#define REFLECTABLE(...)
#define CONSTRUCTOR(...)
#define FIELD(...)
#define METHOD(...)
