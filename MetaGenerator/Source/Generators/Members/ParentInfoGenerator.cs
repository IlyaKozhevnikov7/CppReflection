using System.Text;

namespace MetaGenerator
{
    internal class ParentInfoGenerator : Generator<ClassInfo>
    {
        public override void Generate(StringBuilder builder)
        {
            if (Context.HasParents == false)
            {
                builder.AppendLine("\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_NO_PARENT_INFO");
                return;
            }

            builder.AppendLine("\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_PARENT_INFO_BEGIN");

            foreach (var parentName in Context.parentNames)
                builder.AppendLine($"\t\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_PARENT_INFO({parentName})");

            builder.AppendLine("\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_PARENT_INFO_END");
        }
    }
}
