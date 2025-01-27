# CppReflection

This is a solution for using reflection in a C++ project. With it, you can selectively apply reflection to your C++ types, and also get meta information about types at runtime.

## Requirements

- Standard: C++20
- Compiler: MSVC
- OS: Windows

## Build solution

After clone repository, use premake to generator project files:
```
premake5.exe vs2022
```
After generating the project files, build the solution.

## Project setup

Copy and include CppReflection (```.lib``` and ```.dll```) and ```Reflection/Include``` files to your project.

For example, you can configure your ide to run it before each compilation.
See [generator using](Docs/generator_using.md).

## Common features

- Automatic generation of metainformation
- Handling most language constructs (classes, enumerations, templates, fields, methods, etc.)
- Attributes
- Static and polymorphic type extraction
- Dynamically loading assemblies and getting available types

Here more information about [how to use CppReflection](Docs/how_to_use.md).
