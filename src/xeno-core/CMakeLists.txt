
set (target "xeno-core")

set (sources
    "include/xeno/core/Predef.h"
    "include/xeno/core/Version.h"
    "include/xeno/core/FileService.h"
    "include/xeno/core/FolderService.h"
    "include/xeno/core/WorkspaceService.h"
    "include/xeno/core/Model.h"
    "include/xeno/core/OS.h"
    "include/xeno/core/FileSystemService.h"
    "include/xeno/core/FileSystemServiceBoost.h"
    "include/xeno/core/FolderExplorer.h"

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
