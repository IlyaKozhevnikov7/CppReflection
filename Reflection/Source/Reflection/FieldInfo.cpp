#include "Reflection/FieldInfo.h"
#include "Reflection/Type.h"

namespace Reflection
{
	void* FieldInfo::GetValueInternal(int8_t* object) const
	{
		return IsStatic()
			? (void*)m_ValueOffset
			: (void*)(object + m_ValueOffset);
	}
}
