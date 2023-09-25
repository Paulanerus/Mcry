workspace "Mcry"
    configurations {"Debug", "Release"}

project "Mcry"
    kind "SharedLib"
    language "C++"
    cppdialect "C++17"
    architecture "x86_64"
    targetdir "lib/bin/%{cfg.buildcfg}"
    location "lib/build/"

    includedirs {"include/"}

    files {"src/**.cpp"}

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

project "Example"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    architecture "x86_64"
    targetdir "bin/%{cfg.buildcfg}"
    location "build/"

    includedirs {"include/"}

    links {"Mcry"}

    files {"example/main.cpp"}

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"