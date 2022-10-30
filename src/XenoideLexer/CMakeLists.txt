
set (target XenoideLexer)
set (source 
    "include/Xenoide/Lexer.h"
    "src/Lexer.cpp"
)

add_library(${target} ${source})
target_link_libraries(${target} xeno-scintilla)
