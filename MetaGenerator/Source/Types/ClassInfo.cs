using System;

namespace MetaGenerator
{
    [Flags]
    public enum TypeAttribute : byte
    {
        None = 0,
        Abstract = 1,
        Enum = 1 << 2
    }

    public enum InstanceOf : byte
    {
        Class = 0,
        Struct,
        Enum
    }

    public class TypeMemberInfo : MemberInfo
    {
        public string namespaceName;
        public TypeAttribute attributes;
        public InstanceOf instanceOf;

        public bool HasNamespace => namespaceName != null;
        public string FullName => HasNamespace ? $"{namespaceName}::{name}" : name;
    }

    public class ClassInfo : TypeMemberInfo
    {
        public bool isFinal;
        public string[] parentNames;
        public FieldInfo[] fields;
        public MethodInfo[] methods;
        public string templateParameters;

        public bool HasParents => parentNames.Length > 0;
        public bool HasFileds => fields.Length > 0;
        public bool HasMethods => methods.Length > 0;
        public bool HasVirtualGetType => isFinal == false || HasParents;
        public bool IsTemplate => templateParameters != null;
        public string TypeOf => instanceOf == InstanceOf.Class ? "class" : "struct";
        public bool NeedInlineMeta => HasParents || HasMethods;
    };
}
