projectName = "testObjectLoaderAlgortihm"

project(projectName)
    kind "WindowedApp"


    includedirs {
        "../../../Vendor/GLFW/include",
        "../../../Vendor/GLEW/include",
        "../../../Vendor/GLM/include",
        "../../Source"
    }

    libdirs {
        "../../../Vendor/GLFW/lib",
        "../../../Vendor/GLEW/lib64"
    }

    -- Order Matters!
    links { "glfw3", "GLEW:static", "GL", "X11", "pthread", "dl" }

    objdir ( "/tmp/" .. projectName )

    files {"*.cpp", "*.h", "../../Source/*.cpp", "../../Source/*.h"}

    excludes "../../Source/Main.cpp"

    targetdir "./Executables"

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG", }
        optimize "On"