using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;

namespace MetaGenerator
{
    public static class ProgramContext
    {
        public struct Option
        {
            public string option;
            public string value;

            public Option(string option, string value)
            {
                this.option = option;
                this.value = value;
            }
        }

        enum OptionImportance 
        {
            Necessary = 0,
            Optional
        }

        public const string NO_DLL_EXPORT_MACRO = "NO_EXPORT";

        private static string[] _analizedDirectories;

        public static string ProjectPath { get; private set; }
        public static string GenerationDirectory { get; private set; }
        public static string ProjectName => Path.GetFileName(Path.GetDirectoryName(ProjectPath));
        public static string DllExportMacro { get; private set; }
        public static string InternalDllExportMacro { get; private set; }
        public static bool HasDllExportMacro => DllExportMacro != NO_DLL_EXPORT_MACRO;

        public static readonly string[] CppFileExtensions = [".h", ".hpp"];

        public static void Initialize(string[] args)
        {
            if (args.Length == 0)
            {
                Console.WriteLine("No target project");
                Environment.Exit(1);
            }

            ProjectPath = args[0];
            Console.WriteLine($"Target project {ProjectPath}");

            ParseOptions(args);
            CreateWorkDirectory();

            InternalDllExportMacro = HasDllExportMacro == false
                ? DllExportMacro
                : $"__REFLECTION_{DllExportMacro}_API__";
        }

        public static IEnumerable<HeaderInfo> Headers
        {
            get
            {
                foreach (var directory in _analizedDirectories)
                {
                    foreach (var file in Directory.EnumerateFiles(directory, "*", SearchOption.AllDirectories)
                        .Where(file => CppFileExtensions.Contains(Path.GetExtension(file))))
                    {
                        yield return new HeaderInfo(File.ReadAllText(file), Path.GetRelativePath(directory, file));
                    }
                }
            }
        }

        private static void ParseOptions(string[] args)
        {
            Dictionary<string, string> options = new(args.Length);

            foreach (var arg in args)
            {
                var option = ParseConfigParameter(arg);

                if (option.HasValue == false)
                    continue;

                options.Add(option.Value.option, option.Value.value);
            }

            ExcludeAnalizedDirectories(options);

            var generationDirectoryName = ExcludeOptionValue("gen_dir_name", options, OptionImportance.Optional, "Generation");
            ExcludeGenerationDirectory(options, generationDirectoryName);
           
            DllExportMacro = ExcludeOptionValue("dll_export", options, OptionImportance.Optional, NO_DLL_EXPORT_MACRO);
        }

        private static void ExcludeAnalizedDirectories(Dictionary<string, string> options)
        {
            _analizedDirectories = ExcludeOptionValues("analyze_dirs", options, OptionImportance.Optional);

            if (_analizedDirectories == null)
            {
                _analizedDirectories = [ProjectPath];
            }
            else
            {
                for (int i = 0; i < _analizedDirectories.Length; i++)
                    _analizedDirectories[i] = Path.Combine(ProjectPath, _analizedDirectories[i]);
            }
        }

        private static void ExcludeGenerationDirectory(Dictionary<string, string> options, string generationDirectoryName)
        {
            GenerationDirectory = ExcludeOptionValue("gen_dir", options, OptionImportance.Optional);

            GenerationDirectory = GenerationDirectory == null
                ? Path.Combine(ProjectPath, generationDirectoryName)
                : Path.Combine(ProjectPath, GenerationDirectory, generationDirectoryName);
        }

        private static string ExcludeOptionValue(string option, Dictionary<string, string> options, OptionImportance importance = OptionImportance.Necessary, string defaultValue = null)
        {
            bool isContains = options.ContainsKey(option);

            if (isContains)
            {
                string optionValue = options[option];
                options.Remove(option);
                return optionValue;
            }

            if (isContains == false && importance == OptionImportance.Necessary)
            {
                Console.WriteLine($"Option \"{option}\" was not specified");
                Environment.Exit(1);
            }

            return defaultValue;
        }

        private static string[] ExcludeOptionValues(string option, Dictionary<string, string> options, OptionImportance importance = OptionImportance.Necessary, string[] defaultValue = null)
        {
            bool isContains = options.ContainsKey(option);

            if (isContains)
                return options[option].Split(',');

            if (isContains == false && importance == OptionImportance.Necessary)
            {
                Console.WriteLine($"Option \"{option}\" was not specified");
                Environment.Exit(1);
            }

            return defaultValue;
        }

        public static Option? ParseConfigParameter(string line)
        {
            var match = Regex.Match(line, @"(\S*)=(\S*)");

            if (match.Success == false)
            {
                Console.WriteLine($"Unavailable option {line}");
                return null;
            }

            return new(match.Groups[1].Value, match.Groups[2].Value);
        }

        private static void CreateWorkDirectory()
        {
            if (Directory.Exists(GenerationDirectory))
            {
                string[] entries = Directory.GetFiles(GenerationDirectory);
                foreach (string file in entries)
                    File.Delete(file);

                entries = Directory.GetDirectories(GenerationDirectory);
                foreach (string subDirectory in entries)
                    Directory.Delete(subDirectory, true);
            }

            var info = Directory.CreateDirectory(GenerationDirectory);
            info.Attributes = FileAttributes.Directory; // TODO: | FileAttributes.Hidden;
        }
    }
}
