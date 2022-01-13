set (target "qt5-scintilla-poc")

set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)


if (WIN32)
    # this doesn't work on Linux (arch)
    find_library(QScintilla REQUIRED)
endif()

# required for QScintilla
include_directories("${Qt5_DIR}/../../../include")
link_directories("${Qt5_DIR}/../../")

include_directories("../gui-mvp")

set (sources 
    "src/main.cpp"
    "src/QMainFrame.cpp"
    "src/QMainFrame.hpp"
)

add_definitions (-DXENOIDE_TARGET_NAME=${target})
add_executable(${target} ${sources})

# TODO: Determine if we need a static linking library for QScintilla
target_include_directories(${target} PUBLIC "include/")
target_compile_definitions (${target} PRIVATE "QSCINTILLA_DLL")

# TODO: Parametrize QScintilla for Windows Builds
if (WIN32)
    target_link_libraries (
        ${target} 
        Qt5::Widgets
        debug qscintilla2_qt5d
        optimized qscintilla2_qt5
        Xenoide.Core
        Xenoide.Gui
        gui-mvp
    )
else ()
    target_link_libraries (
        ${target} 
        Qt5::Widgets
        qscintilla2_qt5
        Xenoide.Core
        Xenoide.Gui
        gui-mvp
    )
endif ()

install (TARGETS ${target} DESTINATION "lib")
