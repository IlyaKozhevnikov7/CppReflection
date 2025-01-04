using System.IO;
using System.Linq;

namespace MetaGenerator
{
    public class AssemblyFileGenerator : PrimaryGenerator
    {
        public override void Run()
        {
            Builder.Append(GENERATED_FILE_HEADER_TEXT)
                .Append($"#include \"Reflection/Assembly.h\"\n#include \"Generation/{ProgramContext.ProjectName}ReflectionInclude.meta.h\"\n\nusing namespace Reflection;\n")
                .Append($"\nstatic Assembly assembly(\"{ProgramContext.ProjectName}\",\n\t{{\n");

            foreach (var classInfo in ProjectData.ClassInfos.Where(i => i.IsTemplate == false))
                Builder.Append($"\t\tTypeOf<{classInfo.FullName}>::Get(),\n");

            foreach (var enumInfo in ProjectData.Enums)
                Builder.Append($"\t\tTypeOf<{enumInfo.FullName}>::Get(),\n");

            Builder.Append("\t});\n");

            if (ProgramContext.HasDllExportMacro)
                Builder.Append($"\nextern \"C\"\n{{\n\t{ProgramContext.InternalDllExportMacro} const Assembly* __GEN_REFLECTION_GET_ASSEMBLY()\n\t{{\n\t\treturn &assembly;\n\t}}\n}}\n");

            string path = ProgramContext.GenerationDirectory + $"\\{ProgramContext.ProjectName}Assembly.meta.cpp";
            File.WriteAllText(path, Builder.ToString());
        }
    }
}
