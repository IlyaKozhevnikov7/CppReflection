using System.Text;

namespace MetaGenerator
{
    internal class TemplateGenerator : Generator<ClassInfo>
    {
        public override void Generate(StringBuilder builder)
        {
            string wrapper = Context.HasNamespace ? "__GEN_REFLECTION_NAMESPACE_WRAPPER" : "__GEN_REFLECTION_NO_NAMESPACE_WRAPPER";
            string typeOf = Context.templateParameters + ' ' + Context.TypeOf;

            builder.Append($"__GEN_REFLECTION_TEMPLATE_TYPE_FORWARD_DECLARATION_BEGIN(__GEN_ARG({typeOf}), {Context.name}, {wrapper}, {Context.namespaceName})\n")
                .Append("\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_NO_PARENT_INFO\n");

            Generator.Launch<FieldInfoGenerator, ClassInfo>(builder, Context);
            Generator.Launch<MethodInfoGenerator, ClassInfo>(builder, Context);

            builder.Append($"__GEN_REFLECTION_TEMPLATE_TYPE_FORWARD_DECLARATION_END\n\n");
        }
    }
}
