using MetaGenerator;
using System;
using System.Diagnostics;

Console.WriteLine("MetaGenerator running");

Stopwatch sw = new();
sw.Start();

foreach (string arg in args)
    Console.WriteLine(arg);

ProgramContext.Initialize(args);

foreach (var header in ProgramContext.Headers)
{
    HeaderParser parser = new(header);
    parser.Parse();

    var fileInfo = parser.FileInfo;
    if (fileInfo.HasValue == false)
        continue;

    CodeGenerator codeGenerator = new(fileInfo.Value);
    codeGenerator.GenerateForFile();
}

CodeGenerator.GenerateProjectCommonInclude();
CodeGenerator.GenerateProjectAssemblyFile();

sw.Stop();
Console.WriteLine($"MetaGenerator running done... (${sw.ElapsedMilliseconds} ms)");
