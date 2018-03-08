cmake_minimum_required(VERSION 3.9)

project(AFUManager)

include_directories(
	"${CMAKE_SOURCE_DIR}"
)

set(CMAKE_CXX_STANDARD 11)

add_library(
    afumanager
    AFU.cpp
    AFU.h
    AFUManager.cpp
    AFUManager.h
    AFUManagerDEFS.h
    csr_mgr.h
    opae_svc_wrapper.cpp
    opae_svc_wrapper.h
)
