
# WTL
set (XENOIDE_WTL_INCLUDE_DIR "" CACHE PATH "")

if (NOT EXISTS ${XENOIDE_WTL_INCLUDE_DIR})
	message (SEND_ERROR "The supplied path in the variable XENOIDE_WTL_INCLUDE_DIR doesn't exists. (${XENOIDE_WTL_INCLUDE_DIR}).")
endif ()

add_library(wtl INTERFACE)
target_include_directories(wtl INTERFACE "${XENOIDE_WTL_INCLUDE_DIR}")

# Xenoide WTL Wrapper
set (target "xeno-wtl")
set (sources 
    "include/xeno/wtl/wtl.h"
)

add_library (${target} INTERFACE)
target_include_directories (${target} INTERFACE "include/")
target_link_libraries (${target} INTERFACE wtl xeno-scintilla)
