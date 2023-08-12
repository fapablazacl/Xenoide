
set (target "Xenoide.Sandbox")

set (sources 
    "sandbox.cpp"
	"stdafx.cpp"
	"stdafx.h"
	"Resource.rc"
	"resource.h"
)

add_executable (${target} WIN32 ${sources})

target_include_directories (${target} PUBLIC ${XENOIDE_WTL_INCLUDE_DIR})
target_include_directories (${target} PUBLIC "res")

install(TARGETS ${target} DESTINATION "bin")
