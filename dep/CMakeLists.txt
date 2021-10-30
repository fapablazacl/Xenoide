
# declare WTL
set (XENOIDE_WTL_INCLUDE_DIR "" CACHE PATH "")

if (NOT EXISTS ${XENOIDE_WTL_INCLUDE_DIR})
	message (SEND_ERROR "The supplied path in the variable XENOIDE_WTL_INCLUDE_DIR doesn't exists. (${XENOIDE_WTL_INCLUDE_DIR}).")
endif ()


add_library(wtl INTERFACE)
target_include_directories(wtl INTERFACE "${XENOIDE_WTL_INCLUDE_DIR}")

# declare Scintilla and Lexilla
# Scintilla Include Path
set (XENOIDE_SCINTILLA_INCLUDE_DIR "" CACHE PATH "")

if (NOT EXISTS ${XENOIDE_SCINTILLA_INCLUDE_DIR})
	message (SEND_ERROR "The supplied path in the variable XENOIDE_SCINTILLA_INCLUDE_DIR doesn't exists. (${XENOIDE_SCINTILLA_INCLUDE_DIR}).")
endif ()

add_library(Scintilla INTERFACE)
target_include_directories(Scintilla INTERFACE "${XENOIDE_SCINTILLA_INCLUDE_DIR}")


# Lexilla Include Path
set (XENOIDE_LEXILLA_INCLUDE_DIR "" CACHE PATH "")

if (NOT EXISTS ${XENOIDE_LEXILLA_INCLUDE_DIR})
	message (SEND_ERROR "The supplied path in the variable XENOIDE_LEXILLA_INCLUDE_DIR doesn't exists. (${XENOIDE_LEXILLA_INCLUDE_DIR}).")
endif ()

add_library(Lexilla INTERFACE)
target_include_directories(Lexilla INTERFACE "${XENOIDE_LEXILLA_INCLUDE_DIR}")
