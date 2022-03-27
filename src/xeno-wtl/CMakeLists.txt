
set (target "xeno-wtl")
set (sources 
    "include/xeno/wtl/TreeManager.h"
    "include/xeno/wtl/CodeView.h"
    
    "src/TreeManager.cpp"
    "src/CodeView.cpp"
)

add_library (${target} ${sources})
target_include_directories (${target} PUBLIC "include/")
target_link_libraries (${target} wtl xeno-scintilla)
