cmake_minimum_required(VERSION 3.5)

project(AFUManager)

set(CMAKE_CXX_STANDARD 11)
#Descomentar alinha abaixo para compilar no harp2
set(CMAKE_PREFIX_PATH "/upb/departments/pc2/users/h/h2lucas/usr/local")

include_directories(
	"${CMAKE_SOURCE_DIR}/lib"	
	"${CMAKE_SOURCE_DIR}/include"
	"${CMAKE_PREFIX_PATH}/include"
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
