cmake_minimum_required(VERSION 3.20.0)

# Project configuration
project(sciter-startup VERSION 4.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Platform-specific settings
if(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "Minimum OSX version" FORCE)
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "Build architectures for macOS" FORCE)
    set(CMAKE_XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH NO)
    set(CMAKE_XCODE_ATTRIBUTE_VALID_ARCHS "arm64 x86_64")
    set(CMAKE_INSTALL_RPATH "@executable_path/../Frameworks")
    set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)
elseif(WIN32)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    add_link_options(/MANIFEST:NO)
endif()

# Project options
option(LOCALMODE "Local mode" ON)
option(DEBUG "Debug" ON)
option(CONSOLE "Console mode" OFF)
option(ARCH "Target architecture" "x32")

# Version information
set(VERSION_MAJOR 2)
set(VERSION_MINOR 1)
set(VERSION_3RD 0)
file(STRINGS ${CMAKE_CURRENT_SOURCE_DIR}/BN.txt VERSION_BUILD)
message(STATUS "Building version: ${VERSION_BUILD}")

# Compiler definitions
add_definitions(-DUNICODE -D_UNICODE)
if(LOCALMODE)
    add_definitions(-DLOCAL_MODE)
endif()
if(DEBUG)
    add_definitions(-D_DEBUG)
endif()
if(CONSOLE)
    add_definitions(-DCONSOLE)
    if(WIN32)
        add_link_options(/SUBSYSTEM:CONSOLE /ENTRY:wWinMainCRTStartup)
    endif()
elseif(WIN32)
    add_link_options(/SUBSYSTEM:WINDOWS)
endif()

# Dependencies
set(SCITER_DIR "~/Downloads/sciter-js-sdk-5.0.3.11")
set(CMAKE_PREFIX_PATH ${SCITER_DIR}/include;${SCITER_DIR}/${SCITER_BIN};${CMAKE_PREFIX_PATH})

find_package(spdlog CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

# Source files
set(SRCLIST AppMain.cpp)

if(WIN32)
    enable_language(RC)
    list(APPEND SRCLIST "res/resource.rc")
    find_file(WIN_MAIN "sciter-win-main.cpp" NO_CACHE REQUIRED)
    list(APPEND SRCLIST ${WIN_MAIN})
    find_library(SCITER_LIBRARY sciter REQUIRED)
else()
    find_file(OSX_MAIN "sciter-osx-main.mm" NO_CACHE REQUIRED)
    list(APPEND SRCLIST ${OSX_MAIN})
    find_library(SCITER_LIBRARY sciter NAMES libsciter.dylib REQUIRED)
endif()

# Configure resource files
configure_file(res/resource.h.in ${CMAKE_CURRENT_SOURCE_DIR}/res/resource.h)
configure_file(res/info.plist.in ${CMAKE_CURRENT_SOURCE_DIR}/res/info.plist)

# Library dependencies
set(LIBLIST 
    spdlog::spdlog_header_only 
    fmt::fmt-header-only
)

if(APPLE)
    list(APPEND LIBLIST 
        "-framework Foundation" 
        "-framework Cocoa" 
        "-framework AppKit" 
        "-framework CoreData"
    )
endif()

# Build target
if(APPLE)
    set(MACOSX_BUNDLE_ICON_FILE n8ap.icns)
    set(myApp_ICON ${CMAKE_CURRENT_SOURCE_DIR}/res/gallery.icns)
    set_source_files_properties(${myApp_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

    add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${myApp_ICON} ${SRCLIST})
    set_target_properties(${PROJECT_NAME} PROPERTIES
        BUNDLE True
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.nemoinfo.${PROJECT_NAME}"
        MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME}
        MACOSX_BUNDLE_BUNDLE_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_3RD}"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_3RD}"
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/res/info.plist
        XCODE_ATTRIBUTE_ARCHS "arm64;x86_64"
        XCODE_ATTRIBUTE_VALID_ARCHS "arm64 x86_64"
        XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH NO
    )
else()
    add_executable(${PROJECT_NAME} ${SRCLIST})
endif()

# Add include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${SCITER_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBLIST})

# Installation
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/../bin/${ARCH})
cmake_path(NORMAL_PATH CMAKE_INSTALL_PREFIX)
set(RUNTIMEDIR ${CMAKE_INSTALL_PREFIX})

if(APPLE)
    install(TARGETS ${PROJECT_NAME}
        RUNTIME_DEPENDENCIES
        BUNDLE DESTINATION .
        RUNTIME DESTINATION .
        FRAMEWORK DESTINATION .
        LIBRARY DESTINATION .
    )
    install(FILES ${SCITER_LIBRARY} 
        DESTINATION "${PROJECT_NAME}.app/Contents/Frameworks"
    )
else()
    install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION .
        FRAMEWORK DESTINATION .
        LIBRARY DESTINATION .
    )
    install(FILES ${SCITER_LIBRARY} 
        DESTINATION ${RUNTIMEDIR}
    )
endif()

if(LOCALMODE)
    install(DIRECTORY ui
        DESTINATION ${RUNTIMEDIR}
        CONFIGURATIONS
    )
endif()
