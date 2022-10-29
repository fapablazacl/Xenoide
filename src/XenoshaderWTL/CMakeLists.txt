
set (target XenoshaderWTL)

set (sources
	"src/FolderExplorer.h"
	"src/FolderExplorer.cpp"
	"src/MainFrame.h" 
	"src/MainFrame.cpp" 
	"src/AboutDlg.h" 
	"src/AboutDlg.cpp" 
    "src/TreeManager.h"
	"src/TreeManager.cpp"
	"src/CodeEditor.h"
	"src/CodeEditor.cpp"
    "src/MenuFactory.h"
	"src/MenuFactory.cpp"
	"src/CodeEditorConfiguration.h"
	"src/CodeEditorConfiguration.cpp"
	"src/DocumentManager.cpp" 
	"src/DocumentManager.h" 
	"src/FileNamePatternMatcher.h" 
	"src/FileNamePatternMatcher.cpp" 
	"src/XenoshaderWTL.cpp"
	
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

target_link_libraries (${target} xeno-ui xeno-scintilla xeno-wtl Boost::filesystem)
