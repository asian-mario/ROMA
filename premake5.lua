workspace "ROCA"
    architecture "x64"
    configurations { "Debug", "Release" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "ROCA"
    location "ROCA"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs {
        "src",
        "src/vendor",
        "src/vendor/imgui"
    }

    links {
        "ImGui"
        "ws2_32.lib"
    }

    filter "system:windows"
        systemversion "latest"
        defines {
            "PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines { "DEBUG" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        runtime "Release"
        optimize "On"

project "ImGui"
    location "src/vendor/imgui"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "src/vendor/imgui/*.cpp"
    }

    includedirs {
        "src/vendor/imgui"
    }

    filter "system:windows"
        systemversion "latest"
        defines {
            "PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines { "DEBUG" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        runtime "Release"
        optimize "On"
