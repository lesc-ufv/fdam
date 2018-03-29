cmake_minimum_required(VERSION 3.9)

project(FamLIB)

set(CMAKE_CXX_STANDARD 11)
#Descomentar alinha abaixo para compilar no harp2
#set(CMAKE_PREFIX_PATH "/upb/departments/pc2/users/h/h2lucas/usr/local")

include_directories(
		"${CMAKE_SOURCE_DIR}/lib"
		"${CMAKE_SOURCE_DIR}/include"
		"${CMAKE_PREFIX_PATH}/include"
)

add_library(
		fam SHARED
		${CMAKE_SOURCE_DIR}/lib/AccManagement.cpp
		${CMAKE_SOURCE_DIR}/lib/Accelerator.cpp
		${CMAKE_SOURCE_DIR}/lib/opae_svc_wrapper.cpp
		${CMAKE_SOURCE_DIR}/include/AccManagement.h
		${CMAKE_SOURCE_DIR}/include/Accelerator.h
		${CMAKE_SOURCE_DIR}/include/AccManagementDEFS.h
		${CMAKE_SOURCE_DIR}/include/csr_mgr.h
		${CMAKE_SOURCE_DIR}/include/opae_svc_wrapper.h
)


#add_executable(main_ase test/main.cpp)
#add_executable(main test/main.cpp)
#target_link_libraries(main_ase ${CMAKE_SOURCE_DIR}/cmake-build-debug/libfam.so ${LIB_OPAE_ASE} ${LIB_UUID} ${LIB_MPF})
#target_link_libraries(main ${CMAKE_SOURCE_DIR}/cmake-build-debug/libfam.so ${LIB_OPAE} ${LIB_UUID} ${LIB_MPF})
