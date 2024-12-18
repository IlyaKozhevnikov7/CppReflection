using System;

namespace MetaGenerator
{
    public enum MethodVirtualType : byte
    {
        NotVirtual  = 0,
        Virtual     = 1 << 2,
        PureVirtual = (1 << 3) | Virtual
    }

    public class MethodInfo : MemberInfo
    {
        public struct DeclarationInfo
        {
            public string parameters;
            public string prefix;
            public string suffix;
        }

        public MethodVirtualType virtualType;
        public DeclarationInfo declarationInfo;
    }
}
