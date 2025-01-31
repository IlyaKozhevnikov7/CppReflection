#include "TypeViewer.h"
#include "Reflection/Attribute.h"
#include <cinttypes>

using namespace Reflection;

constexpr static const char* BoolString[2] = { "false", "true" };

static const char* GetTypeName(TypePtr type)
{
	return type == nullptr
		? "[Unknown Type]"
		: type->GetName();
}

static void PrintTabs(int count)
{
	for (int i = 0; i < count; i++)
		printf("    ");
}

void TypeViewer::Display()
{
	DisplayHeader();
	DisplayMemberAttributes(m_Type);

	if (m_Type->IsEnum())
	{
		DisplayEnum();
	}
	else
	{
		DisplayParents();
		DisplayFields();
		DisplayMethods();
	}

	DisplayConstructors();
}

void TypeViewer::DisplayHeader()
{
	printf("============================== Type ==============================\n");
	printf("Type name: %s\n", m_Type->GetName());
	printf("Namespace: %s\n", (m_Type->GetNamespace() != nullptr ? m_Type->GetNamespace() : "[NONE]"));
	printf("Size: %zu bytes\n", m_Type->GetSize());

	printf("Flags:");

	if (m_Type->GetFlags() == false)
	{
		printf(" [NONE]\n");
	}
	else
	{
		if (m_Type->IsPrimitive())
			printf(" Primitive");

		if (m_Type->IsAbstract())
			printf(" Abstract");

		if (m_Type->IsGenericType())
			printf(" Generic");

		if (m_Type->IsEnum())
			printf(" Enum");

		printf("\n");
	}
}

void TypeViewer::DisplayParents()
{
	int index = 0;

	printf("Parents:");
	DisplayParents(index);

	if (index == 0)
		printf(" [NONE]\n");
}

void TypeViewer::DisplayParents(int& index, int nestingLevel)
{
	if (index == 0)
		printf("\n");

	if (index > 0)
	{
		PrintTabs(nestingLevel);
		printf("[%i] %s\n", index - 1, GetTypeName(m_Type));
		++nestingLevel;
	}

	++index;

	for (auto& info : m_Type->GetParentInfos())
	{
		if (info.type)
		{
			TypeViewer viewer(info.type);
			viewer.DisplayParents(index, nestingLevel);
		}
		else
		{
			PrintTabs(nestingLevel);
			printf("[%i] %s\n", index - 1, GetTypeName(info.type));
		}

		++index;
	}
}

void TypeViewer::DisplayFields()
{
	int index = 0;

	printf("Fields:");
	DisplayFields(index);

	if (index == 0)
		printf(" [NONE]\n");
}

void TypeViewer::DisplayFields(int& index)
{
	for (auto& info : m_Type->GetFields())
	{
		if (index == 0)
			printf("\n");

		PrintTabs(1);	printf("Field [%i]:\n", index);
		PrintTabs(2);	printf("Name: %s\n", info.GetName());

		const auto& fieldPrameterType = info.GetParameterType();
		const auto fieldType = fieldPrameterType.GetType();

		PrintTabs(2);	printf("Type: %s\n", GetTypeName(fieldType));
		PrintTabs(2);	printf("Is static: %s\n", BoolString[(int)info.IsStatic()]);
		PrintTabs(2);	printf("Flags:");

		if (fieldPrameterType.GetFlags() == false)
		{
			printf(" [NONE]");
		}
		else
		{
			if (fieldPrameterType.IsConst())
				printf(" Const");

			if (fieldPrameterType.IsPointer())
			{
				if (fieldPrameterType.IsPointerToPointer())
				{
					printf(" PointerToPointer");
				}
				else
				{
					printf(" Pointer");
				}
			}
		}

		printf("\n");
		DisplayMemberAttributes(&info, 2);

		++index;
	}

	for (auto& parent : m_Type->GetParentInfos())
	{
		if (parent.type == nullptr)
			continue;

		TypeViewer viewer(parent.type);
		viewer.DisplayFields(index);
	}
}

void TypeViewer::DisplayMethods()
{
	int index = 0;

	printf("Methods:");
	DisplayMethods(index);

	if (index == 0)
		printf(" [NONE]\n");
}

