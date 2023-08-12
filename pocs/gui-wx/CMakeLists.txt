

find_package(wxWidgets
    COMPONENTS
        core base

    OPTIONAL_COMPONENTS
        stc

    REQUIRED
)

set (target "gui-wx")
set (sources
    "src/MainFrame.h"
    "src/MainFrame.cpp"
    "src/MainApp.h"
    "src/MainApp.cpp"
)

add_executable (${target} ${sources})

add_compile_options(${wxWidgets_CXX_FLAGS})

target_compile_definitions (${target} PRIVATE ${wxWidgets_DEFINITIONS})
target_include_directories (${target} PRIVATE ${wxWidgets_INCLUDE_DIRS})
target_link_directories (${target} PRIVATE ${wxWidgets_LIBRARY_DIRS})

target_link_libraries (
    ${target} 
    Xenoide.Core
    Xenoide.Gui
    ${wxWidgets_LIBRARIES}
)
