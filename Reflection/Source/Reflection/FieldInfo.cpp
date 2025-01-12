#include "Reflection/FieldInfo.h"

namespace Reflection
{
	void* FieldRef::GetValueInternal(int8_t* object) const
	{
		return m_Info->IsStatic()
			? (void*)m_Info->GetOffset()
			: (void*)(object + m_Info->GetOffset() + m_Offset);
	}
}
