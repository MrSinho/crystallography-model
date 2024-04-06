cmake_minimum_required(VERSION 3.13)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)



function(build_crystal_model_docs)



if (NOT DEFINED CRYSTAL_MODEL_BINARIES_DIR)
message(FATAL_ERROR "crystal-model cmake error: missing CRYSTAL_MODEL_BINARIES_DIR")
endif()

if (NOT DEFINED CRYSTAL_MODEL_REPO_ROOT_DIR)
message(FATAL_ERROR "crystal-model cmake error: missing CRYSTAL_MODEL_REPO_ROOT_DIR")
endif()




find_package(Doxygen)


if (DOXYGEN_FOUND)

    set(CRYSTAL_MODEL_DOXYFILESRC ${CRYSTAL_MODEL_REPO_ROOT_DIR}/docs/Doxyfile.in)
    set(CRYSTAL_MODEL_DOXYFILEDST ${CRYSTAL_MODEL_REPO_ROOT_DIR}/docs/Doxyfile)

    message(STATUS "Configuring " ${CRYSTAL_MODEL_DOXYFILEDST} " from " ${CRYSTAL_MODEL_DOXYFILESRC})

    configure_file(
        ${CRYSTAL_MODEL_DOXYFILESRC} 
        ${CRYSTAL_MODEL_DOXYFILEDST}
    )

    message(STATUS "Creating custom docs command: " ${DOXYGEN_EXECUTABLE} " " ${CRYSTAL_MODEL_DOXYFILEDST})

    add_custom_target(docs-crystal-model
        COMMAND ${DOXYGEN_EXECUTABLE} ${CRYSTAL_MODEL_DOXYFILEDST}
        WORKING_DIRECTORY ${CRYSTAL_MODEL_REPO_ROOT_DIR}
        COMMENT "Generating HTML documentation"
        VERBATIM
    )

endif(DOXYGEN_FOUND)

endfunction()