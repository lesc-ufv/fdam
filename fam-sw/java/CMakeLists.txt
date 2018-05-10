set(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR})

include(UseJava)
include(UseSWIG)

include_directories(${JNI_INCLUDE_DIRS})
include_directories("${CPPLIB_INCLUDE_DIR}" "${CMAKE_PREFIX_PATH}/include")

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/ase)
set(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/ase)
set(SWIG_OUTFILE_DIR ${CMAKE_CURRENT_BINARY_DIR}/ase)
set_source_files_properties(swig/AccManagementAse.i PROPERTIES CPLUSPLUS ON)
swig_add_library(
        java_fam_ase
        LANGUAGE
        java
        SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/swig/AccManagementAse.i
        ${CMAKE_SOURCE_DIR}/cpp/include/AccManagement.h
)
swig_link_libraries(java_fam_ase fam-ase)
add_custom_command(
        TARGET java_fam_ase
        POST_BUILD
        COMMAND find ${CMAKE_SWIG_OUTDIR} -iname '*.java' > ${CMAKE_SWIG_OUTDIR}/files.txt
)

add_jar(
        libfam_ase
        SOURCES
        @${CMAKE_SWIG_OUTDIR}/files.txt
)
add_dependencies(libfam_ase java_fam_ase)

add_custom_command(
        TARGET libfam_ase
        POST_BUILD
        COMMAND ${Java_JAR_EXECUTABLE} -uf "${CMAKE_CURRENT_BINARY_DIR}/libfam_ase.jar" "./libjava_fam_ase.so"
)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/harp)
set(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/harp)
set(SWIG_OUTFILE_DIR ${CMAKE_CURRENT_BINARY_DIR}/harp)
set_source_files_properties(swig/AccManagement.i PROPERTIES CPLUSPLUS ON)
swig_add_library(
        java_fam
        LANGUAGE
        java
        SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/swig/AccManagement.i
        ${CMAKE_SOURCE_DIR}/cpp/src/AccManagement.cpp
)
swig_link_libraries(java_fam fam)

add_custom_command(
        TARGET java_fam
        POST_BUILD
        COMMAND find ${CMAKE_SWIG_OUTDIR} -iname '*.java' > ${CMAKE_SWIG_OUTDIR}/files.txt
)
add_jar(
        libfam
        SOURCES
        @${CMAKE_SWIG_OUTDIR}/files.txt
)

add_dependencies(libfam java_fam)

add_custom_command(
        TARGET libfam
        POST_BUILD
        COMMAND ${Java_JAR_EXECUTABLE} -uf "${CMAKE_CURRENT_BINARY_DIR}/libfam.jar" "./libjava_fam.so"
)

install_jar(libfam_ase ${CMAKE_INSTALL_PREFIX}/share/java/fam)
install_jar(libfam ${CMAKE_INSTALL_PREFIX}/share/java/fam)

# add_custom_command(
#     TARGET libfam
#     POST_BUILD
#     COMMAND rm -rf *.so ase harp
# )
