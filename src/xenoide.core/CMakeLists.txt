
set (target "xenoide.core")

set (sources
    "include/xenoide/core/Predef.h"
    "include/xenoide/core/Version.h"
    "include/xenoide/core/FileService.h"
    "include/xenoide/core/FolderService.h"
    "include/xenoide/core/WorkspaceService.h"
    "include/xenoide/core/Model.h"
    "include/xenoide/core/OS.h"
    "include/xenoide/core/FileSystemService.h"
    "include/xenoide/core/FileSystemServiceBoost.h"
    "include/xenoide/core/FolderExplorer.h"

    "src/OS.cpp"
    "src/Version.cpp"
    "src/FileService.cpp"
    "src/FolderService.cpp"
    "src/WorkspaceService.cpp"
    "src/Model.cpp"
    "src/FileSystemService.cpp"
    "src/FileSystemServiceBoost.cpp"
    "src/FolderExplorer.cpp"
) 

add_library(${target} ${sources})

target_include_directories( ${target} PUBLIC "include/")
target_compile_definitions (${target} PRIVATE "XENOIDE_BUILD")

if (UNIX)
    set (DL_LIBRARY dl)
endif ()

target_link_libraries (
    ${target} 
    ${DL_LIBRARY}
    Boost::filesystem
)

install(TARGETS ${target} DESTINATION "lib")
