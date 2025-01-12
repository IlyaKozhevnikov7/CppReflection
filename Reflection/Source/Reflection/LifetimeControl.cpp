#include "LifetimeControl.h"

namespace Reflection::Generaion
{
	void EnumProxyCtorImpl<1>::Func(void* ptr)
	{
		memset(ptr, 0, 1);
	}

	void EnumProxyCtorImpl<2>::Func(void* ptr)
	{
		memset(ptr, 0, 2);
	}
	
	void EnumProxyCtorImpl<4>::Func(void* ptr)
	{
		memset(ptr, 0, 4);
	}
	
	void EnumProxyCtorImpl<8>::Func(void* ptr)
	{
		memset(ptr, 0, 8);
	}
}
