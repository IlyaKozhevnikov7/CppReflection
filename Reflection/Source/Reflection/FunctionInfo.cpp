#include "FunctionInfo.h"

namespace Reflection
{
	void FunctionInfo::GetParameters(const ParameterType** parameters) const
	{
		for (size_t i = 0; i < m_Parameters.size(); i++)
			parameters[i] = &m_Parameters[i];
	}
}
