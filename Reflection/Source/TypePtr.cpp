#include "TypePtr.h"
#include "Type.h"

namespace Reflection
{
	bool TypePtr::operator==(const TypePtr& other) const noexcept
	{
		if (m_Type == other.m_Type)
			return true;

		if (m_Type->IsTemplated() == false && other->IsTemplated() == false)
			return false;

		const auto& thisTemplateClassInfo = m_Type->m_InternalInfo.templateClassInfo;
		const auto& otherTemplateClassInfo = other.m_Type->m_InternalInfo.templateClassInfo;
		
		if (thisTemplateClassInfo.name != otherTemplateClassInfo.name)
			return false;
		
		for (size_t i = 0; i < thisTemplateClassInfo.parameterTypes.size(); i++)
		{
			const auto& thisParam = thisTemplateClassInfo.parameterTypes[i];
			const auto& otherParam = otherTemplateClassInfo.parameterTypes[i];
		
			if (thisParam != otherParam)
				return false;
		}

		return true;
	}
}
