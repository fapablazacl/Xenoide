
set (target "Xenoide.Core")

set (sources
    "include/Xenoide/Core/Predef.h"
    "include/Xenoide/Core/Version.h"
    "include/Xenoide/Core/FileService.h"
    "include/Xenoide/Core/FileServiceImpl.h"
    "include/Xenoide/Core/FolderService.h"
    "include/Xenoide/Core/FolderServiceImpl.h"
    "include/Xenoide/Core/WorkspaceService.h"
    "include/Xenoide/Core/OS.h"

    "src/OS.cpp"
    "src/Version.cpp"
    "src/FileServiceImpl.cpp"
    "src/WorkspaceService.cpp"
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
