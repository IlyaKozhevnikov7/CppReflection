using System.IO;
using System.Linq;
using System.Text;

namespace MetaGenerator
{
    public class CommonIncludeGenerator : PrimaryGenerator
    {
        public override void Run()
        {
            Builder.Append(GENERATED_FILE_HEADER_TEXT)
                .Append("#pragma once\n\n");

            if (ProgramContext.HasDllExportMacro)
            {
                Builder.Append($"#ifdef {ProgramContext.DllExportMacro}\n")
                    .Append($"\t#define {ProgramContext.InternalDllExportMacro} __declspec(dllexport)\n")
                    .Append("#else\n")
                    .Append($"\t#define {ProgramContext.InternalDllExportMacro} __declspec(dllimport)\n")
                    .Append($"#endif // {ProgramContext.DllExportMacro}\n\n");
            }

            Builder.Append("#include \"Reflection/Type.h\"\n\n");

            AddClassForwardDeclarations();
            AddEnumForwardDeclarations();
            AddTemplateForwardDeclarations();

            string path = ProgramContext.GenerationDirectory + $"\\{ProgramContext.ProjectName}ReflectionInclude.meta.h";
            File.WriteAllText(path, Builder.ToString());
        }

        private void AddClassForwardDeclarations()
        {
            foreach (var classInfo in ProjectData.ClassInfos.Where(c => c.IsTemplate == false))
            {
                string wrapper = classInfo.HasNamespace ? "__GEN_REFLECTION_NAMESPACE_WRAPPER" : "__GEN_REFLECTION_NO_NAMESPACE_WRAPPER";
                string typeOf = null;

                switch (classInfo.instanceOf)
                {
                    case InstanceOf.Class: typeOf = "class"; break;
                    case InstanceOf.Struct: typeOf = "struct"; break;
                }

                Builder.Append($"__GEN_REFLECTION_TYPE_FORWARD_DECLARATION({ProgramContext.InternalDllExportMacro}, {typeOf}, {classInfo.name}, {wrapper}, {classInfo.namespaceName})\n");
            }
        }

        private void AddEnumForwardDeclarations()
        {
            foreach (var enumInfo in ProjectData.Enums)
            {
                string wrapper = enumInfo.HasNamespace ? $"__GEN_REFLECTION_NAMESPACE_ENUM_WRAPPER" : "__GEN_REFLECTION_NO_NAMESPACE_WRAPPER";
                Builder.Append($"__GEN_REFLECTION_ENUM_TYPE_FORWARD_DECLARATION({ProgramContext.InternalDllExportMacro}, {enumInfo.name}, {wrapper}, {enumInfo.forwardDeclaration}, {enumInfo.namespaceName})\n");
            }
        }

        private void AddTemplateForwardDeclarations()
        {
            foreach (var classInfo in ProjectData.ClassInfos.Where(c => c.IsTemplate))
            {
                string classFullName = classInfo.FullName.Replace("::", "_");
                int memberId = 0;

                Builder.Append("\nnamespace Reflection::Generation\n{\n")
                    .Append($"\t#define __CURRENT_TYPE__ {classInfo.FullName}<TArgs...>\n")
                    .Append($"\t#define __GEN_REFLECTION_TEMPLATE_MEMBER_{memberId++} {classFullName}\n");

                for (int i = 0; i < classInfo.fields.Length; i++)
                    AddendTemplateMemberName(Builder, memberId++, classInfo.fields[i].name);

                for (int i = 0; i < classInfo.methods.Length; i++)
                    AddendTemplateMemberName(Builder, memberId++, classInfo.methods[i].name);

                for (int i = 0; i < memberId; i++)
                    Builder.Append($"\t__GEN_REFLECTION_TEMPLATE_MEMBER_NAME_FUNCTION({ProgramContext.InternalDllExportMacro}, {i})\n");

                Builder.Append("}\n");

                string wrapper = classInfo.HasNamespace ? "__GEN_REFLECTION_NAMESPACE_WRAPPER" : "__GEN_REFLECTION_NO_NAMESPACE_WRAPPER";
                string typeOf = classInfo.templateParameters + ' ' + classInfo.TypeOf;

                Builder.Append($"{wrapper}(__GEN_ARG({typeOf}), {classInfo.name}, {classInfo.namespaceName});\n");

                Launch<ProxyMethodGenerator, ClassInfo>(Builder, classInfo);
                Launch<TemplateGenerator, ClassInfo>(Builder, classInfo);

                for (int i = 0; i < memberId; i++)
                    Builder.Append($"#undef __GEN_REFLECTION_TEMPLATE_MEMBER_{i}\n");

                Builder.Append($"#undef __CURRENT_TYPE__\n");
            }
        }

        private static void AddendTemplateMemberName(StringBuilder builder, int memberId, string name)
        {
            builder.Append($"\t#define __GEN_REFLECTION_TEMPLATE_MEMBER_{memberId} {name}\n");
        }
    }
}
