cmake_minimum_required(VERSION 3.5)

project(main)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_PREFIX_PATH $ENV{FAM_INSTALLDIR})

include_directories("${CMAKE_PREFIX_PATH}/include")

find_library(LIB_OPAE NAMES opae-c REQUIRED)
find_library(LIB_OPAE_ASE NAMES opae-c-ase REQUIRED)
find_library(LIB_UUID NAMES uuid REQUIRED)
find_library(LIB_MPF NAMES MPF REQUIRED)
find_library(LIB_FAM NAMES fam REQUIRED)
find_library(LIB_FAM_ASE NAMES fam-ase REQUIRED)

set(CPP_SRCS ${CMAKE_SOURCE_DIR}/src/main.cpp)

add_executable(main_ase ${CPP_SRCS})
add_executable(main ${CPP_SRCS})

target_link_libraries(main_ase ${LIB_FAM_ASE} ${LIB_OPAE_ASE} ${LIB_UUID} ${LIB_MPF})
target_link_libraries(main ${LIB_FAM} ${LIB_OPAE} ${LIB_UUID} ${LIB_MPF})
