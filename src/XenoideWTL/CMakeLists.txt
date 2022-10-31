
set (target XenoideWTL)

set (sources
	"include/CDocumentManager.h"
    "include/CDocument.h"
    "include/CIdeFrame.h"
	"include/CClassView.h"
	"include/CFolderBrowser.h"
	"include/CDialogManager.h"

	"src/CDocumentManager.cpp"
	"src/CDocument.cpp"
	"src/CIdeFrame.cpp"
	"src/CClassView.cpp"
	"src/CFolderBrowser.cpp"
    "src/CDialogManager.cpp"
	"src/Main.cpp"

	"res/Resource.rc"
	"res/resource.h"
)

add_executable (${target} WIN32 ${sources})

# target_compile_definitions (${target} PUBLIC _UNICODE)

target_include_directories (${target} PUBLIC "include")
target_include_directories (${target} PUBLIC "res")

target_link_libraries(${target} 
	xeno-ui 
	xeno-scintilla 
	xeno-wtl 
)