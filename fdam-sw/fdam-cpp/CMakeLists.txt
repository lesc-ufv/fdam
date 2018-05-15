project("fdam")

cmake_minimum_required(VERSION 2.8)

set(FDAM_API_VER_MAJOR 0 CACHE STRING "FDAM API major version")
set(FDAM_API_VER_MINOR 9 CACHE STRING "FDAM API minor version")
set(FDAM_API_VER_REV   0 CACHE STRING "FDAM API revision version")
set(FDAM_API_VERSION   ${FDAM_API_VER_MAJOR}.${FDAM_API_VER_MINOR}.${FDAM_API_VER_REV})

set(CMAKE_PROJECT "fdam")
set(CMAKE_VERSION_MAJOR "${FDAM_API_VER_MAJOR}")
set(CMAKE_VERSION_MINOR "${FDAM_API_VER_MINOR}")
set(CMAKE_VERSION_PATCH "${FDAM_API_VER_REV}")

find_package(Threads REQUIRED)
include(CheckIncludeFile)

if (NOT CMAKE_VERSION VERSION_LESS "3.1")
    if (NOT CMAKE_C_COMPILER_VERSION VERSION_LESS 4.7)
        # New CMake, new enough compiler, ideal case...
        set (CMAKE_C_STANDARD 11)
    else ()
        # Ideally here we would set CMAKE_C_STANDARD 99 but that isn't
        # working for our standard setup.
        if (NOT WIN32)
            set (CMAKE_C_FLAGS "--std=gnu99 ${CMAKE_C_FLAGS}")
        endif()
    endif ()
else ()
    # Old version of CMake doesn't know how to set C standard
    if (NOT CMAKE_C_COMPILER_VERSION VERSION_LESS 4.7)
        set (CMAKE_C_FLAGS "--std=gnu11 ${CMAKE_C_FLAGS}")
    else ()
        set (CMAKE_C_FLAGS "--std=gnu99 ${CMAKE_C_FLAGS}")
    endif ()
endif ()

find_package(Doxygen)
if (DOXYGEN_FOUND)
    include(doc/doxygen.cmake)
endif(DOXYGEN_FOUND)


##
## Allow specification of an alternate path to the FPGA library include files
## and binary.
##
set (OPAELIB_LIBS_PATH "" CACHE STRING "OPAE libs path")
set (OPAELIB_INC_PATH "" CACHE STRING "OPAE include path")

if (WIN32)
    if("X$ENV{OPAELIB_ROOT}X" STREQUAL "XX")
        message(FATAL_ERROR "Must specify OPAELIB_ROOT")
    else()
        set(OPAELIB_ROOT $ENV{OPAELIB_ROOT})
    endif()

    get_filename_component(dir ${OPAELIB_ROOT} ABSOLUTE DIRECTORY)

    set(OPAELIB_LIBS_PATH ${dir}/binary/app+lib/x64)
    set(OPAELIB_INC_PATH ${dir}/headers)

    if(NOT IS_DIRECTORY ${OPAELIB_LIBS_PATH} OR NOT IS_DIRECTORY ${OPAELIB_INC_PATH}/opae)
        message(FATAL_ERROR "Invalid OPAELIB_ROOT")
    endif()

    add_library(OpaeLib SHARED IMPORTED GLOBAL)

    set_property(TARGET OpaeLib PROPERTY IMPORTED_LOCATION_DEBUG ${OPAELIB_LIBS_PATH}/Debug/OpaeLib.dll)
    set_property(TARGET OpaeLib PROPERTY IMPORTED_IMPLIB_DEBUG ${OPAELIB_LIBS_PATH}/Debug/OpaeLib.lib)
    set_property(TARGET OpaeLib PROPERTY IMPORTED_LOCATION_RELEASE ${OPAELIB_LIBS_PATH}/Release/OpaeLib.dll)
    set_property(TARGET OpaeLib PROPERTY IMPORTED_IMPLIB_RELEASE ${OPAELIB_LIBS_PATH}/Release/OpaeLib.lib)

    include_directories(${OPAELIB_INC_PATH})
endif(WIN32)


include_directories(${PROJECT_SOURCE_DIR}/include)

## Add install directory to include path in case OPAE has been installed there
if (IS_DIRECTORY ${CMAKE_INSTALL_PREFIX}/include)
    include_directories(AFTER ${CMAKE_INSTALL_PREFIX}/include)
    list(APPEND CMAKE_REQUIRED_INCLUDES ${CMAKE_INSTALL_PREFIX}/include)
endif()

find_file(OPAELIB_HDRS_PRESENT opae/fpga.h PATHS "${OPAELIB_INC_PATH}" NO_DEFAULT_PATH)

if (WIN32)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -wd4710 -wd4711 -wd4820 -O2")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Od")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Od")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O2")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(FATAL_ERROR "Release mode not supported")
endif(CMAKE_BUILD_TYPE STREQUAL "Release")


##
## Build FPGA library if libfpga header files are present
##
check_include_file("opae/fpga.h" OPAE_PRESENT)
if (OPAE_PRESENT OR OPAELIB_HDRS_PRESENT)
    include(src/fdam.cmake)
else(OPAE_PRESENT OR OPAELIB_HDRS_PRESENT)
    message("Not building ${CMAKE_SHARED_LIBRARY_PREFIX}fdam${CMAKE_SHARED_LIBRARY_SUFFIX} -- opae/fpga.h not found")
endif(OPAE_PRESENT OR OPAELIB_HDRS_PRESENT)

