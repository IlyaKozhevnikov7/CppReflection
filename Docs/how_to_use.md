# How to use CppReflection

Once your project is set up, you can start using reflection. Firstly you should define your reflectable type.

```cpp
#pragma once

// Auto generated file per project
#include "Generation/MyPluginReflectionInclude.meta.h"

REFLECTABLE()
class Calculator
{
    // Auto generated per reflectable class
	#include "Generation/Calculator.meta.inl"

	int a, b;

	CONSTRUCTOR()
	Calculator(int a, int b) :
		a(a),
		b(b)
	{
	}

	METHOD()
	int Calculate(int op)
	{
		switch (op)
		{
		case 1: return a + b;
		case 2: return a - b;
		case 3: return a * b;
		case 4: return a / b;
		}
	}
};
```

Then you can extract meta information. There are two ways to do it.

### Static extraction
To statically extract a type, use the following methods:

```cpp
TypePtr type = TypeOf<Calculator>::Get();
```
or
```cpp
auto calculator = new Calculator(1, 2);
TypePtr type = calculator->GetType();
```

To extract types statically, you must include ReflectionInclude file from the project where the type is declared.

```cpp
#include "MyPlugin/Generation/MyPluginReflectionInclude.meta.h"
...
// type extraction
```

### Dynamic extraction

For dynamic types extraction from a library, you need to use the Assembly class.

```cpp
const Assembly* plugin = Assembly::Load("MyPlugin"); // Loading the library
if (plugin == nullptr) return;

const TypePtr type = plugin->GetType("Calculator"); // Find the required type
if (type == nullptr) return;
	
const LifetimeControl* lifetimeControl = type->GetLifetimeControl(); // Check if the object can be initialized
if (lifetimeControl == nullptr) return;

const ConstructorInfo* constructor = lifetimeControl->GetConstructor<int32_t, int32_t>(); // Find the required constructor (by signature)
auto obj = malloc(type->GetSize()); // Allocate memory for object

// Object initialization
{
	uint32_t args[2] = { 3, 4 }; // All arguments are packed into one buffer
	InvokeInfo invoke =
	{
		.result = obj,
		.args = args
	};

	constructor->ConstructExplicit(&invoke); // Invoke constructor
}

// Method dynamic invoke
if (const MethodInfo* method = type->GetMethod("Calculate"))
{
	if (method->IsStatic() == false)
	{
		int32_t subOperation = 2;
		ArgumentsPack pack(obj, subOperation); // for non-static methods, the 1st argument is always an instance ptr
		int32_t result;

		InvokeInfo invoke =
		{
			.result = &result,
			.args = pack.Get()
		};

		method->InvokeExplicit(&invoke); // Invoke method
	}
}

lifetimeControl->Destroy(obj); // Invoke destructor
free(obj); // free object memory
```

### Common features

#### Access to parent types
```cpp
for (auto& info : type->GetParentInfos())
{
	auto type = info.type;
}
```

#### Access to fields
```cpp
FieldPtr fieldPtr = type->GetFieldPtr("m_BaseValue");

for (auto field : type->GetFields())
{
    auto name = field.GetName();
	auto type = field.GetParameterType();
}
```

#### Access to method
```cpp
auto method = type->GetMethod("Sum");

if (method->IsStatic() &&
	method->CheckSignature<int32_t, int32_t>())
{
	int32_t a = 3, b = 5, sum;
	sum = method->Invoke<int32_t>(a, b);
}
```

#### Access to enum values
```cpp
if (type->IsEnum())
{
    auto baseType = type->GetEnumBaseType();
    for (auto info : type->GetEnumValues())
    {
        auto name = info.name;
        uint64_t value = info.value;
    }
}
```

#### Cast

```cpp
TypePtr type = ... // extract type
void* obj = ... // create and initialize object

IInterface* interfacePtr = Cast<IInterface>(obj, type);
```

Note: when using cast, make sure that the type matches the type of the obj pointer. That is, if you want to cast an object to its original type, you need to pass interafce ptr and TypeOf<IInterface>
```cpp
void* original = Cast(interfacePtr, // from ptr
	TypeOf<IInterface>::Get(),  // from ptr type
	type);			    // to type
```
