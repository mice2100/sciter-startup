cmake_minimum_required(VERSION 3.20.0)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "Minimum OSX version" FORCE)
project(n8photoslide VERSION 4.0)

OPTION(LOCALMODE "loacal mode" ON)
OPTION(DEBUG "Debug" ON)
OPTION(CONSOLE "console mode" OFF)
OPTION(ARCH "target OS" "x32")

SET(MYTARGET "n8photoslide")
SET(VERSION_MAJOR 0)
SET(VERSION_MINOR 1)
SET(VERSION_3RD 0)

if(LOCALMODE)
    ADD_DEFINITIONS(-DLOCAL_MODE)
endif()
if(DEBUG)
    ADD_DEFINITIONS(-D_DEBUG)
endif()
if(CONSOLE)
    ADD_DEFINITIONS(-DCONSOLE)
    if(WIN32)
        add_link_options(/SUBSYSTEM:CONSOLE /ENTRY:wWinMainCRTStartup)
    endif()
else()
    if(WIN32)
        add_link_options(/SUBSYSTEM:WINDOWS)
    endif()
endif()

ADD_DEFINITIONS(-DUNICODE -D_UNICODE)

SET(SCITER_DIR "C:/Users/george/Downloads/sciter-js-sdk-5.0.3.15")
SET(CMAKE_PREFIX_PATH ${SCITER_DIR}/include;${SCITER_DIR}/${SCITER_BIN};${CMAKE_PREFIX_PATH})

find_package(spdlog CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

SET(LIBLIST spdlog::spdlog_header_only fmt::fmt-header-only)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED True)

SET(SRCLIST AppMain.cpp)

# store dll file name which needs to install
SET(TMPDLLLIST)

IF(WIN32)
    add_link_options(/MANIFEST:NO)
    ENABLE_LANGUAGE(RC)
    SET(RES_FILES "res/resource.rc")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    
    find_file(TMPNAME2 name "sciter-win-main.cpp" NO_CACHE REQUIRED)
    LIST(APPEND SRCLIST ${TMPNAME2})

    LIST(APPEND TMPDLLLIST "sciter.dll")
ELSE()
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl")
    SET(RES_FILES ${CMAKE_CURRENT_SOURCE_DIR}/ui_res/images/n8ap.icns)
    find_file(TMPNAME name "sciter-osx-main.mm" NO_CACHE REQUIRED)
    LIST(APPEND SRCLIST ${TMPNAME})

    LIST(APPEND TMPDLLLIST "libsciter.dylib")
    LIST(APPEND LIBLIST "-framework Foundation" "-framework Cocoa" "-framework AppKit" "-framework CoreData")

ENDIF()


file(STRINGS ${CMAKE_CURRENT_SOURCE_DIR}/BN.txt VERSION_BUILD)
message("building version: ${VERSION_BUILD}")
configure_file(res/resource.h.in ${CMAKE_CURRENT_SOURCE_DIR}/res/resource.h)
configure_file(res/info.plist.in ${CMAKE_CURRENT_SOURCE_DIR}/res/info.plist)
include_directories(${SCITER_DIR}/include)

add_executable(${MYTARGET} ${SRCLIST} ${RES_FILES})
target_link_libraries(${MYTARGET} PRIVATE ${LIBLIST}  )

SET(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/../bin/${ARCH})
CMAKE_PATH(NORMAL_PATH CMAKE_INSTALL_PREFIX)
SET(RUNTIMEDIR ${CMAKE_INSTALL_PREFIX})
if(APPLE)
    SET(RUNTIMEDIR ${CMAKE_INSTALL_PREFIX}/${MYTARGET}.app/Contents/MacOS)
endif()

# get full file name and push into dlllist
SET(DLLLIST)
foreach(tmp ${TMPDLLLIST})
    unset(TMPFULL)
    find_program(TMPFULL name ${tmp} NO_CACHE REQUIRED)
    LIST(APPEND DLLLIST ${TMPFULL})
endforeach()

if(APPLE)
    INSTALL(
        TARGETS ${MYTARGET}
        RUNTIME_DEPENDENCIES
        BUNDLE DESTINATION .
        RUNTIME DESTINATION .
        FRAMEWORK DESTINATION .
        LIBRARY DESTINATION .
    )
else()
    INSTALL(
        TARGETS ${MYTARGET}
        RUNTIME DESTINATION .
        FRAMEWORK DESTINATION .
        LIBRARY DESTINATION .
    )
endif()

INSTALL(PROGRAMS ${DLLLIST} DESTINATION ${RUNTIMEDIR} )

if(LOCALMODE)
    INSTALL(
        DIRECTORY ui
        DESTINATION ${RUNTIMEDIR}
        CONFIGURATIONS
    )
endif()

if(APPLE)
    set(APPS "\${CMAKE_INSTALL_PREFIX}/${MYTARGET}.app")
    install(CODE "include(BundleUtilities)
        fixup_bundle(\"${APPS}\" \"\" \"${CMAKE_PREFIX_PATH}\")")

    # set(CPACK_GENERATOR "DRAGNDROP")
    # include(CPack)
endif()