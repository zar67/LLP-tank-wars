include(CheckPIESupported)
check_pie_supported()

if ( CMAKE_COMPILER_IS_GNUCC )
    list(APPEND BUILD_FLAGS_FOR_CXX
            "-Wall" "-Wextra" "-Wshadow" "-Wnon-virtual-dtor"
            "-Wold-style-cast" "-Wunused" "-pedantic"
            "-Woverloaded-virtual" "-Wpedantic" "-Wconversion"
            "-Wduplicated-cond" "-Wduplicated-branches" "-Wlogical-op"
            "-Wnull-dereference" "-Wuseless-cast"
            "-Wno-unused-parameter" "-Werror")

    set(APPEND DEBUG_BUILD_FLAGS_FOR_CXX "-O0" )
elseif( MSVC )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W4 /WX")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
