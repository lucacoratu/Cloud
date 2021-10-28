workspace "Cloud"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}

outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "CloudServer"
	location "."
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputDir .. "/")
	objdir ("bin/" .. outputDir .. "/")

	pchheader "cloudpch.h"
	pchsource "src/cloudpch.cpp"

	files
	{
		"./src/**.h",
		"./src/**.cpp"
	}

	includedirs
	{
		"./External/spdlog/include",
		"./src"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

	filter "configurations:Debug"
		defines "SV_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "SV_RELEASE"
		optimize "On"

	filter "configurations:Distribution"
		defines "SV_DISTRIBUTION"
		optimize "On"