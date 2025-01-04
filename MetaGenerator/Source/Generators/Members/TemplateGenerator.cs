
namespace MetaGenerator
{
    internal class TemplateGenerator : SecondaryGenerator<ClassInfo>
    {
        public override void Run()
        {
            string wrapper = Context.HasNamespace ? "__GEN_REFLECTION_NAMESPACE_WRAPPER" : "__GEN_REFLECTION_NO_NAMESPACE_WRAPPER";
            string converter = Context.HasNamespace ? "__GEN_REFLECTION_NAMESPACE_CONVERTER" : "__GEN_REFLECTION_NULL_NAMESPACE_CONVERTER";
            string typeOf = Context.templateParameters + ' ' + Context.TypeOf;

            Builder.Append($"__GEN_REFLECTION_TEMPLATE_TYPE_FORWARD_DECLARATION_BEGIN(__GEN_ARG({typeOf}), {Context.name}, {wrapper}, {converter}, {Context.namespaceName})\n")
                .Append("\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_NO_PARENT_INFO\n");

            Launch<TemplateFieldInfoGenerator, ClassInfo>(Builder, Context);
            Launch<TemplateMethodInfoGenerator, ClassInfo>(Builder, Context);

            Builder.Append($"__GEN_REFLECTION_TEMPLATE_TYPE_FORWARD_DECLARATION_END\n");
        }
    }
}
