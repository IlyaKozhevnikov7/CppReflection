
using System;
using System.Collections.Generic;

namespace MetaGenerator
{
    public struct FileInfo
    {
        public readonly string path;
        public readonly ClassInfo[] classInfos;
        public readonly EnumInfo[] enumInfos;

        public FileInfo(string path, ClassInfo[] classInfos, EnumInfo[] enumInfos)
        {
            this.path = path;
            this.classInfos = classInfos;
            this.enumInfos = enumInfos;
        }
    };
}
