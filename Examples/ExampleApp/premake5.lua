project "ExampleApp"
	kind "ConsoleApp"
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
		"Include",
		"Source",

		"../../Reflection/Include"
	}

	links {
		"CppReflection"
	}

	filter "configurations:*32"
		architecture "x86"

	filter "configurations:*64"
		architecture "x64"

	filter "configurations:Debug*"
		symbols "On"

	filter "configurations:Release*"
		symbols "Off"
