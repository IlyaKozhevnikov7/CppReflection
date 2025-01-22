
namespace MetaGenerator
{
    internal class TemplateGenerator : SecondaryGenerator<ClassInfo>
    {
        public override void Run()
        {
            string converter = Context.HasNamespace ? "__GEN_REFLECTION_NAMESPACE_CONVERTER" : "__GEN_REFLECTION_NULL_NAMESPACE_CONVERTER";

            Builder.Append($"__GEN_REFLECTION_TEMPLATE_TYPE_FORWARD_DECLARATION_BEGIN({Context.name}, {converter}, {Context.namespaceName})\n");

            Launch<AttributeGenerator, MemberInfoBase>(Builder, Context);
            Launch<ParentInfoGenerator, ClassInfo>(Builder, Context);
            Launch<TemplateFieldInfoGenerator, ClassInfo>(Builder, Context);
            Launch<TemplateMethodInfoGenerator, ClassInfo>(Builder, Context);
            Launch<ConstructorInfoGenerator, ClassInfo>(Builder, Context);

            Builder.Append($"__GEN_REFLECTION_TEMPLATE_TYPE_FORWARD_DECLARATION_END\n");
        }
    }
}
