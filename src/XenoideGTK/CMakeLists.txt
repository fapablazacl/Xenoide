
#
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM gtkmm-3.0 gtksourceviewmm-3.0 REQUIRED)

#
add_definitions(${GTK4_CFLAGS_OTHER})

# target 
set(target "XenoideGTK")

set(sources 
    "src/main.cpp"
    src/DocumentHeader.h     
    src/DocumentHeader.cpp      
    src/DocumentManagerGtk.h  
    src/DocumentManagerGtk.cpp  
    src/DocumentGtk.h    
    src/DocumentGtk.cpp  
    src/FolderBrowserGtk.h    
    src/FolderBrowserGtk.cpp  
    src/MainApplication.h    
    src/MainApplication.cpp  
    src/MainWindow.h
    src/MainWindow.cpp
    src/main.cpp
)

add_executable(${target} ${sources})

#
target_include_directories(${target} PUBLIC ${GTKMM_INCLUDE_DIRS})
target_link_directories(${target} PUBLIC ${GTKMM_LIBRARY_DIRS})
target_link_libraries(${target} ${GTK4_LIBRARIES} ${GTKMM_LIBRARIES})
