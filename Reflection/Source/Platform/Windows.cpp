#include "Platform/Current.h"

namespace Reflection
{
	const AssemblyHandle Win32Platform::LoadLibrary(const char* name)
	{
		return ::LoadLibraryA(name);
	}

	void Win32Platform::FreeLibrary(AssemblyHandle handle)
	{
		::FreeLibrary(handle);
	}

	void* Win32Platform::LoadFunction(AssemblyHandle handle, const char* name)
	{
		return ::GetProcAddress(handle, name);
	}

	const AssemblyHandle Win32Platform::GetThisModule(const void* ptr)
	{
		HMODULE module = nullptr;
		GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCTSTR)ptr, &module);

		return module;
	}
}
