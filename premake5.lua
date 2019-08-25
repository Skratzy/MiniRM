-- premake5.lua
project "ResourceManager"
	kind "StaticLib"
	language "C++"
	
	targetdir (binDir)
	objdir (intermediatesDir)

	files {
		"ResourceManager/src/Defines.h",
		"ResourceManager/src/ResourceManager.cpp",
		"ResourceManager/src/ResourceManager.h",
		"ResourceManager/src/Resources/Resource.h",
		"ResourceManager/src/FormatLoaders/FormatLoader.cpp",
		"ResourceManager/src/FormatLoaders/FormatLoader.h",
		"ResourceManager/src/miniz/miniz.h",
		"ResourceManager/src/miniz/miniz.c"
	}
	
	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "On"
		
	filter { "system:windows", "configurations:Release" }
		buildoptions "/MT"
