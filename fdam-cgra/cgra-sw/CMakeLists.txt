project("cgra")

cmake_minimum_required(VERSION 3.0.2)

set(CGRA_API_VER_MAJOR 0 CACHE STRING "CGRA API major version")
set(CGRA_API_VER_MINOR 9 CACHE STRING "CGRA API minor version")
set(CGRA_API_VER_REV   0 CACHE STRING "CGRA API revision version")
set(CGRA_API_VERSION   ${CGRA_API_VER_MAJOR}.${CGRA_API_VER_MINOR}.${CGRA_API_VER_REV})

set(CMAKE_PROJECT "cgra")
set(CMAKE_VERSION_MAJOR "${CGRA_API_VER_MAJOR}")
set(CMAKE_VERSION_MINOR "${CGRA_API_VER_MINOR}")
set(CMAKE_VERSION_PATCH "${CGRA_API_VER_REV}")

find_package(Threads REQUIRED)
include(CheckIncludeFileCXX)

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
set (FDAMLIB_LIBS_PATH "" CACHE STRING "FDAM libs path")
set (FDAMLIB_INC_PATH "" CACHE STRING "FDAM include path")

if (WIN32)
    if("X$ENV{FDAMLIB_ROOT}X" STREQUAL "XX")
        message(FATAL_ERROR "Must specify FDAMLIB_ROOT")
    else()
        set(FDAMLIB_ROOT $ENV{FDAMLIB_ROOT})
    endif()

    get_filename_component(dir ${FDAMLIB_ROOT} ABSOLUTE DIRECTORY)

    set(FDAMLIB_LIBS_PATH ${dir}/binary/app+lib/x64)
    set(FDAMLIB_INC_PATH ${dir}/headers)

    if(NOT IS_DIRECTORY ${FDAMLIB_LIBS_PATH} OR NOT IS_DIRECTORY ${FDAMLIB_INC_PATH}/fdam)
        message(FATAL_ERROR "Invalid FDAMLIB_ROOT")
    endif()

    add_library(FdamLib SHARED IMPORTED GLOBAL)

    set_property(TARGET FdamLib PROPERTY IMPORTED_LOCATION_DEBUG ${FDAMLIB_LIBS_PATH}/Debug/FdamLib.dll)
    set_property(TARGET FdamLib PROPERTY IMPORTED_IMPLIB_DEBUG ${FDAMLIB_LIBS_PATH}/Debug/FdamLib.lib)
    set_property(TARGET FdamLib PROPERTY IMPORTED_LOCATION_RELEASE ${FDAMLIB_LIBS_PATH}/Release/FdamLib.dll)
    set_property(TARGET FdamLib PROPERTY IMPORTED_IMPLIB_RELEASE ${FDAMLIB_LIBS_PATH}/Release/FdamLib.lib)

    include_directories(${FDAMLIB_INC_PATH})
endif(WIN32)


include_directories(${PROJECT_SOURCE_DIR}/include)

## Add install directory to include path in case FDAM has been installed there
if (IS_DIRECTORY ${CMAKE_INSTALL_PREFIX}/include)
    include_directories(AFTER ${CMAKE_INSTALL_PREFIX}/include)
    list(APPEND CMAKE_REQUIRED_INCLUDES ${CMAKE_INSTALL_PREFIX}/include)
endif()

find_file(FDAMLIB_HDRS_PRESENT fdam/AccManagement.h PATHS "${FDAMLIB_INC_PATH}" NO_DEFAULT_PATH)

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
## Build CGRA library if libfdam header files are present
##
check_include_file_cxx("fdam/AccManagement.h" FDAM_PRESENT)
if (FDAM_PRESENT OR FDAMLIB_HDRS_PRESENT)
    include(src/cgra.cmake)
else(FDAM_PRESENT OR FDAMLIB_HDRS_PRESENT)
    message("Not building ${CMAKE_SHARED_LIBRARY_PREFIX}cgra${CMAKE_SHARED_LIBRARY_SUFFIX} -- fdam/AccManagement.h not found")
endif(FDAM_PRESENT OR FDAMLIB_HDRS_PRESENT)

