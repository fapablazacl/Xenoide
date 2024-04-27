cmake_minimum_required(VERSION 3.15)
project(Xenoide)

set(CMAKE_CXX_STANDARD 17)

# Find the required Qt5 packages
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

# Add the source files of your project
add_executable(Xenoide main.cpp)

# Link against the Qt5 libraries
target_link_libraries(Xenoide PRIVATE Qt5::Core Qt5::Widgets)
