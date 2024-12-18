using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace MetaGenerator
{
    public class CodeGenerator
    {
        private const string GENERATED_FILE_HEADER_TEXT = @"/*============================================================
        This file was created using MetaGenerator
    !!!!! DO NOT MAKE ANY CHANGES TO THIS FILE !!!!!
============================================================*/
";

        private static List<ClassInfo> _classes = [];
        private static List<EnumInfo> _enums = [];

        private static string InternalDllExportMacro = null;

        private FileInfo _fileInfo;
        private string FileName => _fileInfo.path;
        private bool HasClassInfos => _fileInfo.classInfos != null;
        private bool HasEnumInfos => _fileInfo.enumInfos != null;
        private IEnumerable<ClassInfo> ClassInfos => _fileInfo.classInfos;
        private IEnumerable<EnumInfo> EnumInfos => _fileInfo.enumInfos;

        public CodeGenerator(in FileInfo fileInfo)
        {
            _fileInfo = fileInfo;

            if (HasClassInfos)
                _classes.AddRange(ClassInfos);

            if (HasEnumInfos)
                _enums.AddRange(EnumInfos);
        }

        public static void GenerateProjectCommonInclude()
        {
            StringBuilder builder = new(GENERATED_FILE_HEADER_TEXT);
            builder.Append("#pragma once\n\n");

            InternalDllExportMacro = ProgramContext.HasDllExportMacro == false
                ? ProgramContext.DllExportMacro
                : $"__REFLECTION_{ProgramContext.DllExportMacro}_API__";

            if (ProgramContext.HasDllExportMacro)
            {
                builder.Append($"#ifdef {ProgramContext.DllExportMacro}\n")
                    .Append($"\t#define {InternalDllExportMacro} __declspec(dllexport)\n")
                    .Append("#else\n")
                    .Append($"\t#define {InternalDllExportMacro} __declspec(dllimport)\n")
                    .Append($"#endif // {ProgramContext.DllExportMacro}\n\n");
            }

            builder.Append("#include \"Reflection/Type.h\"\n\n");

            foreach (var classInfo in _classes)
            {
                string wrapper = classInfo.HasNamespace ? "__GEN_REFLECTION_NAMESPACE_WRAPPER" : "__GEN_REFLECTION_NO_NAMESPACE_WRAPPER";
                string typeOf = null;

                switch (classInfo.instanceOf)
                {
                    case InstanceOf.Class: typeOf = "class"; break;
                    case InstanceOf.Struct: typeOf = "struct"; break;
                }

                if (classInfo.IsTemplate)
                {
                    Generator.Launch<TemplateGenerator, ClassInfo>(builder, classInfo);
                }
                else
                {
                    builder.Append($"__GEN_REFLECTION_TYPE_FORWARD_DECLARATION({InternalDllExportMacro}, {typeOf}, {classInfo.name}, {wrapper}, {classInfo.namespaceName})\n");
                }
            }

            foreach (var enumInfo in _enums)
            {
                string wrapper = enumInfo.HasNamespace ? $"__GEN_REFLECTION_NAMESPACE_ENUM_WRAPPER" : "__GEN_REFLECTION_NO_NAMESPACE_WRAPPER";
                builder.Append($"__GEN_REFLECTION_ENUM_TYPE_FORWARD_DECLARATION({InternalDllExportMacro}, {enumInfo.name}, {wrapper}, {enumInfo.forwardDeclaration}, {enumInfo.namespaceName})\n");
            }

            string path = ProgramContext.GenerationDirectory + $"\\{ProgramContext.ProjectName}ReflectionInclude.meta.h";
            File.WriteAllText(path, builder.ToString());
        }

        public static void GenerateProjectAssemblyFile()
        {
            StringBuilder builder = new(GENERATED_FILE_HEADER_TEXT);

            builder.Append($"#include \"Reflection/Assembly.h\"\n#include \"Generation/{ProgramContext.ProjectName}ReflectionInclude.meta.h\"\n\nusing namespace Reflection;\n")
                .Append($"\nstatic Assembly assembly(\"{ProgramContext.ProjectName}\",\n\t{{\n");

            foreach (var classInfo in _classes)
                if (classInfo.IsTemplate == false)
                    builder.Append($"\t\tTypeOf<{classInfo.FullName}>::Get(),\n");

            foreach (var enumInfo in _enums)
                builder.Append($"\t\tTypeOf<{enumInfo.FullName}>::Get(),\n");

            builder.Append("\t});\n");

            if (ProgramContext.HasDllExportMacro)
                builder.Append($"\nextern \"C\"\n{{\n\t{InternalDllExportMacro} const Assembly* __GEN_REFLECTION_GET_ASSEMBLY()\n\t{{\n\t\treturn &assembly;\n\t}}\n}}\n");

            string path = ProgramContext.GenerationDirectory + $"\\{ProgramContext.ProjectName}Assembly.meta.cpp";
            File.WriteAllText(path, builder.ToString());
        }

        public void GenerateForFile()
        {
            GenerateInlineFiles();
            GenerateImplementationFile();

            Console.WriteLine($"Generated for {FileName}");
        }

        private void GenerateInlineFiles()
        {
            foreach (var classInfo in ClassInfos)
                GenerateInlineFor(in classInfo);
        }

        private void GenerateImplementationFile()
        {
            StringBuilder builder = new(GENERATED_FILE_HEADER_TEXT);

            builder.Append($"#include \"{_fileInfo.path}\"\n")
                .Append("#include \"Reflection/Type.h\"\n\n")
                .Append("namespace Reflection\n{");

            GenerateClassImplementations(builder);
            GenerateEnumImplementations(builder);

            builder.Append("}\n");

            string path = Path.Combine(ProgramContext.GenerationDirectory, Path.GetFileNameWithoutExtension(_fileInfo.path) + ".meta.cpp");
            File.WriteAllText(path, builder.ToString());
        }

        private void GenerateClassImplementations(StringBuilder builder)
        {
            if (HasClassInfos == false)
                return;

            foreach (var classInfo in ClassInfos)
            {
                if (classInfo.IsTemplate)
                    continue;

                string namespaceValue = classInfo.HasNamespace ? $"\"{classInfo.namespaceName}\"" : "nullptr";

                builder.Append($"\n#undef __CURRENT_TYPE__\n#define __CURRENT_TYPE__ {classInfo.FullName}\n")
                    .Append($"\n#undef __CURRENT_TYPE_NAME_STRING__\n#define __CURRENT_TYPE_NAME_STRING__ {classInfo.FullName.Replace("::", "_")}\n")
                    .Append($"\n\t__GEN_REFLECTION_GET_NAME_IMPLEMENTATION({classInfo.name});\n\n")
                    .Append($"\t__GEN_REFLECTION_CREATE_INSTANCE_FUNCTION;\n\n")
                    .Append($"\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_BEGIN({namespaceValue})\n");

                Generator.Launch<ParentInfoGenerator, ClassInfo>(builder, classInfo);
                Generator.Launch<FieldInfoGenerator, ClassInfo>(builder, classInfo);
                Generator.Launch<MethodInfoGenerator, ClassInfo>(builder, classInfo);

                builder.Append("\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_END\n");
            }
        }

        private void GenerateEnumImplementations(StringBuilder builder)
        {
            if (HasEnumInfos == false)
                return;

            foreach (var info in EnumInfos)
            {
                string namespaceValue = info.HasNamespace ? $"\"{info.namespaceName}\"" : "nullptr";

                builder.Append($"\n#undef __CURRENT_TYPE__\n#define __CURRENT_TYPE__ {info.FullName}\n")
                   .Append($"\n\t__GEN_REFLECTION_GET_NAME_IMPLEMENTATION({info.name});\n")
                   .Append($"\n\t__GEN_REFLECTION_GET_ENUM_TYPE_IMPLEMENTATION({namespaceValue});\n")
                   .Append($"\n\t__GEN_REFLECTION_ENUM_VALUES_IMPLEMENTATION({info.valueNames.Length},");

                foreach (var value in info.valueNames)
                    builder.Append($"\n\t\t__GEN_REFLECTION_ENUM_VALUE({value}),");

                builder.Append(");\n");
            }
        }

        private void GenerateInlineFor(in ClassInfo info)
        {
            StringBuilder builder = new();

            builder.Append($"#define __THIS_TYPE__ {info.name}\n");

            string virtualModificator = info.HasVirtualGetType ? "__GEN_VIRTUAL_MODIFICATION" : "__GEN_EMPTY_MODIFICATION";
            builder.Append($"__GEN_REFLECTION_TYPE_INLINE_CORE({virtualModificator})\n");

            if (info.methods != null)
            {
                builder.Append("__GEN_REFLECTION_TYPE_INLINE_METHOD_SIGNATURES_BEGIN\n");

                for (int i = 0; i < info.methods.Length; i++)
                {
                    var declarationInfo = info.methods[i].declarationInfo;
                    string ending = info.methods[i].virtualType == MethodVirtualType.PureVirtual ? " = 0;" : ";";
                    builder.Append($"\t{declarationInfo.prefix} _{i}{declarationInfo.parameters} {declarationInfo.suffix}{ending}\n");
                }

                builder.Append($"__GEN_REFLECTION_TYPE_INLINE_METHOD_SIGNATURES_END\n");
            }

            builder.Append("#undef __THIS_TYPE__\n");

            string path = ProgramContext.GenerationDirectory + $"\\{info.FullName.Replace("::", ".")}" + ".meta.inl";
            File.WriteAllText(path, builder.ToString());
        }
    }
}
