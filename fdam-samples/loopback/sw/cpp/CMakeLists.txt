cmake_minimum_required(VERSION 3.5)

project(main)

set(CMAKE_CXX_STANDARD 11)

include_directories("${CMAKE_PREFIX_PATH}/include")

find_library(LIB_OPAE NAMES opae-c REQUIRED)
find_library(LIB_OPAE_ASE NAMES opae-c-ase REQUIRED)
find_library(LIB_UUID NAMES uuid REQUIRED)
find_library(LIB_MPF NAMES MPF REQUIRED)
find_library(LIB_FDAM NAMES fdam REQUIRED)

file(GLOB CPP_SRCS ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(main_ase ${CPP_SRCS})
add_executable(main ${CPP_SRCS})

target_link_libraries(main_ase ${LIB_FDAM} ${LIB_OPAE_ASE} ${LIB_UUID} ${LIB_MPF})
target_link_libraries(main ${LIB_FDAM} ${LIB_OPAE} ${LIB_UUID} ${LIB_MPF})
