if(${CMAKE_CXX_PLATFORM_ID} STREQUAL "MinGW")
    set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_INCLUDES OFF)
endif()

find_program(CLANG_TIDY
    NAMES clang-tidy-10 clang-tidy-9 clang-tidy-8 clang-tidy
    HINTS ${CMAKE_SOURCE_DIR}/tools/*/${PLATFORM}/)

message-

list(
    APPEND CLANG_TIDY
    "-quiet"
    "-p=${CMAKE_BINARY_DIR}" )

set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY})

#file(GLOB_RECURSE
#        ALL_CXX_SOURCE_FILES
#        *.[chi]pp *.[chi]xx *.cc *.hh *.ii *.[CHI]
#        )

## Adding clang-tidy target if executable is found
##find_program(CLANG_TIDY "clang-tidy")
#if(CLANG_TIDY)
#    add_custom_target(
#            clang-tidy
#            COMMAND ${CLANG_TIDY}
#            "${CMAKE_SOURCE_DIR}/source/components/GCNetServer.cpp"
           # "-extra-arg=-I${CMAKE_SOURCE_DIR}/include/gamelib"
           # "-extra-arg=-isystem${CMAKE_BINARY_DIR}/_deps/asge-src/include"
           # "-extra-arg=-isystem${CMAKE_BINARY_DIR}/_deps/netlib-src/Headers"
            #"-extra-arg=-isystem${CMAKE_BINARY_DIR}/_deps/netlib-src/Headers/NetLib/Windows"
#    )
#endif()

#get_target_property(MY_SOURCES MyNetGame SOURCES)
#message(${MY_SOURCES})
#
#get_target_property(MY_SOURCES GameLib SOURCES)
#message(${MY_SOURCES})
#
#get_target_property(MY_SOURCES MyNetGame SOURCES)
#message(${MY_SOURCES})
