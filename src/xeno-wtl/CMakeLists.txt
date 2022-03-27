
set (target "xeno-wtl")
set (sources )

add_library(${target} ${sources})
target_link_libraries(${target} PUBLIC "include/")
