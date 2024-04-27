cmake_minimum_required(VERSION 3.15)
project(Xenoide)

set(CMAKE_CXX_STANDARD 17)

# Find the required Qt6 packages
find_package(Qt6 COMPONENTS Core Widgets REQUIRED)

# Add the source files of your project
add_executable(Xenoide main.cpp)

# Link against the Qt6 libraries
target_link_libraries(Xenoide PRIVATE Qt6::Core Qt6::Widgets)
