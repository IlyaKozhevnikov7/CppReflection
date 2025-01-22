
namespace MetaGenerator
{
    internal class AttributeGenerator : SecondaryGenerator<MemberInfoBase>
    {
        public override void Run()
        {
            string prefix = Context is ClassInfo ? "_T" : "";

            if (Context.HasAttributes == false)
            {
                Builder.AppendLine($"\t\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_NO_ATTRIBUTES{prefix}");
                return;
            }

            Builder.AppendLine($"\t\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_ATTRIBUTES_BEGIN{prefix}");

            foreach (var info in Context.attributeInfos)
                Builder.AppendLine($"\t\t\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_ATTRIBUTE(__GEN_ARG({info.name}), {info.args})");

            Builder.AppendLine($"\t\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_ATTRIBUTE_END{prefix}");
        }
    }
}
