cmake_minimum_required(VERSION 2.8)

project(sgfilter)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenMP)
if (OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif ()

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include_directories("${CMAKE_PREFIX_PATH}/include" "${CMAKE_SOURCE_DIR}/include")

find_library(LIB_OPAE NAMES opae-c REQUIRED)
find_library(LIB_OPAE_ASE NAMES opae-c-ase REQUIRED)
find_library(LIB_UUID NAMES uuid REQUIRED)
find_library(LIB_MPF NAMES MPF REQUIRED)
find_library(LIB_FDAM NAMES fdam REQUIRED)

file(GLOB CPP_SRCS *.cpp)
file(GLOB H_SRCS *.h)

add_executable(sgfilter_harp ${CPP_SRCS} ${H_SRCS})
target_link_libraries(sgfilter_harp ${LIB_FDAM} ${LIB_OPAE} ${LIB_UUID} ${LIB_MPF})

add_executable(sgfilter_ase ${CPP_SRCS} ${H_SRCS})
target_link_libraries(sgfilter_ase ${LIB_FDAM} ${LIB_OPAE_ASE} ${LIB_UUID} ${LIB_MPF})
