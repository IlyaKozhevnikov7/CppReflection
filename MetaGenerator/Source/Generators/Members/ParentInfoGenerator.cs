
namespace MetaGenerator
{
    internal class ParentInfoGenerator : SecondaryGenerator<ClassInfo>
    {
        public override void Run()
        {
            if (Context.HasParents == false)
            {
                Builder.AppendLine("\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_NO_PARENT_INFO");
                return;
            }

            Builder.AppendLine("\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_PARENT_INFO_BEGIN");

            for (int i = 0; i < Context.parentNames.Length; i++)
                Builder.AppendLine($"\t\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_PARENT_INFO({i})");

            Builder.AppendLine("\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_PARENT_INFO_END");
        }
    }
}
