--
-- premake5 file to build RecastDemo
-- http://premake.github.io/
--
require "lib"

local action = _ACTION or ""
local outdir = action

WorkSpaceInit("CppCfg")



DEF_CPP_EXT_LS={"cpp","h", "hpp", "text", "txt", "cfg"} --改写源码扩展名



Project "sample"
	IncludeFile { 
		"../include/",
	}

	SrcPath { 
		"../sample/**",  --**递归所有子目录，指定目录可用 "cc/*.cpp" 或者  "cc/**.cpp"
	}

Project "export_cpp"
	IncludeFile { 
		"../include/",
	}

	SrcPath { 
		"../export_cpp/**",  --**递归所有子目录，指定目录可用 "cc/*.cpp" 或者  "cc/**.cpp"
	}
	
Project "test"
	IncludeFile { 
		"../include/",
	}

	SrcPath { 
		"../test/**",  --**递归所有子目录，指定目录可用 "cc/*.cpp" 或者  "cc/**.cpp"
	}




    
    
    