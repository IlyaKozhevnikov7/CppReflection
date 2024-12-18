
namespace MetaGenerator
{
    public class MemberInfo
    {
        public string name;
        public AttributeInfo[] attributeInfos;

        public bool HasAttributes => attributeInfos != null;

        public static T[] Construct<T>(int count) where T : MemberInfo, new()
        {
            var infos = new T[count];

            for (int i = 0; i < count; i++)
                infos[i] = new T();

            return infos;
        }
    }
}
