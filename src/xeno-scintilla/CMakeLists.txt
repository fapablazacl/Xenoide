
set (target "xeno-scintilla")
set (sources 
    "include/LexillaAccess.h" 
    "src/LexillaAccess.cxx"
)

add_library (${target} ${sources})
target_include_directories (${target} PUBLIC "include/")
target_link_libraries (${target} Scintilla Lexilla)
