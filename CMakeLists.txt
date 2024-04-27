# Find the required Qt5 packages
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

# Find the QScintilla package
find_package(QScintilla REQUIRED)

# Add the source files of your project
add_executable(Xenoide main.cpp)

# Link against the Qt5 and QScintilla libraries
target_link_libraries(Xenoide PRIVATE Qt5::Core Qt5::Widgets QScintilla::QScintilla)
