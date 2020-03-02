#[[ A specially curated CMAKE file designed to handle
    importing of all the necessary libraries to run ASGE games.
    It's important to note that due to the way CMake operates,
    static libraries requires linking to all dependencies in
    the build chain. Using shared, only requires the use of a
    single DLL.

    Shared Build    => Executable
                    => GameEngine.dll
                    => GCC runtime dlls

    Static Build    => Executable   static app
                    => OpenGL       static library, provided by OS
                    => GLEW         static library
                    => GLFW         static library
                    => Freetype     static library
                    => PhysFS       static library
                    => PhysFS++     static library
                    => GameEngine   static library

    NB: You should not need to edit this file, unless adding new libraries to your game ]]

message( "##### ASGE SETUP #####")
add_library(ASGE UNKNOWN IMPORTED)
set(ASGE_VERSION "2.0.0-alpha")
set(ASGE_VERSION_EXTRA "+2")
set(ASGE_BASEURL "https://github.com/HuxyUK/ASGE/releases/download/v${ASGE_VERSION}")

if(NOT ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug"))
    set(ASGE_URL "${ASGE_BASEURL}/ASGE-${ASGE_VERSION}${ASGE_VERSION_EXTRA}-${CMAKE_CXX_PLATFORM_ID}.zip")
else()
    set(ASGE_URL "${ASGE_BASEURL}/ASGE-devel-${ASGE_VERSION}${ASGE_VERSION_EXTRA}-${CMAKE_CXX_PLATFORM_ID}.zip")
endif()

message(${ASGE_URL})

## download ASGE ##
include(FetchContent)
FetchContent_Declare(
        ASGE
        URL      ${ASGE_URL}
       # URL_HASH MD5=67855acdfb6dcc922bd0c08add137e75
        SOURCE_DIR _deps/asge-src
)

FetchContent_GetProperties(ASGE)
if(NOT asge_POPULATED)
    FetchContent_Populate(asge)
endif()

## set the library directories ##
if(WIN32)
    set(ASGE_BIN_DIRECTORY ${asge_SOURCE_DIR}/bin)
    set(ASGE_LIB_DIRECTORY ${asge_SOURCE_DIR}/lib)
else()
    set(ASGE_BIN_DIRECTORY ${asge_SOURCE_DIR}/lib)
    set(ASGE_LIB_DIRECTORY ${asge_SOURCE_DIR}/lib)
endif()


## postfix to ensure we link to the correct libs ##
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(POSTFIX "d")
else()
    set(POSTFIX "")
endif()


if(BUILD_SHARED_LIBS)

    ## shared libraries only requires a single link to game engine
    find_library(libGameEngine REQUIRED
            NAMES libGameEngine${POSTFIX}.dll.a
            libGameEngine${POSTFIX}.so
            libGameEngine${POSTFIX}.dylib ASGE${POSTFIX}
            PATHS ${ASGE_LIB_DIRECTORY} NO_DEFAULT_PATH)
    message("libGameEngine: " ${libGameEngine} )

    set_property (
            TARGET ASGE PROPERTY
            IMPORTED_LOCATION
            ${libGameEngine})

    set(libGameEngine ${libGameEngine})

    if(WIN32)
        find_library(GameEngineDLL REQUIRED
                NAMES libGameEngine${POSTFIX}.dll ASGE${POSTFIX}
                PATHS ${ASGE_BIN_DIRECTORY} NO_DEFAULT_PATH)

        message("GameEngineDLL: " ${GameEngineDLL} )
        set(GameEngineDLL ${GameEngineDLL} PARENT_SCOPE)

    else()
        set(GameEngineDLL ${libGameEngine} PARENT_SCOPE)
    endif()

else()

    ## static builds are hideous
    find_library(libFreetype REQUIRED NAMES freetype${POSTFIX}              PATHS ${ASGE_LIB_DIRECTORY} NO_DEFAULT_PATH)
    find_library(libPhysFS   REQUIRED NAMES physfs${POSTFIX}                PATHS ${ASGE_LIB_DIRECTORY} NO_DEFAULT_PATH)
    find_library(libPhysFS++ REQUIRED NAMES physfscpp${POSTFIX}             PATHS ${ASGE_LIB_DIRECTORY} NO_DEFAULT_PATH)
    find_library(libGLFW     REQUIRED NAMES glfw3${POSTFIX}                 PATHS ${ASGE_LIB_DIRECTORY} NO_DEFAULT_PATH)
    find_library(libGLEW     REQUIRED NAMES GLEW${POSTFIX} glew32${POSTFIX} PATHS ${ASGE_LIB_DIRECTORY} NO_DEFAULT_PATH)

    set(OpenGL_GL_PREFERENCE "GLVND")
    find_package(OpenGL REQUIRED)
    message("libOpenGL:     " ${OPENGL_LIBRARIES})

    if(PLATFORM STREQUAL "linux")
        find_package(X11 REQUIRED)
        find_package(ZLIB REQUIRED)
        find_package(BZip2 REQUIRED)

        message("libX11:        " ${X11_LIBRARIES})
        message("libZLIB:       " ${ZLIB_LIBRARIES})
        message("libBZip2:      " ${BZIP2_LIBRARIES})
    endif()


    find_library(libGameEngine REQUIRED
            NAMES libGameEngine${POSTFIX}.a libGameEngine${POSTFIX}.lib ASGE
            PATHS ${ASGE_LIB_DIRECTORY} NO_DEFAULT_PATH)

    message("libGLEW:       " ${libGLEW})
    message("libGLFW:       " ${libGLFW})
    message("libFreetype:   " ${libFreetype})
    message("libPhysFS:     " ${libPhysFS})
    message("libPhysFS++:   " ${libPhysFS++})
    message("libGameEngine: " ${libGameEngine})

    set_property (
            TARGET ASGE PROPERTY
            IMPORTED_LOCATION
            ${libGameEngine}
    )

    ## all platforms need these libraries
    list(APPEND LINK_LIBS ${libGLEW} ${libGLFW} ${libFreetype} ${libPhysFS++} ${libPhysFS} ${OPENGL_LIBRARIES})

    # mac specific libraries
    if(PLATFORM STREQUAL "osx")
        find_library(IOKIT IOKit)
        find_library(COCOA Cocoa)
        find_library(COREVIDEO CoreVideo)

        message("IOKit:        " ${IOKIT})
        message("Cocoa:        " ${COCOA})
        message("CoreVideo:    " ${COREVIDEO})

        list(APPEND LINK_LIBS
                -lz -lbz2
                ${AUDIO_UNIT}
                ${AUDIO_TBOX}
                ${COCOA}
                ${COREVIDEO}
                ${IOKIT})
    endif()

    # linux specific libraries
    if(PLATFORM STREQUAL "linux")
        list(APPEND LINK_LIBS
                ${X11_LIBRARIES}
                ${X11_Xi_LIB}
                ${X11_Xrandr_LIB}
                ${X11_Xxf86vm_LIB}
                ${X11_Xinerama_LIB}
                ${X11_Xcursor_LIB}
                ${ZLIB_LIBRARIES}
                ${BZIP2_LIBRARIES}
                ${CMAKE_DL_LIBS}
                harfbuzz pthread)   #threading
    endif()

    set_property(
            TARGET ASGE PROPERTY
            INTERFACE_LINK_LIBRARIES
            ${LINK_LIBS})

    set_property(
            TARGET ASGE PROPERTY
            INTERFACE_INCLUDE_DIRECTORIES
            ${asge_SOURCE_DIR}/include)
endif()

set_target_properties(
        ASGE PROPERTIES
        URL https://github.com/HuxyUK/ASGE.git
        DESCRIPTION "An OpenGL Gaming Framework"
        PURPOSE "Enables ASGE Games" )

include(CMakePrintHelpers)
cmake_print_properties(
        TARGETS ASGE PROPERTIES
        LOCATION
        INTERFACE_INCLUDE_DIRECTORIES )

include_directories(${asge_SOURCE_DIR}/include)
