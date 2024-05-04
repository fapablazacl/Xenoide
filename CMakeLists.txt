cmake_minimum_required(VERSION 3.25)

project (Xenoide)

# version
set(XENOIDE_VERSION_MAJOR "0" CACHE STRING "Xenoide major version, for breaking feature changes")
set(XENOIDE_VERSION_MINOR "0" CACHE STRING "Xenoide minor version, for non-breaking feature changes")
set(XENOIDE_VERSION_REVISION "0" CACHE STRING "Xenoide bugfix number")

# build tracking information
set(XENOIDE_BUILD_COMMIT "local" CACHE STRING "Git hash used to generate this build")
set(XENOIDE_BUILD_TIMESTAMP "now" CACHE STRING "When this build was made, in YYYYmmDDHHMMSS format (if set)")

# Qt packages
set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTOUIC ON)
find_package(Qt6 COMPONENTS Core Widgets REQUIRED)

# Qscintilla target
message (STATUS "Qt6_DIR: ${Qt6_DIR}")
add_library(Qt6QScintilla INTERFACE)
target_include_directories(Qt6QScintilla INTERFACE "${Qt6_DIR}/../../include")
target_link_directories(Qt6QScintilla INTERFACE "${Qt6_DIR}/../../lib")
target_link_libraries(Qt6QScintilla INTERFACE qscintilla2_qt6)

# Xenoide executable
set (target Xenoide)
set (sources main.cpp MainWindow.cpp MainWindow.h LanguageConfig.cpp LanguageConfig.h)

add_executable(${target} ${sources})

target_compile_definitions(${target} PRIVATE XENOIDE_VERSION_MAJOR=${XENOIDE_VERSION_MAJOR})
target_compile_definitions(${target} PRIVATE XENOIDE_VERSION_MINOR=${XENOIDE_VERSION_MINOR})
target_compile_definitions(${target} PRIVATE XENOIDE_VERSION_REVISION=${XENOIDE_VERSION_REVISION})
target_compile_definitions(${target} PRIVATE XENOIDE_BUILD_COMMIT=${XENOIDE_BUILD_COMMIT})
target_compile_definitions(${target} PRIVATE XENOIDE_BUILD_TIMESTAMP=${XENOIDE_BUILD_TIMESTAMP})

target_link_libraries(${target} PRIVATE Qt6::Core Qt6::Widgets Qt6QScintilla)

add_custom_command(TARGET ${target} POST_BUILD 
    COMMAND Qt6::windeployqt
    ARGS $<TARGET_FILE:${target}>
)
