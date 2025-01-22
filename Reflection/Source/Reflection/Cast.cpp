#include "Cast.h"

namespace Reflection
{
	static bool GetUpCastOffset(TypePtr from, TypePtr to, size_t& offset)
	{
		for (auto& parent : from->GetParentInfos())
		{
			if (parent.type == nullptr)
				continue;

			if (parent.type == to)
			{
				offset += parent.offset;
				return true;
			}

			if (GetUpCastOffset(parent.type, to, offset))
			{
				offset += parent.offset;
				return true;
			}
		}

		return false;
	}

	void* Cast(void* ptr, TypePtr ptrType, TypePtr toType)
	{
		size_t offset = 0;
		if (GetUpCastOffset(ptrType, toType, offset)) // Try upcast from ptr type
			return reinterpret_cast<int8_t*>(ptr) + offset;

		auto actualType = ptrType->GetActualType(ptr);

		if (actualType == toType)
		{
			GetUpCastOffset(actualType, ptrType, offset);
			return reinterpret_cast<int8_t*>(ptr) - offset;
		}
		else
		{
			if (GetUpCastOffset(actualType, toType, offset))	 // Try downcast from actual type
				return reinterpret_cast<int8_t*>(ptr) - offset;
		}

		return nullptr;
	}
}
