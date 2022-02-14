cmake_minimum_required(VERSION 3.20.0)

if(APPLE)
    unset(CMAKE_OSX_ARCHITECTURES)
    UNSET(CMAKE_SYSTEM_PROCESSOR)
    UNSET(CMAKE_OSX_DEPLOYMENT_TARGET)
    set(CMAKE_OSX_ARCHITECTURES "x86_64" CACHE STRING "Architectures me" FORCE)
    set(CMAKE_SYSTEM_PROCESSOR "x86_64" CACHE STRING "System Processor me" FORCE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.9" CACHE STRING "Minimum OSX version" FORCE)
    set(CMAKE_FIND_ROOT_PATH "/usr/local")
    set(CMAKE_OSX_SYSROOT "/Library/Developer/CommandLineTools/SDKs/MacOSX10.10.sdk")
    # set(CMAKE_GENERATOR "Ninja")
endif()
project(sciter-startup VERSION 1.0)


OPTION(devmode "debug mode" OFF)

SET(VERSION_MAJOR 3)
SET(VERSION_MINOR 0)
SET(VERSION_3RD 2)

file(STRINGS ${CMAKE_CURRENT_SOURCE_DIR}/BN.txt VERSION_BUILD)
message("building version: ${VERSION_BUILD}")

if(WIN32)
    if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86")
        SET(arch x32)
    else()
        SET(arch x64)
    endif()
    # SET(CMAKE_FIND_ROOT_PATH ${VENDORS}/bin;${CMAKE_FIND_ROOT_PATH})

    ENABLE_LANGUAGE(RC)
    SET(RES_FILES "resource.rc")
    SET(INCREASEBN increasebn.bat)
elseif(APPLE)
    SET(arch osx)
    SET(INCREASEBN increasebn.sh)
endif()

SET(VENDORS ${CMAKE_CURRENT_SOURCE_DIR}/third-party/dist/${arch})
SET(CMAKE_PREFIX_PATH ${VENDORS};${CMAKE_PREFIX_PATH})

# find_package(spdlog CONFIG REQUIRED)

if(WIN32)
    # find_library(freeimage name freeimage REQUIRED)
else()
endif()

# SET(LIBLIST pthread spdlog::spdlog_header_only freeimage)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED True)

SET(SRCLIST src/AppMain.cpp)

# store dll file name which needs to install
SET(TMPDLLLIST)

IF(WIN32)
    LIST(APPEND TMPDLLLIST "sciter.dll")
    find_file(TMPNAME name "sciter-win-main.cpp" NO_CACHE REQUIRED)
    LIST(APPEND SRCLIST ${TMPNAME})
ELSE()
    find_file(TMPNAME name "sciter-osx-main.mm" NO_CACHE REQUIRED)
    LIST(APPEND SRCLIST ${TMPNAME})

    LIST(APPEND TMPDLLLIST "libsciter.dylib")
ENDIF()

# add_link_options(-municode)

if(WIN32)
  add_link_options( -static-libgcc -static-libstdc++ -fuse-ld=lld)
endif()

ADD_DEFINITIONS(-DFIP_EXPORTS -DUNICODE -D_GLIBCXX_USE_CXX11_ABI=0)

if(MINGW)
    add_compile_options(-w)
    add_link_options(-municode)
endif()

if(${devmode})
    ADD_DEFINITIONS(-DDEV_MODE)
else()
    add_link_options(-mwindows)
endif()

SET(MYTARGET sciter-startup)

configure_file(resource.h.in ${CMAKE_CURRENT_SOURCE_DIR}/resource.h)
configure_file(info.plist.in ${CMAKE_CURRENT_SOURCE_DIR}/info.plist)

include_directories( $<BUILD_INTERFACE:${VENDORS}/include> )
link_directories($<BUILD_INTERFACE:${VENDORS}/lib> $<BUILD_INTERFACE:${CMAKE_FIND_ROOT_PATH}/lib> )

if(WIN32)
    add_executable(${MYTARGET} ${SRCLIST} ${RES_FILES})
    target_link_libraries(${MYTARGET} ${LIBLIST})
    if(MSVC)
        target_link_options(${MYTARGET} PRIVATE /ENTRY:wWinMainCRTStartup)
    endif()
elseif(APPLE)
    set(MACOSX_BUNDLE_ICON_FILE gallery.icns)
    set(myApp_ICON ${CMAKE_CURRENT_SOURCE_DIR}/gallery.icns)
    set_source_files_properties(${myApp_ICON} PROPERTIES  MACOSX_PACKAGE_LOCATION "Resources")

    add_executable(${MYTARGET} MACOSX_BUNDLE ${myApp_ICON} ${SRCLIST})
    set_target_properties(${MYTARGET} PROPERTIES
        BUNDLE True
        MACOSX_BUNDLE_GUI_IDENTIFIER com.moosephotoprint.${MYTARGET}
        MACOSX_BUNDLE_BUNDLE_NAME ${MYTARGET}
        MACOSX_BUNDLE_BUNDLE_VERSION ${VERSION_MAJOR}.${VERSION_Minor}.${VERSION_3RD}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${VERSION_MAJOR}.${VERSION_Minor}.${VERSION_3RD}
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist
        XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH "YES"
    )
    target_link_libraries(${MYTARGET} ${LIBLIST} "-framework Foundation" "-framework Cocoa" "-framework AppKit" "-framework CoreData")
endif()

add_custom_command(TARGET ${MYTARGET} PRE_BUILD COMMAND ./${INCREASEBN} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# typical bin/x64/nemobook/nembook.exe
SET(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/bin/${arch})
if(WIN32)
    SET(RUNTIMEDIR ${CMAKE_INSTALL_PREFIX}/${MYTARGET})
    SET(DLLDIR ${CMAKE_INSTALL_PREFIX}/${MYTARGET})
elseif(APPLE)
    SET(RUNTIMEDIR ${CMAKE_INSTALL_PREFIX}/${MYTARGET}/${MYTARGET}.app/Contents/MacOS)
    SET(DLLDIR ${RUNTIMEDIR})
endif()

# get full file name and push into dlllist
SET(DLLLIST)
foreach(tmp ${TMPDLLLIST})
    unset(TMPFULL)
    find_program(TMPFULL name ${tmp} NO_CACHE REQUIRED)
    LIST(APPEND DLLLIST ${TMPFULL})
endforeach()


INSTALL(
    TARGETS ${MYTARGET}
    BUNDLE DESTINATION ${CMAKE_INSTALL_PREFIX}/${MYTARGET}
    RUNTIME DESTINATION ${RUNTIMEDIR}
    FRAMEWORK DESTINATION ${RUNTIMEDIR}
)

install(CODE "set(DLLLIST \"${DLLLIST}\")
    set(DLLDIR \"${DLLDIR}\")
    ")

install(CODE [[
    foreach(_file ${DLLLIST})
        message("${_file}")
        file(INSTALL
        DESTINATION ${DLLDIR}
        TYPE SHARED_LIBRARY
        FOLLOW_SYMLINK_CHAIN
        FILES "${_file}"
        )
    endforeach()
]])

# if(APPLE)
#     install(CODE "
#         include(BundleUtilities)
#         fixup_bundle(\"${RUNTIMEDIR}/nemobook\" \"\" \"\")
#     ")
# endif()

INSTALL(
    DIRECTORY src/ui
    DESTINATION ${RUNTIMEDIR}
    CONFIGURATIONS Debug
)
