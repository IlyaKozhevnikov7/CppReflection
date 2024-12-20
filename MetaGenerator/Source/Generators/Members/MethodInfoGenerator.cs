﻿using System.Text;

namespace MetaGenerator
{
    internal class MethodInfoGenerator : Generator<ClassInfo>
    {
        public override void Generate(StringBuilder builder)
        {
            if (Context.HasMethods == false)
            {
                builder.AppendLine("\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_NO_METHOD_INFO");
                return;
            }

            builder.AppendLine("\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_METHOD_INFO_BEGIN");

            for (int i = 0; i < Context.methods.Length; i++)
            {
                var info = Context.methods[i];

                Generator.Launch<AttributeGenerator, MemberInfo>(builder, info);
                builder.AppendLine($"\t\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_METHOD_INFO({info.name}, {i}, {(int)info.virtualType})");
            }

            builder.AppendLine("\t\t__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_METHOD_INFOS_END");
        }
    }
}
