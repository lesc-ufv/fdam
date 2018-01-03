cmake_minimum_required(VERSION 3.7)
project(sw)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lopae-c-ase -lboost_program_options -luuid -lMPF")
set(SOURCE_FILES test/main.cpp AFUManagerLIB/AFUManager.cpp AFUManagerLIB/AFUManagerDEFS.h AFUManagerLIB/AFU.cpp AFUManagerLIB/opae_svc_wrapper.cpp AFUManagerLIB/csr_mgr.h)
add_executable(sw ${SOURCE_FILES})