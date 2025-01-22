
namespace MetaGenerator
{
    public class ConstructorInfoGenerator : SecondaryGenerator<ClassInfo>
    {
        public override void Run()
        {
            if (Context.HasConstructors == false)
            {
                Builder.AppendLine("\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_NO_CONSTRUCTOR_INFO");
                return;                
            }

            Builder.AppendLine("\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_CONSTRUCTORS_BEGIN");

            for (int i = 0; i < Context.constructors.Length; i++)
            {
                var info = Context.constructors[i];

                Launch<AttributeGenerator, MemberInfoBase>(Builder, info);
                Builder.AppendLine($"\t\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_CONSTRUCTOR_INFO({i})");
            }

            Builder.AppendLine("\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_CONSTRUCTORS_END");
        }
    }
}
