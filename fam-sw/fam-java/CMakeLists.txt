project("fam")

cmake_minimum_required(VERSION 2.8)

find_package(SWIG REQUIRED)
find_package(Java REQUIRED)
find_package(JNI REQUIRED)

find_library(LIB_OPAE NAMES opae-c REQUIRED)
find_library(LIB_OPAE_ASE NAMES opae-c-ase REQUIRED)
find_library(LIB_UUID NAMES uuid REQUIRED)
find_library(LIB_MPF NAMES MPF REQUIRED)
find_library(LIB_FAM NAMES fam REQUIRED)

include(UseJava)
include(UseSWIG)

set(CMAKE_PREFIX_PATH  ${CMAKE_INSTALL_PREFIX})
include_directories(${JNI_INCLUDE_DIRS})
include_directories("${CMAKE_PREFIX_PATH}/include")

set(libname fam-ase-jni)
configure_file(swig/fam.i.in swig/fam-ase.i @ONLY)
set(libname fam-jni)
configure_file(swig/fam.i.in swig/fam.i @ONLY)

set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/swig/fam-ase.i PROPERTIES CPLUSPLUS ON)
set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/swig/fam.i PROPERTIES CPLUSPLUS ON)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/ase)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/harp)

set(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/ase)
set(SWIG_OUTFILE_DIR ${CMAKE_CURRENT_BINARY_DIR}/ase)

swig_add_library(
        fam-ase-jni
        LANGUAGE
        java
        SOURCES
        ${CMAKE_CURRENT_BINARY_DIR}/swig/fam-ase.i
)
swig_link_libraries(fam-ase-jni ${LIB_FAM} ${LIB_OPAE_ASE} ${LIB_UUID} ${LIB_MPF})
add_custom_command(
        TARGET fam-ase-jni
        POST_BUILD
        COMMAND find ${CMAKE_SWIG_OUTDIR} -iname '*.java' > ${CMAKE_SWIG_OUTDIR}/files.txt
)
add_jar(
        fam-ase
        SOURCES
        @${CMAKE_SWIG_OUTDIR}/files.txt
)
add_dependencies(fam-ase fam-ase-jni)


set(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/harp)
set(SWIG_OUTFILE_DIR ${CMAKE_CURRENT_BINARY_DIR}/harp)

swig_add_library(
        fam-jni
        LANGUAGE
        java
        SOURCES
        ${CMAKE_CURRENT_BINARY_DIR}/swig/fam.i
)
swig_link_libraries(fam-jni ${LIB_FAM} ${LIB_OPAE} ${LIB_UUID} ${LIB_MPF})
add_custom_command(
        TARGET fam-jni
        POST_BUILD
        COMMAND find ${CMAKE_SWIG_OUTDIR} -iname '*.java' > ${CMAKE_SWIG_OUTDIR}/files.txt
)
add_jar(
        fam
        SOURCES
        @${CMAKE_SWIG_OUTDIR}/files.txt
)
add_dependencies(fam fam-jni)

install_jar(fam-ase ${CMAKE_INSTALL_PREFIX}/share/java/fam)
install_jar(fam ${CMAKE_INSTALL_PREFIX}/share/java/fam)

get_property(LIB64 GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS)
if ("${LIB64}" STREQUAL "TRUE")
    set(LIB_DIR "lib64")
else()
    set(LIB_DIR "lib")
endif()

install(
    TARGETS fam-jni fam-ase-jni
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIB_DIR}
    ARCHIVE DESTINATION ${LIB_DIR}
)

# add_custom_command(
#     TARGET libfam
#     POST_BUILD
#     COMMAND rm -rf *.so ase harp
# )