void TypeViewer::DisplayMethods(int& index)
{
	for (auto& info : m_Type->GetMethods())
	{
		if (index == 0)
			printf("\n");

		PrintTabs(1);	printf("Method [%i]:\n", index);
		PrintTabs(2); printf("Signature: ");

		if (info.IsStatic())
		{
			printf("static ");
		}
		else
		{
			if (info.IsVirtual())
			{
				if (info.IsPureVirtual())
				{
					printf("pure_virtual ");
				}
				else
				{
					printf("virtual ");
				}
			}
		}

		const auto parameters = info.GetParameterTypes();
		const size_t parameterCount = parameters.size();

		// TODO: doesn`t display return type decorators like *&

		DisplayParameterType(info.GetReturnType());
		printf(" %s::%s(", GetTypeName(info.GetObjectType()), info.GetName());

		for (size_t i = 0; i < parameters.size(); i++)
		{
			DisplayParameterType(parameters[i]);

			if (i < parameterCount - 1)
				printf(", ");
		}

		if (info.IsConst())
			printf(") const;\n");
		else
			printf(");\n");

		DisplayMemberAttributes(&info, 2);

		++index;
	}

	for (auto& parent : m_Type->GetParentInfos())
	{
		if (parent.type == nullptr)
			continue;

		TypeViewer viewer(parent.type);
		viewer.DisplayMethods(index);
	}
}

void TypeViewer::DisplayEnum()
{
	auto baseType = m_Type->GetEnumBaseType();

	printf("Base type: %s\n", baseType->GetName());
	printf("Values:\n");

	for (auto& value : m_Type->GetEnumValues())
	{
		PrintTabs(1);
		printf("%s = ", value.name);

		if (baseType == TypeOf<int8_t>::Get())			printf("%" PRId8, (int8_t)value.value);
		else if (baseType == TypeOf<uint8_t>::Get())	printf("%" PRIu8, (uint8_t)value.value);
		else if (baseType == TypeOf<int16_t>::Get())	printf("%" PRId16, (int16_t)value.value);
		else if (baseType == TypeOf<uint16_t>::Get())	printf("%" PRIu16, (uint16_t)value.value);
		else if (baseType == TypeOf<int32_t>::Get())	printf("%" PRId32, (int32_t)value.value);
		else if (baseType == TypeOf<uint32_t>::Get())	printf("%" PRIu32, (uint32_t)value.value);
		else if (baseType == TypeOf<int64_t>::Get())	printf("%" PRId64, (int64_t)value.value);
		else if (baseType == TypeOf<uint64_t>::Get())	printf("%" PRIu64, (uint64_t)value.value);

		printf("\n");
	}
}

void TypeViewer::DisplayConstructors()
{
	printf("Constructors:");

	auto lifetimeControl = m_Type->GetLifetimeControl();
	if (lifetimeControl == nullptr)
	{
		printf(" [NONE]");
	}
	else
	{
		printf("\n");

		int index = 0;

		for (auto& info : lifetimeControl->GetConstructors())
		{
			PrintTabs(1);	printf("Constructor [%i]:\n", index);
			PrintTabs(2); printf("Signature: ");

			const auto parameters = info.GetParameters();
			const size_t parameterCount = parameters.size();

			printf(" %s::%s(", m_Type->GetName(), m_Type->GetName());
			DisplayParametersAsFunctionArguments(parameters);
			printf(");\n");

			DisplayMemberAttributes(&info, 2);
		}
	}

	printf("\n");
}

void TypeViewer::DisplayMemberAttributes(const MemberInfoBase* info, int tabCount)
{
	PrintTabs(tabCount);
	printf("Attributes:");
	const auto& attributes = info->GetAttributes();

	if (attributes.empty())
	{
		printf(" [NONE]");
	}
	else
	{
		for (const auto attribute : attributes)
			printf(" %s", attribute->GetType()->GetName());
	}

	printf("\n");
}

void TypeViewer::DisplayParameterType(const Reflection::ParameterType& parameter)
{
	if (parameter.IsConst())
		printf("const ");

	printf("%s", GetTypeName(parameter.GetType()));

	if (parameter.IsPointer())
		printf("*");

	if (parameter.IsReference())
		printf("&");
}

void TypeViewer::DisplayParametersAsFunctionArguments(std::span<const ParameterType> parameters)
{
	size_t count = parameters.size();

	for (size_t i = 0; i < parameters.size(); i++)
	{
		DisplayParameterType(parameters[i]);

		if (i < count - 1)
			printf(", ");
	}
}

void TypeViewer::DisplayParents(const std::span<const Type::ParentInfo>& infos, int index, int tabCount)
{
	for (int i = 0; i < infos.size(); i++)
	{
		const auto type = infos[i].type;
		PrintTabs(tabCount);
		printf("[%i]: %s [Offset %i]\n", index + i, GetTypeName(type), infos[i].offset);
		++index;

		if (type != nullptr && type->HasParents())
			DisplayParents(type->GetParentInfos(), index + i, tabCount + 1);
	}
}