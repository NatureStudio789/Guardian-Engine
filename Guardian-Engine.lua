workspace "Guardian Engine"
	architecture "x64"
	startproject "Guardian Editor"
	
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
        dependenciesdir .. "Includes/PhysX/",
		"Third Party/ImGui/Sources/",
		"Third Party/ImGuizmo/Sources/",
		"Third Party/yaml-cpp/include/",
		"Third Party/spdlog/include/",
		"Guardian EUI/Sources/",
	}

	libdirs
	{
		dependenciesdir .. "Libraries/DirectXTex/%{cfg.buildcfg}/",
		dependenciesdir .. "Libraries/Assimp/%{cfg.buildcfg}/",
		dependenciesdir .. "Libraries/DirectXTK/%{cfg.buildcfg}/",
        dependenciesdir .. "Libraries/PhysX/%{cfg.buildcfg}/",
	}

	links
	{
		"ImGui",
		"ImGuizmo",
		"yaml-cpp",
		"spdlog",
		"Guardian EUI",
	
		"Uxtheme",

		"dxgi",
		"d3d12",
		"d3dcompiler",
		"dxguid",
		"DirectXTex",
		"DirectXTK",

		"assimp",

        "LowLevel_static_64",
        "LowLevelAABB_static_64",
        "LowLevelDynamics_static_64",
        "PhysX_64",
        "PhysXCharacterKinematic_static_64",
        "PhysXCommon_64",
        "PhysXCooking_64",
        "PhysXExtensions_static_64",
        "PhysXFoundation_64",
        "PhysXPvdSDK_static_64",
        "PhysXTask_static_64",
        "PhysXVehicle_static_64",
        "PhysXVehicle2_static_64",
        "PVDRuntime_64"
	}

    defines
    {
        "YAML_CPP_STATIC_DEFINE",
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
        buildoptions {"/wd4996", "/MP"}
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
		"Third Party/ImGuizmo/Sources/",
		"Third Party/yaml-cpp/include/",
		"Third Party/spdlog/include/",
		"Guardian EUI/Sources/",
	}

	links
	{
		"Guardian Engine",
		"Guardian EUI"
	}

    defines
    {
        "YAML_CPP_STATIC_DEFINE",
		"GE_EDITOR_MODE",
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
        buildoptions {"/wd4996", "/MP"}
        buildoptions { "/permissive" }

	filter "configurations:Release"
		runtime "Release"
		defines
		{
			"GE_RELEASE"
		}
		optimize "On"

	filter "configurations:Distribution"
		runtime "Release"
		defines
		{
			"GE_DISTRIBUTION"
		}
		optimize "On"

project "Guardian Runtime"
	location "Guardian Runtime"
	kind "ConsoleApp"
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
		dependenciesdir .. "Includes/",
		"Guardian Engine/Sources",
		"Third Party/ImGui/Sources/",
		"Third Party/ImGuizmo/Sources/",
		"Third Party/yaml-cpp/include/",
		"Third Party/spdlog/include/",
	}

	links
	{
		"ImGui",
		"ImGuizmo",
		"yaml-cpp",
		"spdlog",

		"Guardian Engine",
	}

    defines
    {
        "YAML_CPP_STATIC_DEFINE",
		"GE_RUNTIME_MODE",
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
        buildoptions {"/wd4996", "/MP"}
        buildoptions { "/permissive" }

	filter "configurations:Release"
		runtime "Release"
		defines
		{
			"GE_RELEASE"
		}
		optimize "On"

	filter "configurations:Distribution"
		runtime "Release"
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
		"Third Party/ImGuizmo/Sources/",
		"Third Party/yaml-cpp/include/",
		"Third Party/spdlog/include/",
        dependenciesdir .. "Includes/",
    }

    links
    {
        "ImGui",
        "ImGuizmo",
        "spdlog",
    }

    defines
    {
        "YAML_CPP_STATIC_DEFINE",
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
        buildoptions {"/wd4996", "/MP"}

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
        buildoptions {"/wd4996", "/MP"}
        runtime "Debug"

    filter "configurations:Release"
        runtime "Release"

	filter "configurations:Distribution"
		optimize "On"

project "ImGuizmo"
	location "Third Party/ImGuizmo"
	kind "StaticLib"
	language "C++"

	targetdir ("Build/" .. outputdir)
	objdir ("Third Party/%{prj.name}/Intermediate/" .. outputdir)

	files
	{
		"Third Party/%{prj.name}/Sources/**.h",
		"Third Party/%{prj.name}/Sources/**.cpp",
	}

    includedirs
    {
        "Third Party/ImGui/Sources/"
    }

	links
	{
		"ImGui"
	}

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

    filter "configurations:Debug"
        buildoptions {"/wd4996", "/MP"}
        runtime "Debug"

    filter "configurations:Release"
        runtime "Release"

	filter "configurations:Distribution"
		optimize "On"

project "yaml-cpp"
    location "Third Party/yaml-cpp"
	kind "StaticLib"
	language "C++"

	targetdir ("Build/" .. outputdir)
    objdir ("Third Party/%{prj.name}/Intermediate/" .. outputdir)

	files
	{
		"Third Party/%{prj.name}/src/**.h",
		"Third Party/%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Third Party/%{prj.name}/include/"
	}

    defines
    {
        "YAML_CPP_STATIC_DEFINE",
    }

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++20"
		staticruntime "Off"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

project "spdlog"
    location "Third Party/spdlog"
	kind "StaticLib"
	language "C++"

	targetdir ("Build/" .. outputdir)
    objdir ("Third Party/%{prj.name}/Intermediate/" .. outputdir)

	files
	{
		"Third Party/%{prj.name}/include/**.h",
		"Third Party/%{prj.name}/src/**.h",
		"Third Party/%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Third Party/%{prj.name}/include/"
	}

	defines
	{
		"SPDLOG_COMPILED_LIB"
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++20"
		staticruntime "Off"

	filter "configurations:Debug"
        buildoptions {"/wd4996", "/MP"}
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"