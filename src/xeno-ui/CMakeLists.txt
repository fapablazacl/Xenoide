
set (target "xeno-ui")

set (sources
    "include/xeno/ui/WindowsUtils.h"
    "include/xeno/ui/FindReplaceDialog.h"
    "include/xeno/ui/Document.h"
    "include/xeno/ui/DocumentManager.h"
    "include/xeno/ui/FolderBrowser.h"
    "include/xeno/ui/IDEFrame.h"
    "include/xeno/ui/FileSearchDialog.h"
    "include/xeno/ui/DialogManager.h"
    "include/xeno/ui/Menu.h"
    "include/xeno/ui/MenuPanel.h"
    "include/xeno/ui/FileFilter.h"
    "include/xeno/ui/ObjectTreeView.h"
    "include/xeno/ui/TreeManagerController.h"
    "include/xeno/ui/TreeManagerControllerFileSystem.h"
    
    "src/WindowsUtils.cpp"
    "src/FindReplaceDialog.cpp"
    "src/Document.cpp"
    "src/DocumentManager.cpp"
    "src/FolderBrowser.cpp"
    "src/IDEFrame.cpp"
    "src/FileSearchDialog.cpp"
    "src/DialogManager.cpp"
    "src/Menu.cpp"
    "src/MenuPanel.cpp"
    "src/FileFilter.cpp"
    "src/ObjectTreeView.cpp"
    "src/TreeManagerControllerFileSystem.cpp"
)

add_library (${target} ${sources})

target_include_directories (${target} PUBLIC "include/")
target_compile_definitions (${target} PRIVATE "XENOIDE_BUILD")

if (UNIX)
    set (DL_LIBRARY dl)
endif ()

target_link_libraries (
    ${target} 
    ${DL_LIBRARY}
    ${Boost_LIBRARIES}
    xeno-core
)

install(TARGETS ${target} DESTINATION "lib")
