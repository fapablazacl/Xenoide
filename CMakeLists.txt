
cmake_minimum_required (VERSION 3.10)

project (Xenoide LANGUAGES C CXX)

# Specify the use of the C++17 standard
set (CMAKE_CXX_STANDARD 17)

# generic build options
option (XENOIDE_BUILD_POC_GTKCLASSBROWSER "" OFF)
option (XENOIDE_BUILD_POC_GUIMVP "" OFF)
option (XENOIDE_BUILD_POC_GUIWX "" OFF)
option (XENOIDE_BUILD_POC_QT5SCINTILLAPOC "" OFF)
option (XENOIDE_BUILD_POC_SANDBOX "" OFF)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
add_definitions (-DPROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR})

if (XENOIDE_STATIC_LINK)
    add_definitions (-DXENOIDE_STATIC_LINK)
endif ()

# Boost dependencies
find_package(Boost REQUIRED COMPONENTS filesystem program_options)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

if (UNIX)
    # TODO: Link with install prefix
    set(CMAKE_INSTALL_RPATH "/usr/local/lib")
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif ()

# if (UNIX)
#     add_definitions ("-fPIC")
# endif ()

add_subdirectory("dep")

# add IDE core 
add_subdirectory("src")

if (XENOIDE_BUILD_POC_GTKCLASSBROWSER)
    add_subdirectory ("pocs/gtk-class-browser")
endif()

if (XENOIDE_BUILD_POC_GUIMVP)
    add_subdirectory ("pocs/gui-mvp")
endif ()

if (XENOIDE_BUILD_POC_GUIWX)
    add_subdirectory ("pocs/gui-wx")
endif ()

if (XENOIDE_BUILD_POC_QT5SCINTILLAPOC)
    add_subdirectory ("pocs/qt5-scintilla-poc")
endif ()

if (XENOIDE_BUILD_POC_SANDBOX)
    add_subdirectory ("pocs/sandbox")
endif ()
