project "AssemblyViewerLib"
	kind "SharedLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"
	rtti "off"

	targetdir "%{wks.location}/Build/Bin/%{cfg.buildcfg}"
	objdir "%{wks.location}/Build/Obj/%{cfg.buildcfg}"

	files {
		"Source/**"
	}
	
	includedirs {
		"Source",
		"Source/Generation",

		"../../Reflection/Include",
		"../ExampleLib/Include",
	}

	defines {
		"ASSEMBLY_VIEWER_DLL_EXPORT",
	}

	libdirs {
		"../../Build/Bin/%{cfg.buildcfg}"
	}

	links {
		"CppReflection"
	}

	prebuildcommands "..\\..\\MetaGenerator\\Build\\Bin\\Debug\\netcoreapp3.1\\MetaGenerator.exe %{prj.location} analyze_dirs=Source gen_dir=Source dll_export=ASSEMBLY_VIEWER_DLL_EXPORT"

	filter "configurations:*32"
		architecture "x86"

	filter "configurations:*64"
		architecture "x64"

	filter "configurations:Debug*"
		symbols "On"

	filter "configurations:Release*"
		symbols "Off"
