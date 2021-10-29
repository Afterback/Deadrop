--[[ Premake5 configuration file ]]--
CURRENT_PROJECT_DIR = path.getabsolute("")

project "SandboxGame"
	kind "WindowedApp"
	language "C++"
    
    -- set the debug directory for the IDE to use as a working directory
	debugdir "bin/"
    
    -- include all the header files
	includedirs
	{
		"../",
		PROJECT_ROOT_DIR .. "/engine/"
	}
	
    -- specify the source code files for this project
	files { "**.h", "**.cpp" }
	
    -- link the static library
	links { "DeadropEngine", "d3d11" }