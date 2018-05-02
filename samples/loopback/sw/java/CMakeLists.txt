cmake_minimum_required(VERSION 3.5)

project(loopback)

include(UseJava)

find_package(Java REQUIRED)

set(FAM_LIB_JAR "$ENV{FAM_BASEDIR}/sw/build/java/AccManagementJNI.jar")
set(FAM_LIB_ASE_JAR "$ENV{FAM_BASEDIR}/sw/build/java/AccManagementAseJNI.jar")
set(JAVA_SRCS ${CMAKE_SOURCE_DIR}/src/Loopback.java)

add_custom_target(
    EXTRACT_FILES_ASE ALL
    COMMAND ${Java_JAR_EXECUTABLE} -xvf ${FAM_LIB_ASE_JAR}
)

add_custom_command(
     TARGET EXTRACT_FILES_ASE
     POST_BUILD
     COMMAND ls -1 *.class *.so > classes.txt
)

add_jar(
    LoopbackAse 
    SOURCES ${JAVA_SRCS}
    INCLUDE_JARS ${FAM_LIB_ASE_JAR}
    ENTRY_POINT Loopback
)

add_custom_command(
     TARGET LoopbackAse
     POST_BUILD
     COMMAND ${Java_JAR_EXECUTABLE} uf "${CMAKE_CURRENT_BINARY_DIR}/LoopbackAse.jar" @classes.txt
)

add_custom_command(
    TARGET LoopbackAse
    POST_BUILD
    COMMAND rm -rf classes.txt *.class *.so META-INF
)


add_custom_target(
    EXTRACT_FILES ALL
    COMMAND ${Java_JAR_EXECUTABLE} -xvf ${FAM_LIB_JAR}
)

add_custom_command(
     TARGET EXTRACT_FILES
     POST_BUILD
     COMMAND ls -1 *.class *.so > classes.txt
)

add_jar(
    Loopback
    SOURCES ${JAVA_SRCS}
    INCLUDE_JARS ${FAM_LIB_JAR}
    ENTRY_POINT Loopback
)

add_custom_command(
     TARGET Loopback
     POST_BUILD
     COMMAND ${Java_JAR_EXECUTABLE} uf "${CMAKE_CURRENT_BINARY_DIR}/Loopback.jar" @classes.txt
)

add_custom_command(
    TARGET Loopback
    POST_BUILD
    COMMAND rm -rf classes.txt *.class *.so META-INF
)

