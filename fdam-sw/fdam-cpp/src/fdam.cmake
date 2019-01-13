
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

file(
    GLOB_RECURSE
    FDAM_ACC_HDR_CXX
    ${PROJECT_SOURCE_DIR}/include/fdam/acc/*.h
    )
file(
    GLOB_RECURSE
    FDAM_CGRA_HDR_CXX
    ${PROJECT_SOURCE_DIR}/include/fdam/cgra/*.h
    )
    
file(
    GLOB_RECURSE
    FDAM_ACC_CXX
    ${PROJECT_SOURCE_DIR}/src/acc/*.cpp
    )
file(
    GLOB_RECURSE
    FDAM_CGRA_CXX
    ${PROJECT_SOURCE_DIR}/src/cgra/*.cpp
    )
    

add_library(fdam SHARED ${FDAM_CGRA_CXX} ${FDAM_ACC_CXX})

get_property(LIB64 GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS)
if ("${LIB64}" STREQUAL "TRUE")
    set(LIB_DIR "lib64")
else()
    set(LIB_DIR "lib")
endif()

install(
    TARGETS fdam
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIB_DIR}
    ARCHIVE DESTINATION ${LIB_DIR}
    )
        
install(FILES ${FDAM_ACC_HDR_CXX} DESTINATION include/fdam/acc)
install(FILES ${FDAM_CGRA_HDR_CXX} DESTINATION include/fdam/cgra)

##
## Add pthreads to the generated library.  VTP uses a mutex to guarantee
## that only one allocation happens at a time.
##
find_package(Threads REQUIRED)
if(CMAKE_THREAD_LIBS_INIT)
    target_link_libraries(fdam "${CMAKE_THREAD_LIBS_INIT}")
endif()

if(OPAELIB_LIBS_PATH)
    target_link_libraries(fdam OpaeLib)
endif()
