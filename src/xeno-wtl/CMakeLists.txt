
set (target "xeno-wtl")
set (sources 
    "include/xeno/wtl/TreeManager.h"
    "include/xeno/wtl/CodeEditor.h"
    
    "src/TreeManager.cpp"
    "src/CodeEditor.cpp"
)

add_library (${target} ${sources})
target_include_directories (${target} PUBLIC "include/")
target_link_libraries (${target} wtl xeno-scintilla Boost::filesystem)
