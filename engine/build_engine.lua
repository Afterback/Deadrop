
project "DeadropEngine"
	kind "StaticLib"
	language "C++"
	
	includedirs
	{
		"../",
		--PROJECT_ROOT_DIR .. "/thirdparty/"
	}
	
	files 
	{ 
		"**.cpp",
		"**.h"
	}
		
	defines { "PROJECT_COMPILE_SHADERS_ONLINE" }
	
	filter "configurations:Debug"
		defines { "PROJECT_RENDERER_DEBUG" }
	
	--links { "thirdparty" }