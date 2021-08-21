
# WTL Include Path
set (XENOIDE_WTL_INCLUDE_DIR "" CACHE PATH "")

if (NOT EXISTS ${XENOIDE_WTL_INCLUDE_DIR})
	message (SEND_ERROR "The supplied path in the variable XENOIDE_WTL_INCLUDE_DIR doesn't exists. (${XENOIDE_WTL_INCLUDE_DIR}).")
endif ()

# Scintilla Include Path
set (XENOIDE_SCINTILLA_INCLUDE_DIR "" CACHE PATH "")

if (NOT EXISTS ${XENOIDE_SCINTILLA_INCLUDE_DIR})
	message (SEND_ERROR "The supplied path in the variable XENOIDE_SCINTILLA_INCLUDE_DIR doesn't exists. (${XENOIDE_SCINTILLA_INCLUDE_DIR}).")
endif ()

# Lexilla Include Path
set (XENOIDE_LEXILLA_INCLUDE_DIR "" CACHE PATH "")

if (NOT EXISTS ${XENOIDE_LEXILLA_INCLUDE_DIR})
	message (SEND_ERROR "The supplied path in the variable XENOIDE_LEXILLA_INCLUDE_DIR doesn't exists. (${XENOIDE_LEXILLA_INCLUDE_DIR}).")
endif ()


set (target XenoShader.Wtl)

set (sources
	"src/LexillaAccess.h"
	"src/LexillaAccess.cxx"
	"src/XenoShader.cpp"
	"res/Resource.rc"
	"res/resource.h"
)

add_executable (${target} WIN32 ${sources})

# target_compile_definitions (${target} PUBLIC _UNICODE)
# target_include_directories (${target} PUBLIC "include")

target_include_directories (${target} PRIVATE
	"res"
	${XENOIDE_WTL_INCLUDE_DIR}
	${XENOIDE_SCINTILLA_INCLUDE_DIR}
	${XENOIDE_LEXILLA_INCLUDE_DIR}
)

# target_include_directories (${target} PUBLIC ${XENOIDE_WTL_INCLUDE_DIR})
# target_include_directories (${target} PRIVATE ${XENOIDE_SCINTILLA_INCLUDE_DIR})

target_link_libraries (${target} Xenoide.Core Xenoide.Gui)
