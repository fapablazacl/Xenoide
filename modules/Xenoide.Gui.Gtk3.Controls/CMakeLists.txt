
# depends on GTKmm 3
find_package (PkgConfig)
pkg_check_modules (GTKMM gtkmm-3.0 gtksourceviewmm-3.0)

# target definition
set (target "Xenoide.Gui.Gtk3.Controls")

set (sources
    "include/Xenoide/Gui/Gtk3/Controls/Header.h"

    "src/Header.cpp"
)

add_library (${target} ${sources})
target_include_directories (${target} PUBLIC "include")
target_include_directories (${target} PUBLIC ${GTKMM_INCLUDE_DIRS})
link_directories (${GTKMM_LIBRARY_DIRS})
target_link_libraries (${target} ${GTKMM_LIBRARIES} Xenoide.Core)
