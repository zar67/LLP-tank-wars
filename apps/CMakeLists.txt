## set the various target names ##
set(GAME ${PROJECT_NAME})
set(DEDICATED DedicatedServer)
set(COMMON_TARGET CommonTarget)


## we're using a common target for shared properties ##
add_library(${COMMON_TARGET} INTERFACE)

## set to C++17 compiler support ##
target_compile_features(${COMMON_TARGET} INTERFACE cxx_std_17)

## link against the game library and ASGE ##
add_dependencies(${COMMON_TARGET} ${LIB_NAME})
target_link_libraries(${COMMON_TARGET} INTERFACE ASGE)
target_link_libraries(${COMMON_TARGET} INTERFACE ${LIB_NAME})
target_link_libraries(${COMMON_TARGET} INTERFACE ${CMAKE_DL_LIBS} pthread)
target_include_directories(${COMMON_TARGET} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

## logging levels ##
target_compile_definitions(
        ${COMMON_TARGET} INTERFACE
        "$<$<CONFIG:Debug>:LOGGING_LEVEL_DEBUG>"
        "$<$<CONFIG:Release>:LOGGING_LEVEL_ERROR>")

## enable compiler warnings and debug flags ##
target_compile_options(
        ${COMMON_TARGET} INTERFACE
        $<$<COMPILE_LANGUAGE:CXX>:${BUILD_FLAGS_FOR_CXX}>
        $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:Debug>>:${DEBUG_BUILD_FLAGS_FOR_CXX}>)

## enable static linking against gcc build ##
if (WIN32 AND NOT BUILD_SHARED_LIBS AND NOT MSVC)
    target_link_libraries(${COMMON_TARGET} INTERFACE -static)
endif()

## fixes for making position-independent executables ##
if(CMAKE_COMPILER_IS_GNUCC)
    target_link_libraries(${COMMON_TARGET} -no-pie pthread)
endif()

## Icon support, because why not ##
if (WIN32)
    set(ICON Resources/game.rc)
elseif (APPLE)
    set(ICON Resources/game.icns ../source/Input.cpp ../include/gamelib/Input.h ../include/gamelib/InputData.h ../include/gamelib/Audio/AudioManager.h)
    set_source_files_properties(
            Resources/game.icns PROPERTIES
            MAXOSX_PACKAGE_LOCATION "Resources")
endif()

## the ASGE Game, which spawns the Game window ##
add_executable(
        ${GAME} ${ICON}
        ## source code files for app here ##
        ASGEGame/main.cpp
        ASGEGame/ASGEGame.hpp
        ASGEGame/ASGEGame.cpp)

set(GAMEDATA_FOLDER "apps/data")
set(ITCHIO_UESR "")
include(tools/itch.io)

## hide console unless debug build ##
if (NOT CMAKE_BUILD_TYPE STREQUAL "Release" AND WIN32)
    target_compile_options(${GAME} PRIVATE -mwindows)
endif()

## if sound is enabled link against the soloud libs ##
if (ENABLE_SOUND)
    add_dependencies(${GAME} soloud)
    target_link_libraries(${GAME} PRIVATE soloud)
endif()

## connect up the common target properties ##
target_link_libraries(${GAME} LINK_PRIVATE ${COMMON_TARGET})
set_target_properties(${GAME} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${GAME} PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(${GAME} PROPERTIES CXX_CLANG_TIDY ${CLANG_TIDY})

if(ENABLE_DEDICATED)
## a non-gui dedicated server app ##
add_executable(
        ${DEDICATED}
        ## source code files for app here ##
        Dedicated/main.cpp
        Dedicated/GameServer.hpp Dedicated/GameServer.cpp)

target_link_libraries(${DEDICATED} LINK_PRIVATE ${COMMON_TARGET})
set_target_properties(${DEDICATED} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${DEDICATED} PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(${DEDICATED} PROPERTIES CXX_CLANG_TIDY ${CLANG_TIDY})
endif()
