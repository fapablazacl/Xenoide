
set (target "gui-mvp")

set (sources
    "include/xeno/ui/mvp/MainFrame.hpp"
	"include/xeno/ui/mvp/DocumentManager.hpp"
	"include/xeno/ui/mvp/Document.hpp"
	"include/xeno/ui/mvp/FindReplace.hpp"

    "src/MainFrame.cpp"
    "src/DocumentManager.cpp"
    "src/Document.cpp"
    "src/FindReplace.cpp"
)

add_library (${target} ${sources})

target_include_directories(${target} PUBLIC "include")

target_link_libraries (
    ${target} 
    ${DL_LIBRARY}
    ${Boost_LIBRARIES}
    Xenoide.Core
	Xenoide.Gui
)
