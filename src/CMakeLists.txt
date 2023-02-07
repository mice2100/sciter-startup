cmake_minimum_required(VERSION 3.20.0)
project(sciter-starter VERSION 3.0)

OPTION(LOCALMODE "loacal mode" ON)
OPTION(DEBUGWINDOW "Debug Window" ON)
OPTION(ARCH "target OS" "x32")

SET(MYTARGET "sciterstarter")

SET(VERSION_MAJOR 1)
SET(VERSION_MINOR 0)
SET(VERSION_3RD 0)

if(LOCALMODE)
    message("local mode")
    ADD_DEFINITIONS(-DLOCAL_MODE)
endif()
if(DEBUGWINDOW)
    message("debug window")
    ADD_DEFINITIONS(-DDEBUG_WINDOW)
elseif(WIN32)
    add_link_options(-mwindows)
endif()

if(APPLE)
    unset(CMAKE_OSX_ARCHITECTURES)
    UNSET(CMAKE_SYSTEM_PROCESSOR)
    UNSET(CMAKE_OSX_DEPLOYMENT_TARGET)
    set(CMAKE_MACOSX_RPATH ON)
    set(CMAKE_OSX_ARCHITECTURES "x86_64" CACHE STRING "Architectures me" FORCE)
    set(CMAKE_SYSTEM_PROCESSOR "x86_64" CACHE STRING "System Processor me" FORCE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.9" CACHE STRING "Minimum OSX version" FORCE)
else()
    add_link_options(-static -static-libgcc -static-libstdc++)
endif()

file(STRINGS ${CMAKE_CURRENT_SOURCE_DIR}/BN.txt VERSION_BUILD)
message("building version: ${VERSION_BUILD}")

SET(VENDORS ${CMAKE_CURRENT_SOURCE_DIR}/../vendors/${ARCH})
SET(CMAKE_PREFIX_PATH ${VENDORS};${VENDORS}/lib;${VENDORS}/../include;${CMAKE_PREFIX_PATH})

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_STANDARD_REQUIRED True)

SET(SRCLIST AppMain.cpp)

# store dll file name which needs to install
SET(TMPDLLLIST)

IF(WIN32)
    ENABLE_LANGUAGE(RC)
    SET(RES_FILES "res/resource.rc")

    find_file(TMPNAME name "sciter-win-main.cpp" NO_CACHE REQUIRED)
    LIST(APPEND SRCLIST ${TMPNAME})

    LIST(APPEND TMPDLLLIST "sciter.dll")
ELSE()
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl")
    find_file(TMPNAME name "sciter-osx-main.mm" NO_CACHE REQUIRED)
    LIST(APPEND SRCLIST ${TMPNAME})

    LIST(APPEND TMPDLLLIST "libsciter.dylib")
    LIST(APPEND LIBLIST "-framework Foundation" "-framework Cocoa" "-framework AppKit" "-framework CoreData")
ENDIF()

if(MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "-fuse-ld=lld")
    set(CMAKE_MODULE_LINKER_FLAGS "-fuse-ld=lld")
    set(CMAKE_SHARED_LINKER_FLAGS "-fuse-ld=lld")
    add_compile_options(-w -DUNICODE -D_UNICODE)
    add_link_options(-municode)
endif()

ADD_DEFINITIONS(-DUNICODE -D_GLIBCXX_USE_CXX11_ABI=0)
configure_file(res/resource.h.in ${CMAKE_CURRENT_SOURCE_DIR}/res/resource.h)
configure_file(res/info.plist.in ${CMAKE_CURRENT_SOURCE_DIR}/res/info.plist)
include_directories( $<BUILD_INTERFACE:${VENDORS}/../include>)
link_directories($<BUILD_INTERFACE:${VENDORS}/lib> $<BUILD_INTERFACE:${CMAKE_FIND_ROOT_PATH}/lib> )

if(APPLE)
    set(MACOSX_BUNDLE_ICON_FILE n8ap.icns)
    set(myApp_ICON ${CMAKE_CURRENT_SOURCE_DIR}/res/gallery.icns)
    set_source_files_properties(${myApp_ICON} PROPERTIES  MACOSX_PACKAGE_LOCATION "Resources")

    add_executable(${MYTARGET} MACOSX_BUNDLE ${myApp_ICON} ${SRCLIST})
    set_target_properties(${MYTARGET} PROPERTIES
        BUNDLE True
        MACOSX_BUNDLE_GUI_IDENTIFIER com.moosephotoprint.${MYTARGET}
        MACOSX_BUNDLE_BUNDLE_NAME ${MYTARGET}
        MACOSX_BUNDLE_BUNDLE_VERSION ${VERSION_MAJOR}.${VERSION_Minor}.${VERSION_3RD}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${VERSION_MAJOR}.${VERSION_Minor}.${VERSION_3RD}
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/res/Info.plist
        XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH "YES"
    )
else()
    add_executable(${MYTARGET} ${SRCLIST} ${RES_FILES})
endif()
    
target_link_libraries(${MYTARGET} ${LIBLIST}  )

SET(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/../bin)
SET(RUNTIMEDIR ${CMAKE_INSTALL_PREFIX}/${ARCH}-${MYTARGET})
SET(DLLDIR ${CMAKE_INSTALL_PREFIX}/${ARCH}-${MYTARGET})
if(APPLE)
    SET(DLLDIR ${RUNTIMEDIR}/${MYTARGET}.app/Contents/MacOS)
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
    BUNDLE DESTINATION ${RUNTIMEDIR}
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

if(APPLE)
    install(CODE "
        include(BundleUtilities)
        fixup_bundle(\"${DLLDIR}/${MYTARGET}\" \"\" \"\")
    ")
endif()

if(LOCALMODE)
    INSTALL(
        DIRECTORY ui
        DESTINATION ${DLLDIR}
        CONFIGURATIONS
    )
endif()