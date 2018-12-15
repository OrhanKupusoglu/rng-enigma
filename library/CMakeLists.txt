cmake_minimum_required(VERSION 3.5)

set(BUILD_NAME rng-enigma)

#set(BUILD_MAJOR_VER 0)
#set(BUILD_MINOR_VER 3)
#set(BUILD_PATCH_VER 1)

#add_definitions(-DRNG_ENIGMA_LIB_VERSION="${BUILD_MAJOR_VER}.${BUILD_MINOR_VER}.${BUILD_PATCH_VER}")
include("CMakeVersion.txt")

get_directory_property( DirDefs COMPILE_DEFINITIONS )
message( "-- Compile_definitions: ${DirDefs}" )

set(SOURCE_FILES "src/rng_enigma.c"
                 "src/rng_file.c")

set(HEADER_FILES "include/rng_enigma.h"
                 "include/rng_file.h"
                 "include/lib_rng_enigma.h")

project(${BUILD_NAME} VERSION ${BUILD_MAJOR_VER}.${BUILD_MINOR_VER}.${BUILD_PATCH_VER} LANGUAGES C)

add_library(${BUILD_NAME} STATIC ${SOURCE_FILES})

target_compile_options(${BUILD_NAME} INTERFACE -Wall
                                               -Wextra
                                               -Wpedantic
)

target_include_directories(${BUILD_NAME} PUBLIC include)
set_target_properties(${BUILD_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
install(FILES ${HEADER_FILES} DESTINATION ${PROJECT_SOURCE_DIR}/lib)
