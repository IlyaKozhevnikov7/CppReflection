#include "Reflection/Type.h"
#include "Reflection/Assembly.h"

using namespace Reflection;

static void RunAssemblyViewer(const Assembly* externalAssembly)
{
	const auto assembly = Assembly::Load("AssemblyViewerLib");
	assert(assembly);

	const auto viewerType = assembly->GetType("AssemblyViewer");
	assert(viewerType);

	const auto displayInfoMethod = viewerType->GetMethod("DisplayInfo");
	assert(displayInfoMethod);
	assert(displayInfoMethod->IsStatic());
	assert((displayInfoMethod->CheckSignature<void, const Assembly*>()));

	displayInfoMethod->Invoke(externalAssembly);

	Assembly::Free(assembly);
}

int main()
{
#define RUN_EXAMPLE_1

#ifdef RUN_EXAMPLE_1
	const char* assemblyName = "ExampleLib";
	auto exampleDllAssembly = Assembly::Load(assemblyName);
	RunAssemblyViewer(exampleDllAssembly);
	Assembly::Free(exampleDllAssembly);
#endif // RUN_EXAMPLE_1

	return 0;
}