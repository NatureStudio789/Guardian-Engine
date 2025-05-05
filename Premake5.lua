workspace "Guardian Engine"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

    flags
    {
        "MultiProcessorCompile"
    }

outputdir = "Build/%{cfg.buildcfg}/%{cfg.architecture}/"

project "Guardian Engine"
    location "Guardian Engine"
    kind "SharedLib"
    language "C++"

    targetdir (outputdir)
    objdir ("%{prj.name}/Intermediate/%{cfg.buildcfg}/%{cfg.architecture}/")

    files
    {
        "%{prj.name}/Sources/**.h",
        "%{prj.name}/Sources/**.cpp",
    }

    includedirs
    {
        "Setup/Includes/",
        "Setup/Includes/PhysX/",
        "ThirdParty/yaml-cpp/include/",
        "ThirdParty/ImGuizmo/Sources/",
    }

    libdirs
    {
        "Setup/Libraries/DirectX 11/%{cfg.architecture}/%{cfg.buildcfg}/",
        "Setup/Libraries/Assimp/%{cfg.buildcfg}/%{cfg.architecture}/",
        "Setup/Libraries/DirectXTex/%{cfg.buildcfg}/",
        "Setup/Libraries/mono/%{cfg.buildcfg}/",
        "Setup/Libraries/PhysX/%{cfg.buildcfg}/",
    }

    links
    {
        "ImGui",
        "ImGuizmo",
        "yaml-cpp",

        "d3d11",
        "DirectXTK",
        "D3DCompiler",
        "DXGI",
        "DirectXTex",
        "assimp",

        "ole32",
        "windowscodecs",
        "uuid",
        "ws2_32",
        "winmm",
        "version",
        "bcrypt",

        "libmono-static-sgen",

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

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "Off"
        systemversion "latest"

        defines
        {
            "GE_PLATFORM_WINDOWS",
            "GE_BUILD_DLL",
            "_WINDLL",
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
        buildoptions {"/wd4099", "/MP"}
        buildoptions { "/permissive" }

    filter "configurations:Release"
        runtime "Release"
        defines
        {
            "GE_RELEASE"
        }

        buildoptions {"/wd4251", "/MP"}
        buildoptions {"/wd4275", "/MP"}
        buildoptions {"/wd4819", "/MP"}
        buildoptions {"/wd4099", "/MP"}
        buildoptions { "/permissive" }

project "Guardian Editor"
    location "Guardian Editor"
    kind "ConsoleApp"
    language "C++"

    targetdir (outputdir)
    objdir ("%{prj.name}/Intermediate/%{cfg.buildcfg}/%{cfg.architecture}/")

    files
    {
        "%{prj.name}/Sources/**.h",
        "%{prj.name}/Sources/**.cpp",
    }

    includedirs
    {
        "Guardian Engine/Sources/",
        "ThirdParty/yaml-cpp/include/",
        "ThirdParty/ImGuizmo/Sources/",
        "Setup/Includes/",
        "Setup/Includes/PhysX/",
    }

    links
    {
        "Guardian Engine",
        "ImGui",
        "ImGuizmo",
        "yaml-cpp",
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

    filter "configurations:Release"
        runtime "Release"
        defines
        {
            "GE_RELEASE"
        }

        buildoptions {"/wd4251", "/MP"}
        buildoptions {"/wd4275", "/MP"}
        buildoptions {"/wd4819", "/MP"}

project "Guardian-ScriptCore"
    location "Guardian-ScriptCore"
    kind "SharedLib"
    language "C#"
    dotnetframework "4.7.2"

    targetdir ("Guardian Editor/Assets/Scripts/")
    objdir ("Guardian Editor/Assets/Scripts/Intermediate")

    files
    {
        "%{prj.name}/Sources/**.cs",
        "%{prj.name}/Properties/**.cs",
    }

    filter "configurations:Debug"
        optimize "Off"
        symbols "Default"

    filter "configurations:Release"
        optimize "On"
        symbols "Default"

project "ImGui"
    location "ThirdParty/ImGui"
    kind "StaticLib"
    language "C++"

    targetdir (outputdir)
    objdir ("ThirdParty/%{prj.name}/Intermediate/%{cfg.buildcfg}/%{cfg.architecture}/")

    files
    {
        "ThirdParty/%{prj.name}/Sources/**.h",
        "ThirdParty/%{prj.name}/Sources/**.cpp",
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"

    filter "configurations:Release"
        runtime "Release"

project "ImGuizmo"
    location "ThirdParty/ImGuizmo"
    kind "StaticLib"
    language "C++"

    targetdir (outputdir)
    objdir ("ThirdParty/%{prj.name}/Intermediate/%{cfg.buildcfg}/%{cfg.architecture}/")

    files
    {
        "ThirdParty/%{prj.name}/Sources/**.h",
        "ThirdParty/%{prj.name}/Sources/**.cpp",
    }

    includedirs
    {
        "Setup/Includes/ImGui/"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"

    filter "configurations:Release"
        runtime "Release"

project "yaml-cpp"
    location "ThirdParty/yaml-cpp"
	kind "StaticLib"
	language "C++"

	targetdir (outputdir)
	objdir ("ThirdParty/%{prj.name}/Intermediate/%{cfg.buildcfg}/%{cfg.architecture}/")

	files
	{
		"ThirdParty/%{prj.name}/src/**.h",
		"ThirdParty/%{prj.name}/src/**.cpp",
		
		"ThirdParty/%{prj.name}/include/**.h"
	}

	includedirs
	{
		"ThirdParty/%{prj.name}/include/"
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

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

    targetdir (outputdir)
    objdir ("%{prj.name}/Intermediate/%{cfg.buildcfg}/%{cfg.architecture}/")

    files
    {
        "%{prj.name}/Sources/**.h",
        "%{prj.name}/Sources/**.cpp",
    }

    includedirs
    {
        "Guardian Engine/Sources/",
        "Setup/Includes/",
    }

    links
    {
        "Guardian Engine"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "Off"
        systemversion "latest"

        defines
        {
            "GE_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        runtime "Debug"
        defines
        {
            "GE_DEBUG"
        }

        buildoptions {"/wd4251", "/MP"}
        buildoptions {"/wd4275", "/MP"}

    filter "configurations:Release"
        runtime "Release"
        defines
        {
            "GE_RELEASE"
        }

        buildoptions {"/wd4251", "/MP"}
        buildoptions {"/wd4275", "/MP"}
