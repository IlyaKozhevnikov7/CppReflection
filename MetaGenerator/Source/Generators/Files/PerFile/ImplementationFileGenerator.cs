using System.IO;
using System.Linq;
using System.Text;

namespace MetaGenerator
{
    public class ImplementationFileGenerator : PrimaryGenerator<FileInfo>
    {
        public override void Run()
        {
            Builder.Append(GENERATED_FILE_HEADER_TEXT)
                .Append($"#include \"{Context.path}\"\n")
                .Append("#include \"Reflection/Type.h\"\n\n")
                .Append("namespace Reflection\n{");

            GenerateEnumImplementations();
            GenerateClassImplementations();
            GenerateTemplateImplementations();

            Builder.Append("}\n");

            string path = Path.Combine(ProgramContext.GenerationDirectory, Path.GetFileNameWithoutExtension(Context.path) + ".meta.cpp");
            File.WriteAllText(path, Builder.ToString());
        }

        private void GenerateEnumImplementations()
        {
            foreach (var info in Context.enumInfos)
            {
                string namespaceValue = info.HasNamespace ? $"\"{info.namespaceName}\"" : "nullptr";

                Builder.Append($"\n#undef __CURRENT_TYPE__\n#define __CURRENT_TYPE__ {info.FullName}\n")
                   .Append($"\n\t__GEN_REFLECTION_GET_NAME_IMPLEMENTATION({info.name});\n")
                   .Append($"\n\t__GEN_REFLECTION_GET_ENUM_TYPE_IMPLEMENTATION({namespaceValue});\n")
                   .Append($"\n\t__GEN_REFLECTION_ENUM_VALUES_IMPLEMENTATION({info.valueNames.Length},");

                foreach (var value in info.valueNames)
                    Builder.Append($"\n\t\t__GEN_REFLECTION_ENUM_VALUE({value}),");

                Builder.Append(");\n");
            }
        }

        private void GenerateClassImplementations()
        {
            foreach (var classInfo in Context.classInfos.Where(i => i.IsTemplate == false))
            {
                string namespaceValue = classInfo.HasNamespace ? $"\"{classInfo.namespaceName}\"" : "nullptr";

                Builder.Append($"\n#undef __CURRENT_TYPE__\n#define __CURRENT_TYPE__ {classInfo.FullName}\n")
                    .Append($"\n#undef __CURRENT_TYPE_NAME_STRING__\n#define __CURRENT_TYPE_NAME_STRING__ {classInfo.FullName.Replace("::", "_")}\n")
                    .Append($"\n\t__GEN_REFLECTION_GET_NAME_IMPLEMENTATION({classInfo.name});\n\n")
                    .Append($"\t__GEN_REFLECTION_CREATE_INSTANCE_FUNCTION;\n\n")
                    .Append($"\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_BEGIN({namespaceValue})\n");

                Launch<ParentInfoGenerator, ClassInfo>(Builder, classInfo);
                Launch<FieldInfoGenerator, ClassInfo>(Builder, classInfo);
                Launch<MethodInfoGenerator, ClassInfo>(Builder, classInfo);

                Builder.Append("\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_END\n");
            }
        }

        private void GenerateTemplateImplementations()
        {
            Builder.Append("\n\tnamespace Generation\n\t{\n");

            foreach (var classInfo in Context.classInfos.Where(i => i.IsTemplate))
            {
                string classFullName = classInfo.FullName.Replace("::", "_");

                int memberId = 0;
                Builder.Append($"\t\tconst char* GetTemplateMemberName_{classFullName}_{memberId++}() {{ return\"{classInfo.name}\"; }}\n");

                if (classInfo.HasFileds)
                {
                    for (int i = 0; i < classInfo.fields.Length; i++)
                        AddendTemplateMemberNameFunction(Builder, classFullName, memberId++, classInfo.fields[i].name);
                }

                if (classInfo.HasMethods)
                {
                    for (int i = 0; i < classInfo.methods.Length; i++)
                    {
                        AddendTemplateMemberNameFunction(Builder, classFullName, memberId++, classInfo.methods[i].name);
                    }
                }
            }

            Builder.Append("\t}\n");
        }

        private static void AddendTemplateMemberNameFunction(StringBuilder builder, string className, int memberId, string name)
        {
            builder.Append($"\t\tconst char* GetTemplateMemberName_{className}_{memberId}() {{ return\"{name}\"; }}\n");
        }
    }
}
