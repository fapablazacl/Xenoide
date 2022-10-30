
# IDE Core
add_subdirectory ("xeno-core")
add_subdirectory ("xeno-ui")

# WTL UI 
if (XENOIDE_BUILD_UI_WTL)
    add_subdirectory ("XenoideLexer")
    add_subdirectory ("xeno-scintilla")
    add_subdirectory ("xeno-wtl")
    add_subdirectory ("XenoideWTL")
    add_subdirectory ("XenoshaderWTL")
endif ()

# GTK UI
if (XENOIDE_BUILD_UI_GTK)
    add_subdirectory ("XenoideGTK")
endif ()

# build system
add_subdirectory ("core")
add_subdirectory ("libxenocli")
add_subdirectory ("xenocli")
add_subdirectory ("Xenobuild")
