cmake_minimum_required(VERSION 3.7)
project(sw)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I$ENV{FPGA_BBB_CCI_SRC}/samples/tutorial/base/sw -lopae-c-ase -lboost_program_options -luuid -lMPF")
set(SOURCE_FILES sw/main.cpp sw/AFU.cpp $ENV{FPGA_BBB_CCI_SRC}/samples/tutorial/base/sw/opae_svc_wrapper.cpp $ENV{FPGA_BBB_CCI_SRC}/samples/tutorial/base/sw/csr_mgr.h)
add_executable(sw ${SOURCE_FILES})