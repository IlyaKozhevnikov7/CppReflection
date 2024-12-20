using System.Text;

namespace MetaGenerator
{
    internal class AttributeGenerator : Generator<MemberInfo>
    {
        public override void Generate(StringBuilder builder)
        {
            if (Context.HasAttributes == false)
            {
                builder.AppendLine($"\t\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_NO_ATTRIBUTES");
                return;
            }

            builder.AppendLine($"\t\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_ATTRIBUTES_BEGIN");

            foreach (var info in Context.attributeInfos)
                builder.AppendLine($"\t\t\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_ATTRIBUTE(__GEN_ARG({info.name}), {info.args})");

            builder.AppendLine($"\t\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_ATTRIBUTE_END");
        }
    }
}
