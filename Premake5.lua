workspace "Guardian Engine"
	architecture "x64"
	
	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}
	
    flags
    {
        "MultiProcessorCompile"
    }

outputdir = "%{cfg.buildcfg} - %{cfg.system}/%{cfg.architecture}"
dependenciesdir = "Dependencies/"

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

	includedirs
	{
		dependenciesdir .. "Includes/",
		"Third Party/ImGui/Sources/",
		"Guardian EUI/Sources/",
	}

	libdirs
	{
		dependenciesdir .. "Libraries/DirectXTex/%{cfg.buildcfg}/",
		dependenciesdir .. "Libraries/Assimp/%{cfg.buildcfg}/",
	}

	links
	{
		"ImGui",
		"Guardian EUI",
	
		"Uxtheme",

		"dxgi",
		"d3d12",
		"d3dcompiler",
		"dxguid",
		"DirectXTex",

		"xaudio2",

		"assimp"
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

        buildoptions {"/wd4251", "/MP"}
        buildoptions {"/wd4275", "/MP"}
        buildoptions {"/wd4819", "/MP"}
        buildoptions {"/wd4099", "/MP"}
        buildoptions { "/permissive" }

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
		"%{prj.name}/Sources/**.cpp",
		"%{prj.name}/**.rc",
	}

	includedirs
	{
		dependenciesdir .. "Includes/",
		"Guardian Engine/Sources",
		"Third Party/ImGui/Sources/",
		"Guardian EUI/Sources/",
	}

	links
	{
		"ImGui",

		"Guardian Engine",
		"Guardian EUI"
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
		
        buildoptions {"/wd4251", "/MP"}
        buildoptions {"/wd4275", "/MP"}
        buildoptions {"/wd4819", "/MP"}
        buildoptions {"/wd4099", "/MP"}
        buildoptions { "/permissive" }

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

project "Guardian EUI"
    location "Guardian EUI"
    kind "StaticLib"
    language "C++"

    targetdir ("Build/" .. outputdir)
    objdir ("%{prj.name}/Intermediate/" .. outputdir)

    files
    {
        "%{prj.name}/Sources/**.h",
        "%{prj.name}/Sources/**.cpp",
    }

    includedirs
    {
        "Guardian Engine/Sources/",
		"Third Party/ImGui/Sources/",
        dependenciesdir .. "Includes/",
    }

    links
    {
        "ImGui",
    }

    filter "system:windows"
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

        buildoptions {"/wd4251", "/MP"}
        buildoptions {"/wd4275", "/MP"}
        buildoptions {"/wd4819", "/MP"}

    filter "configurations:Release"
        runtime "Release"
        defines
        {
            "GE_RELEASE"
        }

        buildoptions {"/wd4251", "/MP"}
        buildoptions {"/wd4275", "/MP"}
        buildoptions {"/wd4819", "/MP"}

project "ImGui"
    location "Third Party/ImGui"
    kind "StaticLib"
    language "C++"

    targetdir ("Build/" .. outputdir)
    objdir ("Third Party/%{prj.name}/Intermediate/" .. outputdir)

    files
    {
        "Third Party/%{prj.name}/Sources/**.h",
        "Third Party/%{prj.name}/Sources/**.cpp",
    }

	links
	{
		"dxgi",
		"d3d12",
		"d3dcompiler",
		"dxguid",
	}

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"

    filter "configurations:Release"
        runtime "Release"

	filter "configurations:Distribution"
		optimize "On"