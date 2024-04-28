cmake_minimum_required(VERSION 3.25)

project (Xenoide)

# version
option (XENOIDE_VERSION_MAJOR "Xenoide major version, for breaking feature changes" 0)
option (XENOIDE_VERSION_MINOR "Xenoide minor version, for non-breaking feature changes" 0)
option (XENOIDE_VERSION_REVISION "Xenoide bugfix number" 0)

# build tracking information
option (XENOIDE_BUILD_COMMIT "Git hash used to generate this build" "local")
option (XENOIDE_BUILD_TIMESTAMP "When this build was made, in YYYYmmDDHHMMSS format (if set)" "now")

# Qt packages
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

# Qt5QScintilla target
add_library(Qt5QScintilla INTERFACE)
target_include_directories(Qt5QScintilla INTERFACE "${Qt5_DIR}/../../../include")
target_link_directories(Qt5QScintilla INTERFACE "${Qt5_DIR}/../../")
target_link_libraries(Qt5QScintilla INTERFACE qscintilla2_qt5)

# Xenoide executable
set (target Xenoide)
set (sources main.cpp)

add_executable(${target} ${sources})

target_compile_definitions(${target} PRIVATE XENOIDE_VERSION_MAJOR=${XENOIDE_VERSION_MAJOR})
target_compile_definitions(${target} PRIVATE XENOIDE_VERSION_MINOR=${XENOIDE_VERSION_MINOR})
target_compile_definitions(${target} PRIVATE XENOIDE_VERSION_REVISION=${XENOIDE_VERSION_REVISION})
target_compile_definitions(${target} PRIVATE XENOIDE_BUILD_COMMIT=${XENOIDE_BUILD_COMMIT})
target_compile_definitions(${target} PRIVATE XENOIDE_BUILD_TIMESTAMP=${XENOIDE_BUILD_TIMESTAMP})

target_link_libraries(${target} PRIVATE Qt5::Core Qt5::Widgets Qt5QScintilla)
