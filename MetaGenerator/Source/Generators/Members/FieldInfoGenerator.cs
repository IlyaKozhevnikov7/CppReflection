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
                Generator.Launch<AttributeGenerator, MemberInfo>(builder, field);
                builder.AppendLine($"\t\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_FIELD_INFO({field.name})");
            }

            builder.AppendLine("\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_FIELDS_END");
        }
    }
}
