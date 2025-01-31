#include "AssemblyViewer.h"
#include "Reflection/Assembly.h"

#include "TypeViewer.h"

using namespace Reflection;

void AssemblyViewer::DisplayInfo(const Assembly* assembly)
{
	assert(assembly);

	printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Assembly Viewer >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
	printf("Name: %s\n", assembly->GetName());

	auto types = assembly->GetTypes();

	for (int i = 0; i < types.size(); i++)
	{
		TypeViewer typeViewer(types[i]);
		typeViewer.Display();
	}
}
