/*============================================================
        This file was created using MetaGenerator
    !!!!! DO NOT MAKE ANY CHANGES TO THIS FILE !!!!!
============================================================*/
#include "Reflection/Assembly.h"
#include "Generation/ReflectionReflectionInclude.meta.h"

using namespace Reflection;

static Assembly assembly("Reflection",
	{
		TypeOf<void>::Get(),
		TypeOf<bool>::Get(),
		TypeOf<int8_t>::Get(),
		TypeOf<uint8_t>::Get(),
		TypeOf<int16_t>::Get(),
		TypeOf<uint16_t>::Get(),
		TypeOf<int32_t>::Get(),
		TypeOf<uint32_t>::Get(),
		TypeOf<int64_t>::Get(),
		TypeOf<uint64_t>::Get(),
		TypeOf<float>::Get(),
		TypeOf<double>::Get(),
		TypeOf<char>::Get(),
		TypeOf<Reflection::Attribute>::Get(),
	});

extern "C"
{
	__REFLECTION_REFLECTION_DLL_EXPORT_API__ const Assembly* __GEN_REFLECTION_GET_ASSEMBLY()
	{
		return &assembly;
	}
}
