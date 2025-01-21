workspace "Guardian Engine"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "Build/%{cfg.buildcfg}/%{cfg.architecture}/"

project "Guardian Engine"
    location "Guardian Engine"
    kind "SharedLib"
    language "C++"

    targetdir (outputdir)
    objdir ("%{prj.name}/Immediate/%{cfg.buildcfg}/%{cfg.architecture}/")

    files
    {
        "%{prj.name}/Sources/**.h",
        "%{prj.name}/Sources/**.cpp",
    }

    includedirs
    {
        "Setup/Includes/",
    }

    libdirs
    {
        "Setup/Libraries/DirectX 11/%{cfg.architecture}/%{cfg.buildcfg}/",
    }

    links
    {
        "d3d11",
        "DirectXTK",
        "D3DCompiler",
        "DXGI",
        "DirectXTex",
        "ImGui"
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

    filter "configurations:Release"
        runtime "Release"
        defines
        {
            "GE_RELEASE"
        }

        buildoptions {"/wd4251", "/MP"}
        buildoptions {"/wd4275", "/MP"}

project "Guardian Editor"
    location "Guardian Editor"
    kind "ConsoleApp"
    language "C++"

    targetdir (outputdir)
    objdir ("%{prj.name}/Immediate/%{cfg.buildcfg}/%{cfg.architecture}/")

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
        "Guardian Engine",
        "ImGui"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "Off"
        systemversion "latest"

        defines
        {
            "GE_PLATFORM_WINDOWS",
            "GE_EDITOR_PROGRAM",
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

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

    targetdir (outputdir)
    objdir ("%{prj.name}/Immediate/%{cfg.buildcfg}/%{cfg.architecture}/")

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

project "ImGui"
    location "ThirdParty/ImGui"
    kind "StaticLib"
    language "C++"

    targetdir (outputdir)
    objdir ("ThirdParty/%{prj.name}/Immediate/%{cfg.buildcfg}/%{cfg.architecture}/")

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