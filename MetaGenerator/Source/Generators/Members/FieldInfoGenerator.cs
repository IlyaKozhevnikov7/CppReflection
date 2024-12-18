using System.Text;

namespace MetaGenerator
{
    internal class FieldInfoGenerator : Generator<ClassInfo>
    {
        public override void Generate(StringBuilder builder)
        {
            if (Context.HasFileds == false)
            {
                builder.AppendLine("\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_NO_FIELD_INFO");
                return;
            }

            builder.AppendLine("\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_FIELDS_BEGIN");

            foreach (var field in Context.fields)
            {
                if (field.HasAttributes)
                {
                    builder.AppendLine($"\t\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_FIELD_INFO_ATTRIBUTE_BEGIN");

                    foreach (var info in field.attributeInfos)
                        builder.AppendLine($"\t\t\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_FIELD_INFO_ATTRIBUTE(__GEN_TO_VALUE({info.name}), {info.args})");

                    builder.AppendLine($"\t\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_FIELD_INFO_ATTRIBUTE_END");
                }
                else
                {
                    builder.AppendLine($"\t\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_NO_ATTRIBUTES");
                }

                var flags = field.flags;
                string location = flags.HasFlag(FieldFlag.Static) ? "__GEN_FIELD_GLOBAL" : "__GEN_FIELD_LOCAL";
                builder.AppendLine($"\t\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_FIELD_INFO({field.name}, {location}, {(int)flags})");
            }

            builder.AppendLine("\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_FIELDS_END");
        }
    }
}
