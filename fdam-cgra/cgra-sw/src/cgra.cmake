
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

file(
    GLOB
    HDR_CXX
    ${PROJECT_SOURCE_DIR}/include/cgra/*.h
    )

aux_source_directory(
    ${PROJECT_SOURCE_DIR}/src
    LIBCGRA
    )


add_library(cgra SHARED ${LIBCGRA})

get_property(LIB64 GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS)
if ("${LIB64}" STREQUAL "TRUE")
    set(LIB_DIR "lib64")
else()
    set(LIB_DIR "lib")
endif()

install(
    TARGETS cgra
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIB_DIR}
    ARCHIVE DESTINATION ${LIB_DIR}
    )

install(FILES ${HDR_CXX} DESTINATION include/cgra)

##
## Add pthreads to the generated library.  VTP uses a mutex to guarantee
## that only one allocation happens at a time.
##
find_package(Threads REQUIRED)
if(CMAKE_THREAD_LIBS_INIT)
    target_link_libraries(cgra "${CMAKE_THREAD_LIBS_INIT}")
endif()

if(FDAMLIB_LIBS_PATH)
    target_link_libraries(cgra FdamLib)
endif()
