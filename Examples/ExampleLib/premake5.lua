project "ExampleLib"
	kind "SharedLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"
	rtti "off"

	targetdir "%{wks.location}/Build/Bin/%{cfg.buildcfg}"
	objdir "%{wks.location}/Build/Obj/%{cfg.buildcfg}"

	files {
		"Include/**",
		"Source/**"
	}
	
	includedirs {
		"Include",
		"Include/%{prj.name}",
		"Include/Generation",
		"Source",

		"../../Reflection/Include",
	}

	defines {
		"EXAMPLE_LIB_DLL_EXPORT",
	}

	links {
		"CppReflection"
	}

	prebuildcommands "..\\..\\MetaGenerator\\Build\\Bin\\Debug\\netcoreapp3.1\\MetaGenerator.exe %{prj.location} analyze_dirs=Include/%{prj.name} gen_dir=Include dll_export=EXAMPLE_LIB_DLL_EXPORT"

	filter "configurations:*32"
		architecture "x86"

	filter "configurations:*64"
		architecture "x64"

	filter "configurations:Debug*"
		symbols "On"

	filter "configurations:Release*"
		symbols "Off"
