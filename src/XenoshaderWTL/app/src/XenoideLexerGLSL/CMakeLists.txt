
set (target XenoideLexerGLSL)
set (source 
    "include/Xenoide/LexerGLSL.h"
    "src/LexerGLSL.cpp"
)

add_library(${target} ${source})
target_link_libraries(${target} XenoideLexer)
