# How to use Generator

Once you have set up the project, you need to set up a generator that will parse the project headers and generate meta information. The following arguments are used to set up the generator:
- analyze_dirs - specify in which directory the headers are located
- gen_dir - specify the directory in which the generated files will be located
- gen_dir_name - you can specify the directory name for the generated files (default is Generation)
- dll_export - if the project is a library, a unique project macro should be specified

Example:
```
MetaGenerator.exe ../MyProject/ 
    analyze_dirs=Include/MyProject 
    gen_dir=Include 
    dll_export=MY_PROJECT_DLL_EXPORT
```
