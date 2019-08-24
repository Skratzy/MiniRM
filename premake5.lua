-- premake5.lua
project "MemoryManager"
	kind "StaticLib"
	language "C++"
	
	targetdir (binDir)
	objdir (intermediatesDir)

	files {
		"ResourceManager/src/Defines.h",
		"ResourceManager/src/ResManAPI/ResourceManager.cpp",
		"ResourceManager/src/ResManAPI/ResourceManager.h",
		"ResourceManager/src/ResManAPI/Resources/Resource.h",
		"ResourceManager/src/ResManAPI/FormatLoaders/FormatLoader.cpp",
		"ResourceManager/src/ResManAPI/FormatLoaders/FormatLoader.h",
		"ResourceManager/src/miniz/miniz.h",
		"ResourceManager/src/miniz/miniz.c"
	}
	
	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "On"
		
	filter { "system:windows", "configurations:Release" }
		buildoptions "/MT"
