
cmake_minimum_required (VERSION 3.5)

project (Xenoide LANGUAGES Swift C CXX)

option (XENOIDE_GUI_COCOA "Builds the Cocoa based UI Frontend" OFF)
option (XENOIDE_GUI_GTK3 "Builds the Gtk3 based UI Frontend" OFF)
option (XENOIDE_GUI_QT5 "Builds the Qt5 based UI Frontend" OFF)
option (XENOIDE_GUI_WTL "Builds the WTL UI Frontend" OFF)

option (XENOIDE_TESTS "Builds experimental programs and libraries for development testing purposes" OFF)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
add_definitions (-DPROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR})

if (XENOIDE_STATIC_LINK)
    add_definitions (-DXENOIDE_STATIC_LINK)
endif ()

# we will require Boost ...
find_package(Boost REQUIRED COMPONENTS filesystem program_options)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

if (UNIX)
    # TODO: Link with install prefix
    set(CMAKE_INSTALL_RPATH "/usr/local/lib")
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif ()

# Specify the use of the C++17 standard
if (MSVC)
    add_definitions ("/std:c++latest")
else ()
    set (CMAKE_CXX_STANDARD 17)
endif (MSVC)

# if (UNIX)
#     add_definitions ("-fPIC")
# endif ()

# IDE Core
add_subdirectory ("modules/Xenoide.Core")
add_subdirectory ("modules/Xenoide.Gui")

#
if (XENOIDE_GUI_COCOA)
    add_subdirectory ("modules/Xenoide.Gui.Cocoa")
endif ()

if (XENOIDE_GUI_GTK3)
    add_subdirectory ("modules/Xenoide.Gui.Gtk3")
endif ()

if (XENOIDE_GUI_QT5)
    add_subdirectory ("modules/Xenoide")
    add_subdirectory ("modules/Xenoide.Gui.Qt5")
endif ()

if (XENOIDE_GUI_WTL)
    add_subdirectory ("modules/Xenoide.Gui.Wtl")
endif ()

# Experimental programs and libraries for development testing purposes
if (XENOIDE_TESTS)
    add_subdirectory ("tests/gui-mvp")
    add_subdirectory ("tests/qt5-scintilla-poc")
    add_subdirectory ("tests/gui-wx")
    add_subdirectory ("tests/sandbox")
endif ()
