
cmake_minimum_required (VERSION 3.10)

project (Xenoide LANGUAGES C CXX)

option (XENOIDE_TESTS "Builds experimental programs and libraries for development and testing purposes" OFF)
option (XENOIDE_TEST_SANDBOX "" OFF)

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

# Specify the use of the C++17 standard
set (CMAKE_CXX_STANDARD 17)

# if (UNIX)
#     add_definitions ("-fPIC")
# endif ()

add_subdirectory("dep")
add_subdirectory("src")

# IDE Core
if (XENOIDE_TESTS)
    add_subdirectory ("tests/gui-mvp")
    add_subdirectory ("tests/qt5-scintilla-poc")
    add_subdirectory ("tests/gui-wx")
endif ()

if (XENOIDE_TEST_SANDBOX)
	add_subdirectory ("tests/sandbox")
endif ()
