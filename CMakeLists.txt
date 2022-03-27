
cmake_minimum_required (VERSION 3.10)

project (Xenoide LANGUAGES C CXX)

option (XENOBUILD_ENABLE_LINTER "Enable the use of Linter Tool. clang-tidy must be installed." OFF)
option (XENOBUILD_ENABLE_TESTS "Enable the automated self-tests for all libraries." OFF)
option (XENOIDE_BUILD_POC_GTKCLASSBROWSER "" OFF)
option (XENOIDE_BUILD_POC_GUIMVP "" OFF)
option (XENOIDE_BUILD_POC_GUIWX "" OFF)
option (XENOIDE_BUILD_POC_QT5SCINTILLAPOC "" OFF)
option (XENOIDE_BUILD_POC_SANDBOX "" OFF)

# Specify the use of the C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
add_definitions (-DPROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR})

if (UNIX)
    # TODO: Link with install prefix
    set(CMAKE_INSTALL_RPATH "/usr/local/lib")
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif ()

# Boost
find_package(Boost
    REQUIRED
    COMPONENTS program_options filesystem thread
)

# some definitions
add_definitions(-DCMAKE_CURRENT_SOURCE_DIR="$CMAKE_CURRENT_SOURCE_DIR")

# yaml-cpp
set(YAML_CPP_BUILD_TESTS "OFF" CACHE STRING "" FORCE)
set(YAML_CPP_BUILD_CONTRIB "OFF" CACHE STRING "" FORCE)
set(YAML_CPP_BUILD_TOOLS "OFF" CACHE STRING "" FORCE)
set(YAML_CPP_INSTALL "OFF" CACHE STRING "" FORCE)
add_subdirectory("dependencies/yaml-cpp")

# add unit test support libraries
if (XENOBUILD_ENABLE_TESTS)
    add_subdirectory("dependencies/googletest")
    include(CTest)
endif()

if (XENOBUILD_ENABLE_LINTER)
    set(CMAKE_CXX_CLANG_TIDY "clang-tidy")
else ()
    set(CMAKE_CXX_CLANG_TIDY "")
endif()

# Enable Full Warning for all build configuration and compilers
if(MSVC)
    add_compile_options(/W3)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_subdirectory("dep")
add_subdirectory("src")
add_subdirectory("pocs")
