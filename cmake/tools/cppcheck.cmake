find_program(
        CMAKE_CXX_CPPCHECK
        NAMES cppcheck
        HINTS ${CMAKE_SOURCE_DIR}/tools/cppcheck)

if (CMAKE_CXX_CPPCHECK)

      include(ProcessorCount)
      ProcessorCount(CPU_CORES)

      if(WIN32)
          set(CPPCHECK_PLATFORM "win64")
      else()
          set(CPPCHECK_PLATFORM "unix64")
      endif()

      list(
        APPEND CMAKE_CXX_CPPCHECK
        "-j ${CPU_CORES}"
        "--platform=${CPPCHECK_PLATFORM}"
        "--enable=warning,style,performance,portability"
        "--force"
        "--inline-suppr"
        "--suppressions-list=${CMAKE_SOURCE_DIR}/.cppcheck"
        "--template={file}({line}): [CPPCHECK]: {severity} ({id}): {message}'"
        "--std=c++17"
        "--quiet"
        "--error-exitcode=1"
        "-i_deps"
        "-iCMakeFiles")

#      # Check CppCheck version
#      set(CPP_CHECK_CMD ${CPPCHECK} --version)
#      execute_process(COMMAND ${CPP_CHECK_CMD}
#              WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#              RESULT_VARIABLE CPP_CHECK_RESULT
#              OUTPUT_VARIABLE CPP_CHECK_VERSION
#              ERROR_VARIABLE CPP_CHECK_ERROR)

#      add_custom_target(ANALYZE_CPPCHECK DEPENDS ${PROJECT_NAME}
#              COMMAND ${CPPCHECK}
#              WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
#              COMMENT "Static code analysis using ${CPP_CHECK_VERSION}"
#              )
#       endif()
#      # Run CppCheck from the working directory, as specified in the add_custom_target command below
#      "." )

endif()
