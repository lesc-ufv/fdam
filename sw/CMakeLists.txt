cmake_minimum_required(VERSION 3.9)

set(DESC "Framework for accelerator management is a library that encapsulates calls to the Open Programmable Engine OPAE to facilitate the use of FPGAs as accelerators in heterogeneous systems providing a simple hardware and software interface with a focus on running dataflow")

project(Fam VERSION 0.0.0 DESCRIPTION ${DESC})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall -fPIC")
set(OPAE_INSTALLDIR "$ENV{OPAE_BASEDIR}/install")
set(CMAKE_PREFIX_PATH ${OPAE_INSTALLDIR})
set(CPPLIB_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/cpp/include/")

find_package(SWIG REQUIRED)
find_package(Java REQUIRED)
find_package(JNI REQUIRED)
find_library(LIB_OPAE NAMES opae-c)
find_library(LIB_OPAE_ASE NAMES opae-c-ase)
find_library(LIB_UUID NAMES uuid)
find_library(LIB_MPF NAMES MPF)

include_directories(
        "${CPPLIB_INCLUDE_DIR}"
        "${CMAKE_PREFIX_PATH}/include"
)

add_subdirectory(cpp)
add_subdirectory(java)
