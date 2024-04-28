cmake_minimum_required(VERSION 3.25)

project (Xenoide)

# version
option (Xenoide_VERSION_MAJOR "Xenoide major version, for breaking feature changes" 0)
option (Xenoide_VERSION_MINOR "Xenoide minor version, for non-breaking feature changes" 0)
option (Xenoide_VERSION_REVISION "Xenoide bugfix number" 0)

# build tracking information
option (Xenoide_BUILD_COMMIT "Git hash used to generate this build" "local")
option (Xenoide_BUILD_TIMESTAMP "When this build was made, in YYYYmmDDHHMMSS format (if set)" "now")

# Qt dependencies
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
find_package(QScintilla REQUIRED)

# Xenoide executable
set (target Xenoide)
set (sources main.cpp)

add_executable(${target} ${sources})

target_compile_definitions(${target} PRIVATE XENOIDE_VERSION_MAJOR=${Xenoide_VERSION_MAJOR})
target_compile_definitions(${target} PRIVATE XENOIDE_VERSION_MINOR=${Xenoide_VERSION_MINOR})
target_compile_definitions(${target} PRIVATE XENOIDE_VERSION_REVISION=${Xenoide_VERSION_REVISION})
target_compile_definitions(${target} PRIVATE XENOIDE_BUILD_COMMIT=${Xenoide_BUILD_COMMIT})
target_compile_definitions(${target} PRIVATE XENOIDE_BUILD_TIMESTAMP=${Xenoide_BUILD_TIMESTAMP})

target_link_libraries(${target} PRIVATE Qt5::Core Qt5::Widgets QScintilla::QScintilla)
