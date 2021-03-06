cmake_minimum_required(VERSION 2.8)

project(Main)

include(UseJava)

find_package(Java REQUIRED)

set(FDAM_LIB_JAR "${CMAKE_PREFIX_PATH}/share/java/fdam/fdam.jar")
set(FDAM_LIB_ASE_JAR  "${CMAKE_PREFIX_PATH}/share/java/fdam/fdam-ase.jar")

file(GLOB JAVA_SRCS ${CMAKE_SOURCE_DIR}/src/*.java)

file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/fixed_jar_ase.sh 
     CONTENT
"
#!/bin/sh
${Java_JAR_EXECUTABLE} -xvf ${FDAM_LIB_ASE_JAR} > /dev/null
ls -1 *.class > ase_classes.txt
${Java_JAR_EXECUTABLE} uf ${CMAKE_CURRENT_BINARY_DIR}/MainAse.jar @ase_classes.txt
rm -rf *.class ase_classes.txt ${CMAKE_CURRENT_BINARY_DIR}/fixed_jar_ase.sh META-INF
"
)
file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/fixed_jar_harp.sh 
     CONTENT
"
#!/bin/sh
${Java_JAR_EXECUTABLE} -xvf ${FDAM_LIB_JAR} > /dev/null
ls -1 *.class > harp_classes.txt
${Java_JAR_EXECUTABLE} uf ${CMAKE_CURRENT_BINARY_DIR}/Main.jar @harp_classes.txt
rm -rf *.class harp_classes.txt ${CMAKE_CURRENT_BINARY_DIR}/fixed_jar_harp.sh META-INF
"
)

add_jar(
    MainAse 
    SOURCES ${JAVA_SRCS}
    INCLUDE_JARS ${FDAM_LIB_ASE_JAR}
    ENTRY_POINT Main
)

add_jar(
    Main
    SOURCES ${JAVA_SRCS}
    INCLUDE_JARS ${FDAM_LIB_JAR}
    ENTRY_POINT Main
)

add_custom_command(
    TARGET MainAse
    POST_BUILD
    COMMAND sh ${CMAKE_CURRENT_BINARY_DIR}/fixed_jar_ase.sh
)

add_custom_command(
    TARGET Main
    POST_BUILD
    COMMAND sh ${CMAKE_CURRENT_BINARY_DIR}/fixed_jar_harp.sh
)

