project "CppReflection"
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
		"Include/Reflection",
		"Source"
	}
	
	defines {
		"REFLECTION_DLL_EXPORT",
	}

	---- prebuildcommands "..\\MetaGenerator\\Build\\Bin\\Debug\\netcoreapp3.1\\MetaGenerator.exe %{prj.location} analyze_dirs=Include gen_dir=Include dll_export=REFLECTION_DLL_EXPORT"

	filter "configurations:*32"
		architecture "x86"

	filter "configurations:*64"
		architecture "x64"

	filter "configurations:Debug*"
		defines {
			"CR_DEBUG",
		}
		symbols "On"

	filter "configurations:Release*"
		defines {
			"CR_RELEASE",
		}
		symbols "Off"