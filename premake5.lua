workspace "CppReflection"
	
	configurations {
		"Debug32",
		"Debug64",
		"Release32",
		"Release64"
	}

	flags {
		"MultiProcessorCompile"
	}

	include "MetaGenerator"
	include "Reflection"

	group "Examples"
		include "Examples/AssemblyViewerLib"
		include "Examples/ExampleApp"
		include "Examples/ExampleLib"