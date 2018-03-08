cmake_minimum_required(VERSION 3.9)
project(main)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lboost_program_options -luuid -lMPF -lopae-c-ase")

add_executable(main_ase main.cpp)
target_link_libraries(main_ase ${CMAKE_SOURCE_DIR}/../AFUManagerLIB/build/libafumanager.a)
