
namespace MetaGenerator
{
    public class PerFileGenerator
    {
        public static void Run(in FileInfo file)
        {
            Generator.Launch<ImplementationFileGenerator, FileInfo>(file);
            RunIncludeFileGenerators(in file);
        }

        private static void RunIncludeFileGenerators(in FileInfo file)
        {
            foreach (var info in file.classInfos)
                Generator.Launch<InlineFileGenerator, ClassInfo>(info);
        }
    }
}
