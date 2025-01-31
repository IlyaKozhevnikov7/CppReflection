#pragma once

#include "AssemblyViewerLibCore.h"

namespace Reflection
{
	class TypePtr;
	class Assembly;
	class MemberInfo;
};

REFLECTABLE()
class AssemblyViewer
{
	#include "AssemblyViewer.meta.inl"

private:

	METHOD()
	static void DisplayInfo(const Reflection::Assembly* assembly);
};

