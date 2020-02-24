find_program(CLANG_TIDY
        NAMES clang-tidy)

message(${CLANG_TIDY})
set(CMAKE_CXX_CLANG_TIDY clang-tidy)

#file(GLOB_RECURSE
#        ALL_CXX_SOURCE_FILES
#        *.[chi]pp *.[chi]xx *.cc *.hh *.ii *.[CHI]
#        )

#
#message(${ALL_CXX_SOURCE_FILES})
#
## Adding clang-tidy target if executable is found
##find_program(CLANG_TIDY "clang-tidy")
#if(CLANG_TIDY)
#    add_custom_target(
#            clang-tidy
#            COMMAND ${CLANG_TIDY}
#            ${ALL_CXX_SOURCE_FILES}
#            -config='${CMAKE_SOURCE_DIR/.clang-tidy}'
#            --
#            -std=c++11
#            ${INCLUDE_DIRECTORIES}
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