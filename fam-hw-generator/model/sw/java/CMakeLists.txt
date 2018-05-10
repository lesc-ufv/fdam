cmake_minimum_required(VERSION 3.11)

project(Main)

include(UseJava)

find_package(Java REQUIRED)

set(FAM_LIB_JAR "$ENV{FAM_INSTALLDIR}/share/java/fam/libfam.jar")
set(FAM_LIB_ASE_JAR  "$ENV{FAM_INSTALLDIR}/share/java/fam/libfam_ase.jar")

set(JAVA_SRCS ${CMAKE_SOURCE_DIR}/src/Main.java)

file(GENERATE OUTPUT fixed_jar_ase.sh 
     CONTENT
"
#!/bin/sh
${Java_JAR_EXECUTABLE} -xvf ${FAM_LIB_ASE_JAR} > /dev/null
ls -1 *.class *.so > ase_classes.txt
${Java_JAR_EXECUTABLE} uf ${CMAKE_CURRENT_BINARY_DIR}/MainAse.jar @ase_classes.txt
rm -rf *.class *.so ase_classes.txt fixed_jar_ase.sh META-INF
"
)
file(GENERATE OUTPUT fixed_jar_harp.sh 
     CONTENT
"
#!/bin/sh
${Java_JAR_EXECUTABLE} -xvf ${FAM_LIB_JAR} > /dev/null
ls -1 *.class *.so > harp_classes.txt
${Java_JAR_EXECUTABLE} uf ${CMAKE_CURRENT_BINARY_DIR}/Main.jar @harp_classes.txt
rm -rf *.class *.so harp_classes.txt fixed_jar_harp.sh META-INF
"
)

add_jar(
    MainAse 
    SOURCES ${JAVA_SRCS}
    INCLUDE_JARS ${FAM_LIB_ASE_JAR}
    ENTRY_POINT Main
)

add_jar(
    Main
    SOURCES ${JAVA_SRCS}
    INCLUDE_JARS ${FAM_LIB_JAR}
    ENTRY_POINT Main
)

add_custom_command(
    TARGET MainAse
    POST_BUILD
    COMMAND sh fixed_jar_ase.sh
)

add_custom_command(
    TARGET Main
    POST_BUILD
    COMMAND sh fixed_jar_harp.sh
)

