
# depends on GTKmm 3
find_package (PkgConfig)
pkg_check_modules (GTKMM gtkmm-3.0 gtksourceviewmm-3.0)
#pkg_check_modules (GTKMM gtkmm-3.0 gtksourceviewmm-3.0 gdlmm-3.0)

# target definition
set (target "Xenoide.Gui.Gtk3")

set (sources
    "include/Xenoide/Gui/Gtk3/MainApplication.hpp"
    "include/Xenoide/Gui/Gtk3/IDEFrameGtk.hpp"
    "include/Xenoide/Gui/Gtk3/DialogManagerGtk.hpp"
    "include/Xenoide/Gui/Gtk3/FolderBrowserGtk.hpp"
    "include/Xenoide/Gui/Gtk3/DocumentGtk.hpp"
    "include/Xenoide/Gui/Gtk3/DocumentManagerGtk.hpp"
    "include/Xenoide/Gui/Gtk3/DocumentHeader.hpp"

    "src/MainApplication.cpp"
    "src/IDEFrameGtk.cpp"
    "src/DialogManagerGtk.cpp"
    "src/FolderBrowserGtk.cpp"
    "src/DocumentGtk.cpp"
    "src/DocumentManagerGtk.cpp"
    "src/DocumentHeader.cpp"
    "src/Main.cpp"
)

message(WARN "GTKMM_INCLUDE_DIRS: " ${GTKMM_INCLUDE_DIRS})
message(WARN "GTKMM_LIBRARY_DIRS: " ${GTKMM_LIBRARY_DIRS})
message(WARN "GTKMM_LIBRARIES: " ${GTKMM_LIBRARIES})

add_executable (${target} ${sources})
target_include_directories (${target} PUBLIC "include")
target_include_directories (${target} PRIVATE ${GTKMM_INCLUDE_DIRS})
link_directories (${GTKMM_LIBRARY_DIRS})
target_link_libraries (${target} ${GTKMM_LIBRARIES} Xenoide.Core Xenoide.Gui stdc++fs)
