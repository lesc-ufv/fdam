from scripts.utils import *


def create_dir_project(path_project, numAcc):
    cmd = 'mkdir -p ' + path_project + '/sw'
    result = commands_getoutput(cmd)
    if len(result) != 0:
        print('Failed to create directory for project!')
    cmd = 'mkdir -p ' + path_project + '/hw/sim'
    result = commands_getoutput(cmd)
    if len(result) != 0:
        print('Failed to create directory for project!')
    cmd = 'mkdir -p ' + path_project + '/hw/synth'
    result = commands_getoutput(cmd)
    if len(result) != 0:
        print('Failed to create directory for project!')
    cmd = 'mkdir -p ' + path_project + '/hw/rtl'
    result = commands_getoutput(cmd)
    if len(result) != 0:
        print('Failed to create directory for project!')

    for i in range(numAcc):
        cmd = 'mkdir -p ' + path_project + '/hw/rtl/acc%d'%i
        result = commands_getoutput(cmd)
        if len(result) != 0:
            print('Failed to create directory for project!')

    create_setup_ase_file(path_project)
    create_setup_synth_file(path_project)

def create_setup_ase_file(path_project):
    f = path_project + '/hw/sim/setup_ase'
    fd = open(f, 'w')
    str_file = '#!/bin/sh\n\nrm -rf ase_sim\n$FAM_BASEDIR/hw/fam-generator/scripts/update_sources.py\nafu_sim_setup --sources=sources.txt afu_sim\n'
    fd.write(str_file)
    fd.close()
    commands_getoutput('chmod +x ' + f)


def create_setup_synth_file(path_project):
    f = path_project + '/hw/synth/setup_synth'
    fd = open(f, 'w')
    str_file = '#!/bin/sh\n\nrm -rf fam_synth\n$FAM_BASEDIR/hw/fam-generator/scripts/update_sources.py\nafu_synth_setup  -s sources.txt --lib $AALSDK_BASEDIR/hw/lib afu_synth\n'
    fd.write(str_file)
    fd.close()
    commands_getoutput('chmod +x ' + f)


def create_cmake_file(path_project):
    f = open(path_project + '/sw/CMakeLists.txt', 'w')
    str = 'cmake_minimum_required(VERSION 3.5)\n\nproject(main)\n\nset(CMAKE_CXX_STANDARD 11)\nset(FAM_BASEDIR $ENV{FAM_BASEDIR})\nset(FAM_LIB "${FAM_BASEDIR}/sw/build/cpp/libfam.so")\nset(FAM_LIB_ASE "${FAM_BASEDIR}/sw/build/cpp/libfam-ase.so")\nset(OPAE_INSTALLDIR "$ENV{OPAE_BASEDIR}/install")\nset(CMAKE_PREFIX_PATH ${OPAE_INSTALLDIR})\n\ninclude_directories("${CMAKE_PREFIX_PATH}/include" "${FAM_BASEDIR}/sw/cpp/include")\n\nfind_library(LIB_OPAE NAMES opae-c)\nfind_library(LIB_OPAE_ASE NAMES opae-c-ase)\nfind_library(LIB_UUID NAMES uuid)\nfind_library(LIB_MPF NAMES MPF)\n\nadd_executable(main_ase main.cpp)\nadd_executable(main main.cpp)\n\ntarget_link_libraries(main_ase ${FAM_LIB_ASE} ${LIB_OPAE_ASE} ${LIB_UUID} ${LIB_MPF})\ntarget_link_libraries(main ${FAM_LIB} ${LIB_OPAE} ${LIB_UUID} ${LIB_MPF})\n'
    f.write(str)
    f.close()


def create_main_file(path_project):
    f = open(path_project + '/sw/main.cpp', 'w')
    str = '#include <AccManagement.h>\n\nusing namespace std;\n\nint main(int argc, char *argv[]) {\n\n   auto *accMgr = new AccManagement();\n   accMgr->printHwInfo();\n\n   delete accMgr;\n\n}'
    f.write(str)
    f.close()
