
set (target "xeno-wtl")
set (sources 
    "include/xeno/wtl/wtl.h"
    "include/xeno/wtl/TreeManager.h"
    "include/xeno/wtl/CodeEditor.h"
    "include/xeno/wtl/MenuFactory.h"
    
    "src/TreeManager.cpp"
    "src/CodeEditor.cpp"
    "src/MenuFactory.cpp"
)

add_library (${target} ${sources})
target_include_directories (${target} PUBLIC "include/")
target_link_libraries (${target} wtl xeno-scintilla xeno-ui Boost::filesystem)
