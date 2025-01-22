#include "LifetimeControl.h"

namespace Reflection::Generation
{
	void EnumProxyCtorImpl<1>::Invoke(const InvokeInfo* info)
	{
		memset(info->result, 0, 1);
	}

	void EnumProxyCtorImpl<2>::Invoke(const InvokeInfo* info)
	{
		memset(info->result, 0, 2);
	}

	void EnumProxyCtorImpl<4>::Invoke(const InvokeInfo* info)
	{
		memset(info->result, 0, 4);
	}

	void EnumProxyCtorImpl<8>::Invoke(const InvokeInfo* info)
	{
		memset(info->result, 0, 8);
	}
}
