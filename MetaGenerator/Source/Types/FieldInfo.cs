using System;

namespace MetaGenerator
{
    [Flags]
    public enum FieldFlag : byte
    {
        None = 0,
        Static = 1
    }

    public class FieldInfo : MemberInfo
    {
        public string typeName;
        public FieldFlag flags;
    };
}
