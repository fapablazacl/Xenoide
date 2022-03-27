
set (target XenoshaderWTL)

set (sources
	"include/framework.h"
	"include/AppController.h"
	"include/FolderExplorer.h"
	"include/DocumentManager.h" 
	"include/FolderView.h" 
	"include/MainFrame.h" 
	"include/AboutDlg.h" 

	"src/XenoShader.cpp"
	"src/FolderExplorer.cpp"
	"src/DocumentManager.cpp" 
	"src/FolderView.cpp" 
	"src/MainFrame.cpp" 
	"src/AboutDlg.cpp" 
	
	"res/Resource.rc"
	"res/resource.h"
)

add_executable (${target} WIN32 ${sources})

# target_compile_definitions (${target} PUBLIC _UNICODE)
target_compile_definitions (${target} PRIVATE STRICT)
target_compile_definitions (${target} PRIVATE WIN32_LEAN_AND_MEAN)
target_compile_definitions (${target} PRIVATE _WTL_USE_CSTRING)

target_include_directories (${target} PUBLIC "include")
target_include_directories (${target} PRIVATE "res")

target_link_libraries (${target} xeno-ui xeno-scintilla xeno-wtl)
