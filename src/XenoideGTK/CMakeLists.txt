
#
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM gtkmm-3.0 gtksourceviewmm-3.0 REQUIRED)

#
add_definitions(${GTK4_CFLAGS_OTHER})

# target 
set(target "XenoideGTK")

set(sources 
    "src/main.cpp"
    "src/document-manager/DocumentHeader.h"
    "src/document-manager/DocumentHeader.cpp"
    "src/document-manager/DocumentManagerGtk.h"
    "src/document-manager/DocumentManagerGtk.cpp"
    "src/document-manager/DocumentGtk.h"
    "src/document-manager/DocumentGtk.cpp"
    "src/folder-browser/FolderBrowserGtk.h"
    "src/folder-browser/FolderBrowserGtk.cpp"
    "src/MainApplication.h"
    "src/MainApplication.cpp"
    "src/MainWindow.h"
    "src/MainWindow.cpp"
)

add_executable(${target} ${sources})

# Core 
target_include_directories(${target} PRIVATE "include")
target_link_libraries(${target} xeno-core xeno-ui)

# GTK
target_include_directories(${target} PUBLIC ${GTKMM_INCLUDE_DIRS})
target_link_directories(${target} PUBLIC ${GTKMM_LIBRARY_DIRS})
target_link_libraries(${target} ${GTK4_LIBRARIES} ${GTKMM_LIBRARIES})
