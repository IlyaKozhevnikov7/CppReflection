#pragma once

#include "AssemblyViewerLibCore.h"
#include "Reflection/ParameterType.h"

REFLECTABLE()
class TypeViewer
{
	#include "Generation/TypeViewer.meta.inl"

private:

	const Reflection::TypePtr m_Type;

public:

	CONSTRUCTOR()
	TypeViewer(const Reflection::TypePtr type) :
		m_Type(type)
	{
	}

	METHOD()
	void Display();

private:

	void DisplayHeader();

	void DisplayParents();

	void DisplayParents(int& index, int nestingLevel = 1);

	void DisplayFields();

	void DisplayFields(int& index);

	void DisplayMethods();

	void DisplayMethods(int& index);

	void DisplayEnum();

	void DisplayConstructors();

	static void DisplayMemberAttributes(const Reflection::MemberInfoBase* info, int tabCount = 0);

	static void DisplayParameterType(const Reflection::ParameterType& parameter);

	static void DisplayParametersAsFunctionArguments(std::span<const Reflection::ParameterType> parameters);

	static void DisplayParents(const std::span<const Reflection::Type::ParentInfo>& infos, int index = 0, int tabCount = 0);
};