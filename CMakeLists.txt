cmake_minimum_required(VERSION 3.12...3.16)
include(FeatureSummary)

OPTION(ENABLE_DEDICATED "Console based dedicated server" OFF)
add_feature_info("ENABLE_DEDICATED" ENABLE_DEDICATED "Provides a console based dedicated server.")

set(ENABLE_JSON    OFF CACHE BOOL "Adds JSON to the Project" FORCE)
set(ENABLE_NETWORK ON  CACHE BOOL "Adds Network to the Project" FORCE)
set(ENABLE_SOUND   OFF CACHE BOOL "Adds SoLoud Audio" FORCE)

project(MyNetGame
VERSION 0.1
DESCRIPTION "A network game coded in ASGE"
LANGUAGES CXX)

## global CMake settings ##
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

## helper scripts ##
include(build/flags)
include(build/lto)
include(build/platform)
include(tools/cargo)
include(tools/cppcheck)
include(tools/clang-tidy)
include(tools/clang-format)
include(tools/python)
include(libs/asge)
include(libs/json)
include(libs/soloud)

## set to true to use dlls instead of static libs ##
option(STATIC_APP "Build a static application" ON)
message(" ")
message("##### STATIC OR SHARED APPLICATION #####")
if( STATIC_APP )
    message("BUILDING STATIC APPLICATION \n")
    set(BUILD_SHARED_LIBS false)
else()
    message("BUILDING SHARED APPLICATION \n")
    set(BUILD_SHARED_LIBS true)
endif()

## the game library and executable projects ##
add_subdirectory(source)
add_subdirectory(apps)
add_subdirectory(extern)

## print the feature summary this project added ##
message("\n##### FEATURE AND PACKAGE SUMMARY #####")
feature_summary(WHAT ENABLED_FEATURES DISABLED_FEATURES PACKAGES_FOUND)
feature_summary(FILENAME ${CMAKE_CURRENT_BINARY_DIR}/features.log WHAT ALL)
