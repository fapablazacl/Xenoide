if (XENOIDE_BUILD_POC_GTKCLASSBROWSER)
    add_subdirectory ("pocs/gtk-class-browser")
endif()

if (XENOIDE_BUILD_POC_GUIMVP)
    add_subdirectory ("pocs/gui-mvp")
endif ()

if (XENOIDE_BUILD_POC_GUIWX)
    add_subdirectory ("pocs/gui-wx")
endif ()

if (XENOIDE_BUILD_POC_QT5SCINTILLAPOC)
    add_subdirectory ("pocs/qt5-scintilla-poc")
endif ()

if (XENOIDE_BUILD_POC_SANDBOX)
    add_subdirectory ("pocs/sandbox")
endif ()
