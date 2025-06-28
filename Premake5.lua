workspace "Guardian Engine"
	architecture "x64"
	
	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}

outputdir = "%{cfg.buildcfg} - %{cfg.system}/%{cfg.architecture}"

project "Guardian Engine"
	location "Guardian Engine"
	kind "SharedLib"
	language "C++"

	targetdir ("Build/" .. outputdir)
	objdir ("%{prj.name}/Intermediate/" .. outputdir)

	files
	{
		"%{prj.name}/Sources/**.h",
		"%{prj.name}/Sources/**.hpp",
		"%{prj.name}/Sources/**.cpp"
	}

	filter "system:Windows"
		cppdialect "C++20"
		staticruntime "Off"
		systemversion "latest"

		defines
		{
			"GE_PLATFORM_WINDOWS",
			"GE_BUILD_DLL"
		}

	filter "configurations:Debug"
		runtime "Debug"
		defines
		{
			"GE_DEBUG"
		}
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		defines
		{
			"GE_RELEASE"
		}
		optimize "On"

	filter "configurations:Distribution"
		defines
		{
			"GE_DISTRIBUTION"
		}
		optimize "On"

project "Guardian Editor"
	location "Guardian Editor"
	kind "ConsoleApp"
	language "C++"

	targetdir ("Build/" .. outputdir)
	objdir ("%{prj.name}/Intermediate/" .. outputdir)

	files
	{
		"%{prj.name}/Sources/**.h",
		"%{prj.name}/Sources/**.hpp",
		"%{prj.name}/Sources/**.cpp"
	}

	includedirs
	{
		"Guardian Engine/Sources"
	}

	links
	{
		"Guardian Engine"
	}

	filter "system:Windows"
		cppdialect "C++20"
		staticruntime "Off"
		systemversion "latest"

		defines
		{
			"GE_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		runtime "Debug"
		defines
		{
			"GE_DEBUG"
		}
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		defines
		{
			"GE_RELEASE"
		}
		optimize "On"

	filter "configurations:Distribution"
		defines
		{
			"GE_DISTRIBUTION"
		}
		optimize "On"