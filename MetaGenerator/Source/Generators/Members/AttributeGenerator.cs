
namespace MetaGenerator
{
    internal class AttributeGenerator : SecondaryGenerator<MemberInfo>
    {
        public override void Run()
        {
            if (Context.HasAttributes == false)
            {
                Builder.AppendLine($"\t\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_NO_ATTRIBUTES");
                return;
            }

            Builder.AppendLine($"\t\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_ATTRIBUTES_BEGIN");

            foreach (var info in Context.attributeInfos)
                Builder.AppendLine($"\t\t\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_ATTRIBUTE(__GEN_ARG({info.name}), {info.args})");

            Builder.AppendLine($"\t\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_ATTRIBUTE_END");
        }
    }
}
