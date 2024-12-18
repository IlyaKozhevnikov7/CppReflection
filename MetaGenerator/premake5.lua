project "MetaGenerator"
	kind "ConsoleApp"
	language "C#"
	csversion "12.0"
	dotnetframework "netcoreapp3.1"
	excludes { "Build/**", "obj/**" } 

	nuget { "PCRE.NET:1.0.0" }

	targetdir("Build/Bin/%{cfg.buildcfg}")
	objdir("Build/Obj/%{cfg.buildcfg}")

	architecture "universal"

	files {
		"Source/**"
	}

	configmap {
		["Debug*"] = "Debug",
		["Release*"] = "Release"
	}

	filter "configurations:Debug"
		defines "REFLECTION_DEBUG"
		runtime "Debug"
		optimize "Off"
		symbols "On"
	
	filter "configurations:Release"
		defines "REFLECTION_RELEASE"
		runtime "Release"
		optimize "Full"
		symbols "Off"
