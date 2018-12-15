cmake_minimum_required(VERSION 3.5)

# ------------------------------------------------------------------------------
# SWIG BINDINGS FOR PYTHON
# ------------------------------------------------------------------------------

# $ sudo apt install swig
# $ sudo apt install python-dev
# $ sudo apt install python3-dev
# $ LD_LIBRARY_PATH=. python3

include("../CMakeVersion.txt")

if(POLICY CMP0078)
    cmake_policy(SET CMP0078 LEGACY)
endif()

find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_PATH})

include_directories("../include")

set(SOURCE_FILES "../src/rng_enigma.c"
                 "../src/rng_file.c")

set(CMAKE_SWIG_FLAGS "")

set_source_files_properties("lib_rng_enigma.i" PROPERTIES SWIG_FLAGS "-threads" SWIG_FLAGS "-py3")

swig_add_library(rng_enigma TYPE SHARED
                            LANGUAGE python
                            SOURCES "lib_rng_enigma.i" ${SOURCE_FILES}
)

swig_link_libraries(rng_enigma ${PYTHON_LIBRARIES})

add_custom_command(TARGET ${SWIG_MODULE_rng_enigma_REAL_NAME}
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy _rng_enigma.so "../../lib"
                   COMMAND ${CMAKE_COMMAND} -E copy rng_enigma.py "../../lib")
