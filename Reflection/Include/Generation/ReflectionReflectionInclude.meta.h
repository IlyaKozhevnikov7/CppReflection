/*============================================================
        This file was created using MetaGenerator
    !!!!! DO NOT MAKE ANY CHANGES TO THIS FILE !!!!!
============================================================*/
#pragma once

#ifdef REFLECTION_DLL_EXPORT
	#define __REFLECTION_REFLECTION_DLL_EXPORT_API__ __declspec(dllexport)
#else
	#define __REFLECTION_REFLECTION_DLL_EXPORT_API__ __declspec(dllimport)
#endif // REFLECTION_DLL_EXPORT

#include "Reflection/Type.h"

__GEN_REFLECTION_TYPE_FORWARD_DECLARATION(__REFLECTION_REFLECTION_DLL_EXPORT_API__, class, Attribute, __GEN_REFLECTION_NAMESPACE_WRAPPER, Reflection)
