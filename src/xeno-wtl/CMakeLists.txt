
set (XENOIDE_WTL_INCLUDE_DIR "" CACHE PATH "")

if (NOT EXISTS ${XENOIDE_WTL_INCLUDE_DIR})
	message (SEND_ERROR "The supplied path in the variable XENOIDE_WTL_INCLUDE_DIR doesn't exists. (${XENOIDE_WTL_INCLUDE_DIR}).")
endif ()

add_library(wtl INTERFACE)
target_include_directories(wtl INTERFACE "${XENOIDE_WTL_INCLUDE_DIR}")


set (target "xeno-wtl")
set (sources 
    "include/xeno/wtl/wtl.h"
    "include/xeno/wtl/TreeManager.h"
    "include/xeno/wtl/CodeEditor.h"
    "include/xeno/wtl/MenuFactory.h"
    
    "src/TreeManager.cpp"
    "src/CodeEditor.cpp"
    "src/MenuFactory.cpp"
)

add_library (${target} ${sources})
target_include_directories (${target} PUBLIC "include/")
target_link_libraries (${target} wtl xeno-scintilla xeno-ui Boost::filesystem)
