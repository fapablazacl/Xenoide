
set (target XenoideLexerCPP)
set (source 
    "include/Xenoide/LexerCPP.h"
    "src/LexerCPP.cpp"
)

add_library(${target} ${source})
target_link_libraries(${target} XenoideLexer)
