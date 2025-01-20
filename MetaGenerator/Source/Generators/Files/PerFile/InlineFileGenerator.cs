using System.IO;

namespace MetaGenerator
{
    public class InlineFileGenerator : PrimaryGenerator<ClassInfo>
    {
        public override void Run()
        {
            string virtualSpecifier = Context.HasVirtualGetType ? "__GEN_VIRTUAL_SPECIFIER" : "__GEN_EMPTY_SPECIFIER";

            Builder.Append(GENERATED_FILE_HEADER_TEXT)
                .Append($"#define __THIS_TYPE__ {Context.name}\n")
                .Append($"__GEN_REFLECTION_TYPE_INLINE_CORE({virtualSpecifier})\n");

            if (Context.NeedInlineMeta)
            {
                Builder.Append("__GEN_REFLECTION_TYPE_INLINE_META_BEGIN\n");

                for (int i = 0; i < Context.parentNames.Length; i++)
                    Builder.Append($"\tusing TParent{i}={Context.parentNames[i]};\n");

                for (int i = 0; i < Context.constructors.Length; i++)
                {
                    var argsDeclaration = Context.constructors[i].parameters;
                    Builder.Append($"\tstatic void c{i}{argsDeclaration};\n");
                }

                for (int i = 0; i < Context.methods.Length; i++)
                {
                    var declarationInfo = Context.methods[i].declarationInfo;
                    string ending = Context.methods[i].virtualType == MethodVirtualType.PureVirtual ? " = 0;" : ";";
                    Builder.Append($"\t{declarationInfo.prefix} m{i}{declarationInfo.parameters}{declarationInfo.suffix}{ending}\n");
                }

                Builder.Append($"__GEN_REFLECTION_TYPE_INLINE_META_END\n");
            }

            Builder.Append("#undef __THIS_TYPE__\n");

            string path = ProgramContext.GenerationDirectory + $"\\{Context.FullName.Replace("::", ".")}" + ".meta.inl";
            File.WriteAllText(path, Builder.ToString());
        }
    }
}
