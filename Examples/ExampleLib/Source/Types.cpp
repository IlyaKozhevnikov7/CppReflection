#include "ExampleLib/Types.h"

int16_t ExampleType::m_StaticConstInt16 = 321;

void ExampleType::Print(const char* text) const
{
	if (text == nullptr)
		text = "[NULL]";

	printf("%s\n", text);
}
