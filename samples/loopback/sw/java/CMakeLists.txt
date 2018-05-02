cmake_minimum_required(VERSION 3.5)

project(loopback)

include(UseJava)

find_package(Java REQUIRED)

set(FAM_LIB_JAR "$ENV{FAM_BASEDIR}/sw/build/java/libfam.jar")
set(FAM_LIB_ASE_JAR "$ENV{FAM_BASEDIR}/sw/build/java/libfam_ase.jar")
set(JAVA_SRCS ${CMAKE_SOURCE_DIR}/src/Loopback.java)

file(GENERATE OUTPUT fixed_jar_ase.sh 
     CONTENT
"
#!/bin/sh
${Java_JAR_EXECUTABLE} -xvf ${FAM_LIB_ASE_JAR}
ls -1 *.class *.so > ase_classes.txt
${Java_JAR_EXECUTABLE} uf ${CMAKE_CURRENT_BINARY_DIR}/LoopbackAse.jar @ase_classes.txt
rm -rf *.class *.so ase_classes.txt fixed_jar_ase.sh META-INF
"
)
file(GENERATE OUTPUT fixed_jar_harp.sh 
     CONTENT
"
#!/bin/sh
${Java_JAR_EXECUTABLE} -xvf ${FAM_LIB_JAR}
ls -1 *.class *.so > harp_classes.txt
${Java_JAR_EXECUTABLE} uf ${CMAKE_CURRENT_BINARY_DIR}/Loopback.jar @harp_classes.txt
rm -rf *.class *.so harp_classes.txt fixed_jar_harp.sh META-INF
"
)

add_jar(
    LoopbackAse 
    SOURCES ${JAVA_SRCS}
    INCLUDE_JARS ${FAM_LIB_ASE_JAR}
    ENTRY_POINT Loopback
)

add_jar(
    Loopback
    SOURCES ${JAVA_SRCS}
    INCLUDE_JARS ${FAM_LIB_JAR}
    ENTRY_POINT Loopback
)

add_custom_command(
    TARGET LoopbackAse
    POST_BUILD
    COMMAND sh fixed_jar_ase.sh
)

add_custom_command(
    TARGET Loopback
    POST_BUILD
    COMMAND sh fixed_jar_harp.sh
)

