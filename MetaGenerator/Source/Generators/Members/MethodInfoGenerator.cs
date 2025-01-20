
namespace MetaGenerator
{
    public abstract class MethodInfoGeneratorBase : SecondaryGenerator<ClassInfo>
    {
        public override void Run()
        {
            if (Context.HasMethods == false)
            {
                Builder.AppendLine("\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_NO_METHOD_INFO");
                return;
            }

            Builder.AppendLine("\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_METHOD_INFO_BEGIN");
            GenerateMethods();
            Builder.AppendLine("\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_METHOD_INFOS_END");
        }

        protected abstract void GenerateMethods();
    }

    public class MethodInfoGenerator : MethodInfoGeneratorBase
    {
        protected override void GenerateMethods()
        {
            for (int i = 0; i < Context.methods.Length; i++)
            {
                var info = Context.methods[i];

                Launch<AttributeGenerator, MemberInfo>(Builder, info);
                Builder.AppendLine($"\t\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_METHOD_INFO({info.name}, {i}, {(int)info.virtualType})");
            }
        }
    }

    public class TemplateMethodInfoGenerator : MethodInfoGeneratorBase
    {
        protected override void GenerateMethods()
        {
            int memberIdOffset = Context.fields.Length + 1;

            for (int i = 0; i < Context.methods.Length; i++)
            {
                var info = Context.methods[i];

                Launch<AttributeGenerator, MemberInfo>(Builder, info);
                Builder.AppendLine($"\t\t\t__GEN_REFLECTION_TEMPLATE_GET_TYPE_IMPLEMENTATION_METHOD_INFO({i + memberIdOffset}, {i}, {(int)info.virtualType})");
            }
        }
    }

    public class ProxyMethodGenerator : SecondaryGenerator<ClassInfo>
    {
        public override void Run()
        {
            if (Context.HasMethods == false)
                return;

            string begin = Context.IsTemplate ? "__GEN_TEMPLATE_PROXY_INVOKER_BEGIN" : "__GEN_PROXY_INVOKER_BEGIN";
            Builder.AppendLine($"\t{begin}");

            for (int i = 0; i < Context.methods.Length; i++)
            {
                var info = Context.methods[i];
                Builder.AppendLine($"\t\t__GEN_PROXY_METHOD({info.name}, {i})");
            }

            Builder.AppendLine("\t__GEN_PROXY_INVOKER_END\n");
        }
    }
}
