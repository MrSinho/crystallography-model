cmake_minimum_required(VERSION 3.0)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)



project(crystal-model C)



# YOU CAN MODIFY
set(CRYSTAL_MODEL_ROOT_DIR     ${CMAKE_CURRENT_SOURCE_DIR})
set(CRYSTAL_MODEL_BINARIES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)



# YOU CAN MODIFY
option(CRYSTAL_MODEL_BINARY_TYPE CACHE OFF)
option(SH_ENGINE_ROOT_DIR     CACHE OFF)
option(SH_ENGINE_BINARIES_DIR CACHE OFF)



# YOU CAN MODIFY 
if (NOT CRYSTAL_MODEL_BINARY_TYPE)
set(CRYSTAL_MODEL_BINARY_TYPE "EXECUTABLE")
endif()

# DO NOT MODIFY!
if (NOT SH_ENGINE_ROOT_DIR)
message(FATAL_ERROR "crystal-model: missing SH_ENGINE_ROOT_DIR")
endif()

# DO NOT MODIFY!
if (NOT SH_ENGINE_BINARIES_DIR)
set(SH_ENGINE_BINARIES_DIR ${SH_ENGINE_ROOT_DIR}/bin)
endif()


message(STATUS "CRYSTAL_MODEL_ROOT_DIR      : ${CRYSTAL_MODEL_ROOT_DIR}")
message(STATUS "CRYSTAL_MODEL_BINARIES_DIR  : ${CRYSTAL_MODEL_BINARIES_DIR}")
message(STATUS "CRYSTAL_MODEL_BINARY_TYPE   : ${CRYSTAL_MODEL_BINARY_TYPE}")

message(STATUS "SH_ENGINE_ROOT_DIR     : ${SH_ENGINE_ROOT_DIR}")
message(STATUS "SH_ENGINE_BINARIES_DIR : ${SH_ENGINE_BINARIES_DIR}")



include(${SH_ENGINE_ROOT_DIR}/collection/collection.cmake)
include(${SH_ENGINE_ROOT_DIR}/docs/shengine-docs.cmake)


function(build_crystal_model)

build_shengine_collection()
build_shengine_docs()


if(${CRYSTAL_MODEL_BINARY_TYPE} STREQUAL "STATIC")
add_library(crystal-model STATIC 
    ${CRYSTAL_MODEL_ROOT_DIR}/src/crystal-model.c
    #src_files
)


elseif(${CRYSTAL_MODEL_BINARY_TYPE} STREQUAL "SHARED")
add_library(crystal-model SHARED 
    ${CRYSTAL_MODEL_ROOT_DIR}/src/crystal-model.c
    #src_files
)
target_compile_definitions(shengine PUBLIC 
    SH_APPLICATION_TARGET_TYPE_SHARED=1
)


elseif(${CRYSTAL_MODEL_BINARY_TYPE} STREQUAL "EXECUTABLE")
add_executable(crystal-model  
    ${CRYSTAL_MODEL_ROOT_DIR}/src/crystal-model.c
    #src_files
    ${SH_ENGINE_ROOT_DIR}/collection/sheditor/src/shEditor.c
)
target_include_directories(crystal-model PUBLIC 
    ${SH_ENGINE_ROOT_DIR}/collection/sheditor/include
)
target_compile_definitions(crystal-model PUBLIC 
    SH_APPLICATION_TARGET_TYPE_EXECUTABLE=1
)
endif()


target_include_directories(crystal-model PUBLIC 
    ${CRYSTAL_MODEL_ROOT_DIR}/include
)

target_link_libraries(crystal-model PUBLIC
    shengine
)

if (WIN32)
set_target_properties(crystal-model sheditor PROPERTIES 
    ARCHIVE_OUTPUT_DIRECTORY      ${CRYSTAL_MODEL_BINARIES_DIR}/windows
    RUNTIME_OUTPUT_DIRECTORY      ${CRYSTAL_MODEL_BINARIES_DIR}/windows
    VS_DEBUGGER_WORKING_DIRECTORY ${CRYSTAL_MODEL_BINARIES_DIR}/windows
)
else()
set_target_properties(crystal-model sheditor PROPERTIES 
    ARCHIVE_OUTPUT_DIRECTORY      ${CRYSTAL_MODEL_BINARIES_DIR}/linux 
    RUNTIME_OUTPUT_DIRECTORY      ${CRYSTAL_MODEL_BINARIES_DIR}/linux
    VS_DEBUGGER_WORKING_DIRECTORY ${CRYSTAL_MODEL_BINARIES_DIR}/linux
)
endif(WIN32)

set_shengine_collection_binaries()

endfunction()


function(crystal_model_main)
build_crystal_model()
endfunction()



crystal_model_main()
