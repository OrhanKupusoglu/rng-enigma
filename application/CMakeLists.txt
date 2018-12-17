cmake_minimum_required(VERSION 3.5)

set(BUILD_NAME rng-enigma-app)

set(BUILD_MAJOR_VER 0)
set(BUILD_MINOR_VER 3)
set(BUILD_PATCH_VER 1)

add_definitions(-DRNG_DEBUG=false)
add_definitions(-DRNG_CFG=NULL)
add_definitions(-DRNG_SEED=NULL)

get_directory_property( DirDefs COMPILE_DEFINITIONS )
message( "-- Compile_definitions: ${DirDefs}" )

set(SOURCE_FILES "src/main.c")

set(HEADER_FILES "rng_enigma.h")

set(CMAKE_LIBRARY_PATH "${PROJECT_SOURCE_DIR}/../library/lib")

project(${BUILD_NAME} VERSION ${BUILD_MAJOR_VER}.${BUILD_MINOR_VER}.${BUILD_PATCH_VER} LANGUAGES C)

add_executable(${BUILD_NAME} ${SOURCE_FILES})

target_compile_options(${BUILD_NAME} INTERFACE -Wall
                                               -Wextra
                                               -Wpedantic
)

find_library(LIB_ENIGMA NAMES rng-enigma PATHS ${PROJECT_SOURCE_DIR}/../library PATH_SUFFIXES lib)

execute_process(COMMAND dirname -- ${LIB_ENIGMA} OUTPUT_VARIABLE LIB_DIR)
string(REGEX REPLACE "\n$" "" LIB_DIR ${LIB_DIR})

message(STATUS "Link library      : ${LIB_ENIGMA}")
message(STATUS "Include directory : ${LIB_DIR}")

target_link_libraries(${BUILD_NAME} PUBLIC ${LIB_ENIGMA})
target_include_directories(${BUILD_NAME} PUBLIC ${LIB_DIR})
set_target_properties(${BUILD_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
