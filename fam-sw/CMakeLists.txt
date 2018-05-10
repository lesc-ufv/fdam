cmake_minimum_required(VERSION 3.9)

set(DESC "Framework for accelerator management is a library that encapsulates calls to the Open Programmable Engine OPAE to facilitate the use of FPGAs as accelerators in heterogeneous systems providing a simple hardware and software interface with a focus on running dataflow")

project(Fam VERSION 0.0.0 DESCRIPTION ${DESC})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall -fPIC")
set(CMAKE_PREFIX_PATH ${CMAKE_INSTALL_PREFIX})
set(CPPLIB_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/cpp/include/")

find_package(SWIG REQUIRED)
find_package(Java REQUIRED)
find_package(JNI REQUIRED)

find_library(LIB_OPAE NAMES opae-c REQUIRED)
find_library(LIB_OPAE_ASE NAMES opae-c-ase REQUIRED)
find_library(LIB_UUID NAMES uuid REQUIRED)
find_library(LIB_MPF NAMES MPF REQUIRED)

include_directories("${CPPLIB_INCLUDE_DIR}" "${CMAKE_PREFIX_PATH}/include")

add_subdirectory(cpp)
add_subdirectory(java)
