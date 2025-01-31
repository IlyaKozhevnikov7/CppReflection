#pragma once

#include "ExampleLib/Common.h"
#include "Reflection/Attribute.h"

REFLECTABLE()
class DisplayName : public Reflection::Attribute
{
	#include "Generation/DisplayName.meta.inl"

private:

	const char* m_Name;

public:

	DisplayName(const char* name) :
		m_Name(name)
	{
	}

	const char* GetName() const
	{
		return m_Name;
	}
};
