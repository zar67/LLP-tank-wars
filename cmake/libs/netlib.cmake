 OPTION(ENABLE_NETWORK "Adds Network to the Project" OFF)

if(ENABLE_NETWORK)

    # fetch project
    include(FetchContent)

    FetchContent_Declare(
            netlib
            GIT_REPOSITORY https://github.com/HuxyUK/NetLibrary.git
            GIT_TAG        origin/master)
        #    GIT_REPOSITORY https://github.com/Halichoerus/NetLibrary
        #    GIT_TAG       v0.07-alpha)


    FetchContent_GetProperties(netlib)
    if(NOT netlib_POPULATED)
        FetchContent_Populate(netlib)
        add_subdirectory(${netlib_SOURCE_DIR} ${netlib_BINARY_DIR})
    endif()

    # lets not worry about compile warnings
    target_compile_options(
            NetLib
            INTERFACE
            $<$<COMPILE_LANGUAGE:CXX>:-w -fpermissive>)

    add_dependencies(${PROJECT_NAME} NetLib)
    target_link_libraries(${PROJECT_NAME} PUBLIC NetLib)

    set_target_properties(NetLib PROPERTIES CXX_CLANG_TIDY "")
    set_target_properties(NetLib PROPERTIES CXX_CPPCHECK "")
endif()
