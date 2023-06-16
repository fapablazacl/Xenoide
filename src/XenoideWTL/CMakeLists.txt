
set (target XenoideWTL)

set (sources
	"src/CDocumentManager.h"
    "src/CDocument.h"
    "src/CIdeFrame.h"
	"src/CClassView.h"
	"src/CFolderBrowser.h"
	"src/CDialogManager.h"

	"src/CDocumentManager.cpp"
	"src/CDocument.cpp"
	"src/CIdeFrame.cpp"
	"src/CClassView.cpp"
	"src/CFolderBrowser.cpp"
    "src/CDialogManager.cpp"
	"src/Main.cpp"

	"src/CustomTabView.h"
	
	"res/Resource.rc"
	"res/resource.h"

	"src/IdeCommands.h" 
	"src/MainFrame.h" 
	"src/MainFrame.cpp"
	"src/CodeEditor.cpp"
	"src/CodeEditor.h"
	"src/CodeEditorConfiguration.cpp"
	"src/CodeEditorConfiguration.h"
	"src/FileNamePatternMatcher.cpp"
	"src/FileNamePatternMatcher.h"
)

add_executable (${target} WIN32 ${sources} )

target_compile_definitions (${target} PUBLIC _UNICODE UNICODE)
target_include_directories (${target} PUBLIC "res")

target_link_libraries(${target} 
	xeno-ui 
	xeno-scintilla 
	xeno-wtl 
	gdiplus
)