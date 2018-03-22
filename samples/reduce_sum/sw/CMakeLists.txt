cmake_minimum_required(VERSION 3.5)

project(main)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

#Descomentar a linha abaixo para compilar no harp2
#set(CMAKE_PREFIX_PATH "/upb/departments/pc2/users/h/h2lucas/usr/local")
set(LIB_AFU_MGR "${CMAKE_SOURCE_DIR}/../../../sw/AFUManagerLIB/build/libafumanager.a")

include_directories(
	"${CMAKE_PREFIX_PATH}/include"
	"${CMAKE_SOURCE_DIR}/../../../sw/AFUManagerLIB/include"
	"${CMAKE_SOURCE_DIR}/../../common"
)
find_library(LIB_OPAE
    NAMES opae-c
)
find_library(LIB_OPAE_ASE
    NAMES opae-c-ase
)
find_library(LIB_UUID
    NAMES uuid 
)
find_library(LIB_MPF
    NAMES MPF 
)

add_executable(main_ase main.cpp  ${CMAKE_SOURCE_DIR}/../../common/dataflow_exec.cpp)
add_executable(main main.cpp ${CMAKE_SOURCE_DIR}/../../common/dataflow_exec.cpp)
target_link_libraries(main_ase  ${LIB_AFU_MGR}  ${LIB_OPAE_ASE} ${LIB_UUID} ${LIB_MPF})
target_link_libraries(main ${LIB_AFU_MGR} ${LIB_OPAE} ${LIB_UUID} ${LIB_MPF})
