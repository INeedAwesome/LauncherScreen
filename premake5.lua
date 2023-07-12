workspace "LauncherScreen"
   architecture "x64"
   configurations { "Debug", "Release", "Dist"}
   startproject "Sandbox"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "LauncherScreen"
    location "LauncherScreen"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "On" -- Sets <RuntimeLibrary> to "MultiThreaded"

    pchheader "lascpch.h"
    pchsource "%{prj.name}/src/lascpch.cpp"

    targetdir ("bin/" .. outputdir .. "-%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "-%{prj.name}")

    files 
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs 
    {
        "$(ProjectDir)src\\"
    }

    filter "system:windows"
        systemversion "latest"
        defines { "LS_PLATFORM_WINDOWS" }

    filter "configurations:Debug"
        defines "LS_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "LS_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "LS_DIST"
        optimize "Speed"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "On" -- Sets <RuntimeLibrary> to "MultiThreaded"

    targetdir ("bin/" .. outputdir .. "-%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "-%{prj.name}")

    files 
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs 
    {
        "LauncherScreen/src"
    }

    links 
    {
        "LauncherScreen"
    }

    filter "system:windows"
        systemversion "latest"
        defines { "SB_PLATFORM_WINDOWS" }

    filter "configurations:Debug"
        defines "SB_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "SB_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "SB_DIST"
        optimize "Speed"