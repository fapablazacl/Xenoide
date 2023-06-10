
add_subdirectory("src/XenoideLexerCPP")
add_subdirectory("src/XenoideLexerGLSL")

set (target XenoshaderWTL)

set (sources
	"src/Resource.h"
	"src/FolderExplorer.h"
	"src/FolderExplorer.cpp"
	"src/MainFrame.h" 
	"src/MainFrame.cpp" 
	"src/AboutDlg.h" 
	"src/AboutDlg.cpp" 
    "src/TreeManager.h"
	"src/TreeManager.cpp"
	"../../XenoideWTL/src/CodeEditor.h"
	"../../XenoideWTL/src/CodeEditor.cpp"
    "src/MenuFactory.h"
	"src/MenuFactory.cpp"
	"../../XenoideWTL/src/CodeEditorConfiguration.h"
	"../../XenoideWTL/src/CodeEditorConfiguration.cpp"
	"src/DocumentManager.cpp" 
	"src/DocumentManager.h" 
	"src/FileNamePatternMatcher.h" 
	"src/FileNamePatternMatcher.cpp" 
	"src/XenoshaderWTL.cpp"
	"res/XenoshaderWTL.rc"

)

add_executable (${target} WIN32 ${sources})

target_compile_definitions (${target} PUBLIC _UNICODE)
target_compile_definitions (${target} PRIVATE 
	STRICT 
	WIN32_LEAN_AND_MEAN 
	_WTL_USE_CSTRING
)

target_include_directories (${target} PUBLIC "include")
target_include_directories (${target} PRIVATE "res")

target_link_libraries (${target} 
	xeno-ui 
	xeno-scintilla 
	xeno-wtl 
	Boost::filesystem 
	XenoideLexerGLSL 
	XenoideLexerCPP
)
