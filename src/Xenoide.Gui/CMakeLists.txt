
set (target Xenoide.Gui)

set (sources
    "include/Xenoide/Gui/WindowsUtils.h"
    "include/Xenoide/Gui/FindReplaceDialog.h"
    "include/Xenoide/Gui/Document.h"
    "include/Xenoide/Gui/DocumentManager.h"
    "include/Xenoide/Gui/FolderBrowser.h"
    "include/Xenoide/Gui/IDEFrame.h"
    "include/Xenoide/Gui/FileSearchDialog.h"
    "include/Xenoide/Gui/DialogManager.h"
    "include/Xenoide/Gui/Menu.h"
    "include/Xenoide/Gui/MenuPanel.h"
    "include/Xenoide/Gui/FileFilter.h"
    "include/Xenoide/Gui/ObjectTreeView.h"

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
    Xenoide.Core
)

install(TARGETS ${target} DESTINATION "lib")
