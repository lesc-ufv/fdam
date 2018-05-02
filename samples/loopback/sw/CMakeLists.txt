cmake_minimum_required(VERSION 3.5)

project(main)

set(CMAKE_CXX_STANDARD 11)
set(FAM_BASEDIR $ENV{FAM_BASEDIR})
set(FAM_LIB "${FAM_BASEDIR}/sw/build/cpp/libfam.so")
set(FAM_LIB_ASE "${FAM_BASEDIR}/sw/build/cpp/libfam-ase.so")
set(OPAE_INSTALLDIR "$ENV{OPAE_BASEDIR}/install")
set(CMAKE_PREFIX_PATH ${OPAE_INSTALLDIR})

include_directories("${CMAKE_PREFIX_PATH}/include" "${FAM_BASEDIR}/sw/cpp/include")

find_library(LIB_OPAE NAMES opae-c)
find_library(LIB_OPAE_ASE NAMES opae-c-ase)
find_library(LIB_UUID NAMES uuid)
find_library(LIB_MPF NAMES MPF)

add_executable(main_ase main.cpp)
add_executable(main main.cpp)

target_link_libraries(main_ase ${FAM_LIB_ASE} ${LIB_OPAE_ASE} ${LIB_UUID} ${LIB_MPF})
target_link_libraries(main ${FAM_LIB} ${LIB_OPAE} ${LIB_UUID} ${LIB_MPF})
