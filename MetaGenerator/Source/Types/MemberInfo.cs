
namespace MetaGenerator
{
    public class MemberInfoBase
    {
        public AttributeInfo[] attributeInfos;

        public bool HasAttributes => attributeInfos != null;
        
        public static T[] Construct<T>(int count) where T : MemberInfoBase, new()
        {
            var infos = new T[count];

            for (int i = 0; i < count; i++)
                infos[i] = new T();

            return infos;
        }
    }

    public class MemberInfo : MemberInfoBase
    {
        public string name;
    }
}
