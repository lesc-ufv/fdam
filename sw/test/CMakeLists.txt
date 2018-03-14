cmake_minimum_required(VERSION 3.5)

project(main)

set(CMAKE_CXX_STANDARD 11)

#Descomentar a linha abaixo para compilar no HARP2:
#set(CMAKE_PREFIX_PATH "/upb/departments/pc2/users/h/h2lucas/usr/local")

include_directories(
	"${CMAKE_PREFIX_PATH}/include"
	"${CMAKE_PREFIX_PATH}/lib"
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

add_executable(main_ase main.cpp)
add_executable(main main.cpp)
target_link_libraries(main_ase ${CMAKE_SOURCE_DIR}/../AFUManagerLIB/libafumanager.a ${LIB_OPAE_ASE} ${LIB_UUID} ${LIB_MPF})
target_link_libraries(main ${CMAKE_SOURCE_DIR}/../AFUManagerLIB/libafumanager.a ${LIB_OPAE} ${LIB_UUID} ${LIB_MPF})
