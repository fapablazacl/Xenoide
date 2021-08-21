
set (MAIN_NIBFILE "MainMenu")
set (PRINCIPAL_CLASS "NSApplication")

set (target Xenoide.Gui.Cocoa)

set (sources 
    AppDelegate.swift
    Window.swift
)

set (xibs
    "Base.lproj/MainMenu"
)

set (resources)

foreach(xib ${xibs})
    list(APPEND resources ${xib}.xib)
endforeach()

# message("CMAKE_SOURCE_DIR ${CMAKE_SOURCE_DIR}")
# message("CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}")

add_executable(${target} MACOSX_BUNDLE ${sources} ${resources})

target_sources(${target} PRIVATE Assets.xcassets)

set_source_files_properties(Assets.xcassets PROPERTIES
    MACOSX_PACKAGE_LOCATION Resources
)

set_target_properties(${target} PROPERTIES
    MACOSX_BUNDLE_INFO_PLIST                "${CMAKE_SOURCE_DIR}/modules/${target}/Info.plist.in"
    XCODE_ATTRIBUTE_INFOPLIST_PREPROCESS    YES
)

target_link_libraries(${target} "-framework Cocoa" Xenoide.Core Xenoide.Gui)

# prepare XIB's compilation ...
find_program(IBTOOL ibtool HINTS "/usr/bin" "${OSX_DEVELOPER_ROOT}/usr/bin")
if (${IBTOOL} STREQUAL "IBTOOL-NOTFOUND")
    message(SEND_ERROR "ibtool can not be found and is needed to compile the .xib files. It should have been installed with
                  the Apple developer tools. The default system paths were searched in addition to ${OSX_DEVELOPER_ROOT}/usr/bin")
endif()

# make sure the "Resources" directory is correctly created before we build the xibs
set (TARGET_RESOURCE_BUNDLE_DIR "${CMAKE_CURRENT_BINARY_DIR}/\${CONFIGURATION}/${target}.app/Contents/Resources")

add_custom_command (TARGET ${target} PRE_BUILD
                      COMMAND mkdir -p ${TARGET_RESOURCE_BUNDLE_DIR})

# issue a compilation for each XIB ...
set (XIB_PARENT_DIR "${CMAKE_SOURCE_DIR}/modules/${target}/")

foreach(xib ${xibs})
    add_custom_command (
        TARGET ${target} POST_BUILD
        COMMAND ${IBTOOL} --errors --warnings --notices --output-format human-readable-text
                              --compile ${TARGET_RESOURCE_BUNDLE_DIR}/${xib}.nib
                             ${XIB_PARENT_DIR}/${xib}.xib
        COMMENT "Compiling ${XIB_PARENT_DIR}/${xib}.xib"
    )
endforeach()
