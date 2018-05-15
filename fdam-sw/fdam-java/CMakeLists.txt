project("fdam")

cmake_minimum_required(VERSION 2.8)

find_package(SWIG REQUIRED)
find_package(Java REQUIRED)
find_package(JNI REQUIRED)

find_library(LIB_OPAE NAMES opae-c REQUIRED)
find_library(LIB_OPAE_ASE NAMES opae-c-ase REQUIRED)
find_library(LIB_UUID NAMES uuid REQUIRED)
find_library(LIB_MPF NAMES MPF REQUIRED)
find_library(LIB_FDAM NAMES fdam REQUIRED)

include(UseJava)
include(UseSWIG)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_PREFIX_PATH  ${CMAKE_INSTALL_PREFIX})
include_directories(${JNI_INCLUDE_DIRS})
include_directories("${CMAKE_PREFIX_PATH}/include")

set(libname fdam-ase-jni)
configure_file(swig/fdam.i.in swig/fdam-ase.i @ONLY)
set(libname fdam-jni)
configure_file(swig/fdam.i.in swig/fdam.i @ONLY)

set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/swig/fdam-ase.i PROPERTIES CPLUSPLUS ON)
set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/swig/fdam.i PROPERTIES CPLUSPLUS ON)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/ase)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/harp)

set(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/ase)
set(SWIG_OUTFILE_DIR ${CMAKE_CURRENT_BINARY_DIR}/ase)

swig_add_library(
        fdam-ase-jni
        LANGUAGE
        java
        SOURCES
        ${CMAKE_CURRENT_BINARY_DIR}/swig/fdam-ase.i
)
swig_link_libraries(fdam-ase-jni ${LIB_FDAM} ${LIB_OPAE_ASE} ${LIB_UUID} ${LIB_MPF})
add_custom_command(
        TARGET fdam-ase-jni
        POST_BUILD
        COMMAND find ${CMAKE_SWIG_OUTDIR} -iname '*.java' > ${CMAKE_SWIG_OUTDIR}/files.txt
)
add_jar(
        fdam-ase
        SOURCES
        @${CMAKE_SWIG_OUTDIR}/files.txt
)
add_dependencies(fdam-ase fdam-ase-jni)


set(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/harp)
set(SWIG_OUTFILE_DIR ${CMAKE_CURRENT_BINARY_DIR}/harp)

swig_add_library(
        fdam-jni
        LANGUAGE
        java
        SOURCES
        ${CMAKE_CURRENT_BINARY_DIR}/swig/fdam.i
)
swig_link_libraries(fdam-jni ${LIB_FDAM} ${LIB_OPAE} ${LIB_UUID} ${LIB_MPF})
add_custom_command(
        TARGET fdam-jni
        POST_BUILD
        COMMAND find ${CMAKE_SWIG_OUTDIR} -iname '*.java' > ${CMAKE_SWIG_OUTDIR}/files.txt
)
add_jar(
        fdam
        SOURCES
        @${CMAKE_SWIG_OUTDIR}/files.txt
)
add_dependencies(fdam fdam-jni)

install_jar(fdam-ase ${CMAKE_INSTALL_PREFIX}/share/java/fdam)
install_jar(fdam ${CMAKE_INSTALL_PREFIX}/share/java/fdam)

get_property(LIB64 GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS)
if ("${LIB64}" STREQUAL "TRUE")
    set(LIB_DIR "lib64")
else()
    set(LIB_DIR "lib")
endif()

install(
    TARGETS fdam-jni fdam-ase-jni
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${LIB_DIR}
    ARCHIVE DESTINATION ${LIB_DIR}
)

# add_custom_command(
#     TARGET libfdam
#     POST_BUILD
#     COMMAND rm -rf *.so ase harp
# )
