OPTION(ENABLE_CLANG_FORMAT "Adds clang-format style as a target" OFF)

find_program(
        CFE NAMES clang-format
        HINTS ${CMAKE_SOURCE_DIR}/tools/*/${PLATFORM}/)

file(GLOB_RECURSE SOURCE_CODE
        LIST_DIRECTORIES FALSE
        CONFIGURE_DEPENDS
        ${CMAKE_SOURCE_DIR}/apps/*.cpp
        ${CMAKE_SOURCE_DIR}/apps/*.h??
        ${CMAKE_SOURCE_DIR}/include/*.cpp
        ${CMAKE_SOURCE_DIR}/include/*.h??
        ${CMAKE_SOURCE_DIR}/source/*.cpp
        ${CMAKE_SOURCE_DIR}/source/*.h??)

if(CFE)
    list(APPEND CLANG-FORMAT_ARGS
            -i
            --style=file
            ${SOURCE_CODE}
    )

    add_custom_target(
            ANALYZE_CLANG_FORMAT
            COMMAND "${CFE}" ${CLANG-FORMAT_ARGS}
            COMMENT "running clang-format"
    )
endif()

