require("ecc/ecc")

-- ======================================
-- WORKSPACE("PlatformerGame")
-- ======================================
workspace "Pong2"
architecture "x64"
configurations {"Debug", "Release"}

filter "configurations:Debug"
defines {"DEBUG"}
symbols "On"

filter "configurations:Release"
defines {"NDEBUG"}
optimize "On"

filter {"action:vs*", "configurations:Release"}
staticruntime "On"

filter {"action:gmake", "configurations:Release"}
buildoptions {"-static-libgcc", "-static-libstdc++"}
linkoptions {"-static-libgcc", "-static-libstdc++"}

filter "system:windows"
defines {"PLATFORM_WINDOWS", "UNICODE", "_UNICODE"}

outdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- ======================================
-- PROJECT("SharedLib")
-- ======================================
project "SharedLib"
location "SharedLib"
kind "SharedLib"
language "C"
cdialect "C11"

targetdir("bin/" .. outdir .. "/data")
objdir("bin-int/" .. outdir .. "/data")

pchheader "pch.h"
pchsource "%{prj.name}/src/pch.c"

files {"%{prj.name}/include/**.h", "%{prj.name}/src/**.c"}
includedirs {"%{prj.name}/include", "%{prj.name}", "%{prj.name}/vendor"}
links {"user32", "gdi32", "opengl32"}
defines {"DLL_EXPORTS"}

-- I'm using gmake from MSYS2, for some reason i can't use {COPY} from premake because 
-- Makefile always will be runned with MSYS2 terminal injection. But premake will generate us the cmd style
-- command because we are using windows to build. So this is just windows error. 
filter {"action:gmake"}
postbuildcommands {'[ -f %{wks.location}/bin/' .. outdir .. '/data/SharedLib.dll ] && cp -f %{wks.location}/bin/' ..
    outdir .. '/data/SharedLib.dll %{wks.location}/bin/' .. outdir .. '/'}

filter {}

-- ======================================
-- PROJECT("Game")
-- ======================================
project "Game"
location "Game"
kind "ConsoleApp"
language "C"
cdialect "C11"

targetdir("bin/" .. outdir)
objdir("bin-int/" .. outdir)

files {"%{prj.name}/*.c"}
includedirs {"SharedLib"}
links {"SharedLib"}

