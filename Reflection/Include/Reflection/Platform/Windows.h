#pragma once

#include <Windows.h>

#undef LoadLibrary

namespace Reflection
{
	using AssemblyHandle = HMODULE;

	class Win32Platform final
	{
	public:
	
		static const AssemblyHandle LoadLibrary(const char* name);
	
		static void FreeLibrary(AssemblyHandle handle);
	
		static void* LoadFunction(AssemblyHandle handle, const char* name);
	
		static const AssemblyHandle GetThisModule(const void* ptr);
	};

	using Platform = Win32Platform;
}
