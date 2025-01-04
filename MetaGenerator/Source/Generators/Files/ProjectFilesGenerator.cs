using System.Collections.Generic;

namespace MetaGenerator
{
    public static class ProjectData
    {
        private static List<ClassInfo> _classes = [];
        private static List<EnumInfo> _enums = [];

        public static IEnumerable<ClassInfo> ClassInfos => _classes;
        public static IEnumerable<EnumInfo> Enums => _enums;

        public static void RegisterClasses(IEnumerable<ClassInfo> infos) => _classes.AddRange(infos);

        public static void RegisterEnums(IEnumerable<EnumInfo> infos) => _enums.AddRange(infos);
    }

    public class ProjectFilesGenerator
    {
        public static void Run()
        {
            Generator.Launch<AssemblyFileGenerator>();
            Generator.Launch<CommonIncludeGenerator>();
        }
    }
}
