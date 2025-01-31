#pragma once

#include "ExampleLib/Common.h"
#include "ExampleLib/Templates.h"
#include "ExampleLib/Enums.h"

#include <string>
#include <functional>

REFLECTABLE()
class BaseType
{
	#include "Generation/BaseType.meta.inl"

	FIELD()
	size_t m_BaseValue = 2;

public:

	virtual ~BaseType()
	{
		printf(__FUNCTION__ "\n");
	}

	CONSTRUCTOR()
	BaseType() = default;

	METHOD()
	virtual const char* GetName()
	{
		return "Base Type";
	}

	METHOD()
	virtual int GetInt()
	{ 
		return 1; 
	}
};

class NoReflectableType
{
private:

	int32_t m_IntValue;
	double m_DoubleValue;

public:

	virtual ~NoReflectableType()
	{
		printf(__FUNCTION__ "\n");
	}
};

REFLECTABLE()
class EXAMPLE_LIB_API ExampleType final : public BaseType, public NoReflectableType, public ExampleDll::MyPtr<bool>
{
	#include "ExampleType.meta.inl"

public:

	FIELD()
	uint32_t m_UInt32 = 99 + 1;

	FIELD(DisplayName("Self Pointer"))
	ExampleType* m_MyTypePtr = nullptr;

	FIELD()
	const float m_ConstFloat{ 1.23f };

	FIELD()
	std::string m_SomeString = "text";

	FIELD(DisplayName("Static Short"))
	static int16_t m_StaticConstInt16;

	FIELD()
	ExampleDll::MyPair<std::string, Numebers> m_Pair;

public:

	~ExampleType() override
	{
		printf(__FUNCTION__ "\n");
	}

	CONSTRUCTOR(DisplayName("Main constructor"))
	ExampleType()
	{
		printf(__FUNCTION__ "\n");
	}

	METHOD()
	static void StaticMethod(std::function<void(std::exception)>* unknownArg)
	{
		printf(__FUNCTION__ "\n");
	}

private:
	
	METHOD(DisplayName("Get integer method"))
	virtual int GetInt()
	{
		return 2;
	}

	METHOD()
	void Print(const char* text = "Hello from DllExportedType::Print") const;
};
