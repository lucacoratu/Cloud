workspace "Cloud"
	architecture "x64"
	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}

outputDir = "%{prj.name}/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "CloudServer"
	location "%{prj.name}"
	kind "ConsoleApp"
	language "C++"
	

	targetdir ("bin/" .. outputDir .. "/")
	objdir ("bin-int/" .. outputDir .. "/")

	pchheader "cloudpch.h"
	pchsource "%{prj.location}/src/cloudpch.cpp"

	files
	{
		"%{prj.location}/src/**.h",
		"%{prj.location}/src/**.cpp",
		"%{prj.location}/External/sqlite/sqlite3.c"
	}

	includedirs
	{
		"%{prj.location}/External/spdlog/include",
		"%{prj.location}/External/sqlite",
		"%{prj.location}/src"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		defines "SV_WINDOWS"

	filter "configurations:Debug"
		defines "SV_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "SV_RELEASE"
		optimize "On"

	filter "configurations:Distribution"
		defines "SV_DISTRIBUTION"
		optimize "On"

	--Filters the .c files and sets their precompiled header option to Not using precompiled headers
	filter "files:**.c"
		flags {"NoPCH"}

project "CloudClient"
	location "%{prj.name}"
	language "C#"
	kind "WindowedApp"
	dotnetframework "netcoreapp3.1"

	flags
	{
		"WPF"
	}
	
	targetdir ("bin/" .. outputDir .. "/")
	objdir ("bin-int/" .. outputDir .. "/")

	files
	{
		"%{prj.location}/Icons/*.png",
		"%{prj.location}/*.cs",
		"%{prj.location}/src/**.cs"

	}

	filter "configurations:Debug"
		defines "CL_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "CL_RELEASE"
		optimize "On"

	filter "configurations:Distribution"
		defines "CL_DISTRIBUTION"
		optimize "On"