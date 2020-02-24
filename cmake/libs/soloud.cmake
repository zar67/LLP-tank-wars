OPTION(ENABLE_SOUND "Adds SoLoud to the Project" OFF)

if(ENABLE_SOUND)

    message("##### SOLOUD SOUND ENGINE #####")
    message("ENABLING SOLOUD SUPPORT FOR " ${PROJECT_NAME})

    set (SOLOUD_BACKEND_SDL2        "OFF" CACHE INTERNAL "OFF")

    # Enable a native-ish build of Audio Engine
    if (WIN32)
        set(SOLOUD_BACKEND_WASAPI    "ON" CACHE INTERNAL "ON")
        set(SOLOUD_BACKEND_WINMM     "ON" CACHE INTERNAL "ON")
    elseif(APPLE)
        set(SOLOUD_BACKEND_COREAUDIO "ON" CACHE INTERNAL "ON")
    else()
        find_package(ALSA QUIET)
        if (ALSA_FOUND)
            set(SOLOUD_BACKEND_ALSA  "ON" CACHE INTERNAL "ON")
        endif()
    endif()

    if(BUILD_SHARED_LIBS)
        set(SOLOUD_DYNAMIC           "ON"  CACHE INTERNAL "ON")
        set(SOLOUD_STATIC            "OFF" CACHE INTERNAL "OFF")
    else()
        set(SOLOUD_DYNAMIC           "OFF" CACHE INTERNAL "OFF")
        set(SOLOUD_STATIC            "ON"  CACHE INTERNAL "ON")
    endif()

    # fetch project
    include(FetchContent)
    FetchContent_Declare(
            soloud
            GIT_REPOSITORY https://github.com/HuxyUK/soloud.git
            GIT_TAG        ef788f7b9c9b6603e11a324459cb6af02eb425d4)

    FetchContent_GetProperties(soloud)
    if(NOT soloud_POPULATED)
        FetchContent_Populate(soloud)
        add_subdirectory(${soloud_SOURCE_DIR}/contrib ${soloud_BINARY_DIR})
    endif()

    set_target_properties(soloud PROPERTIES CXX_CLANG_TIDY "")
    set_target_properties(soloud PROPERTIES CXX_CPPCHECK "")
endif()
