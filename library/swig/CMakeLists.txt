cmake_minimum_required(VERSION 3.5)

# ------------------------------------------------------------------------------
# SWIG BINDINGS FOR PYTHON
# ------------------------------------------------------------------------------

# $ sudo apt install swig
# $ sudo apt install python-dev
# $ sudo apt install python3-dev
# $ LD_LIBRARY_PATH=. python3

include("../CMakeVersion.txt")

cmake_policy(SET CMP0078 NEW)

set(BUILD_MAJOR_VER 0)
set(BUILD_MINOR_VER 3)
set(BUILD_PATCH_VER 1)

add_definitions(-DRNG_ENIGMA_LIB_VERSION="${BUILD_MAJOR_VER}.${BUILD_MINOR_VER}.${BUILD_PATCH_VER}")

find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_PATH})

include_directories("../include")

set(SOURCE_FILES "../src/rng_enigma.c"
                 "../src/rng_file.c")


#set(CMAKE_SWIG_OUTDIR "../../lib")
set(CMAKE_SWIG_FLAGS "")

#set_property(SOURCE ./swig/lib_rng_enigma.i PROPERTY SWIG_MODULE_NAME rng_enigma.so)
#set_property(SOURCE ./swig/lib_rng_enigma.i PROPERTY SWIG_OUTFILE_DIR "./lib")

#set_source_files_properties("lib_rng_enigma.i" PROPERTIES C ON)
set_source_files_properties("lib_rng_enigma.i" PROPERTIES SWIG_FLAGS "-threads" SWIG_FLAGS "-py3")

swig_add_library(rng_enigma TYPE SHARED
                            LANGUAGE python
                            SOURCES "lib_rng_enigma.i" ${SOURCE_FILES}
)

swig_link_libraries(rng_enigma ${PYTHON_LIBRARIES})

add_custom_command(TARGET rng_enigma
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:rng_enigma> "../../lib"
                   COMMAND ${CMAKE_COMMAND} -E copy rng_enigma.py "../../lib")
