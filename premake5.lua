--[[ Premake5 configuration file ]]--
PROJECT_ROOT_DIR = path.getabsolute("")

workspace "ProjectDeadrop"
	configurations { "Debug", "Release", "Production"}
	platforms { "x32", "x64" }
	objdir "build/bin/%{cfg.platform}_%{cfg.buildcfg}"
    location ("build/" .. _ACTION)
	
	filter "platforms:x32"
		architecture "x32"
		
	filter "platforms:x64"
		architecture "x64"
	
	filter "system:Windows"
		defines { "PROJECT_PLATFORM_WIN" }
		
	filter { "system:Windows" and "platforms:x32" }
		defines { "PROJECT_PLATFORM_WIN32" }
	
	filter { "system:Windows" and "platforms:x64" }
		defines { "PROJECT_PLATFORM_WIN64" }
		
	-- These settings will apply to the entire workspace or project (whichever is active)
	filter {}
		-- Each configuration has its own obj file dir
		objdir ("build/" .. _ACTION .. "/obj")
		-- Disable precompiled headers
		flags { "NoPCH", "NoManifest", "MultiProcessorCompile"}
		cppdialect "C++17" 
		characterset "Unicode"
	
	filter "configurations:Debug"
		defines { "_DEBUG", "PROJECT_BUILD_DEBUG" }
		symbols "On"
		
	filter "configurations:Release"
		defines { "NDEBUG", "PROJECT_BUILD_RELEASE"}
		symbols "On"
		optimize "On"
		
	filter "configurations:Production"
		defines { "PROJECT_BUILD_PRODUCTION" }
		symbols "Off"
		optimize "Full"
		
	filter ""
		
	-- Enable Full symbols for Debug and Release builds if using VS2017
	filter { "action:vs2019" and "configurations:Debug" or "configurations:Release" }
		symbols "Full" -- Visual Studio 2017 or newer
		
	-- Use latest available winsdk
	filter {"system:windows", "action:vs*"}
		systemversion("latest")

    -- Build the engine.
	include("engine/build_engine.lua")
	
    -- Build the Sandbox game project
    include("sandbox/build_sandbox.lua")
    
