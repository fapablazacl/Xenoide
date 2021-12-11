
set (target "Xenoide.Core")

set (sources
    "include/Xenoide/Core/Predef.h"
    "include/Xenoide/Core/Version.h"
    "include/Xenoide/Core/FileService.h"
    "include/Xenoide/Core/FolderService.h"
    "include/Xenoide/Core/WorkspaceService.h"
    "include/Xenoide/Core/Model.h"
    "include/Xenoide/Core/OS.h"
    "include/Xenoide/Core/FileSystemService.h"
    "include/Xenoide/Core/FileSystemServiceBoost.h"
    "include/Xenoide/Core/FolderExplorer.h"

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


if (XENOIDE_DYNAMIC_LINK)
	add_library (${target} SHARED ${sources})
else ()
    add_library (${target} ${sources})
endif()

target_include_directories( ${target} PUBLIC "include/")
target_compile_definitions (${target} PRIVATE "XENOIDE_BUILD")

if (UNIX)
    set (DL_LIBRARY dl)
endif ()

target_link_libraries (
    ${target} 
    ${DL_LIBRARY}
    ${Boost_LIBRARIES}
)

install(TARGETS ${target} DESTINATION "lib")
