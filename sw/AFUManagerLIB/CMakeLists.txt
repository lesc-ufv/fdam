cmake_minimum_required(VERSION 3.5)

project(AFUManager)

set(CMAKE_CXX_STANDARD 11)

include_directories(
	"${CMAKE_SOURCE_DIR}/lib"	
	"${CMAKE_SOURCE_DIR}/include"
)
add_library(
    afumanager
    lib/AFU.cpp
    include/AFU.h
    lib/AFUManager.cpp
    include/AFUManager.h
    include/AFUManagerDEFS.h
    include/csr_mgr.h
    lib/opae_svc_wrapper.cpp
    include/opae_svc_wrapper.h
)
