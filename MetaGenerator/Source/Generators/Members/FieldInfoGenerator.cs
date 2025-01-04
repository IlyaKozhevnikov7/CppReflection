
namespace MetaGenerator
{
    public abstract class FieldInfoGeneratorBase : SecondaryGenerator<ClassInfo>
    {
        public override void Run()
        {
            if (Context.HasFileds == false)
            {
                Builder.AppendLine("\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_NO_FIELD_INFO");
                return;
            }

            Builder.AppendLine("\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_FIELDS_BEGIN");
            GenerateFields();
            Builder.AppendLine("\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_FIELDS_END");
        }

        protected abstract void GenerateFields();
    }

    public class FieldInfoGenerator : FieldInfoGeneratorBase
    {
        protected override void GenerateFields()
        {
            foreach (var field in Context.fields)
            {
                Launch<AttributeGenerator, MemberInfo>(Builder, field);
                Builder.AppendLine($"\t\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_FIELD_INFO({field.name})");
            }
        }
    }

    public class TemplateFieldInfoGenerator : FieldInfoGeneratorBase
    {
        protected override void GenerateFields()
        {
            for (int i = 0; i < Context.fields.Length; i++)
            {
                Launch<AttributeGenerator, MemberInfo>(Builder, Context.fields[i]);
                Builder.AppendLine($"\t\t\t__GEN_REFLECTION_TEMPLATE_GET_TYPE_IMPLEMENTATION_FIELD_INFO({i + 1})");
            }
        }
    }
}
