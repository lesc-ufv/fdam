import os
import shutil
import traceback

from common.utils import *
from fdam_acc.make_acc_management import make_acc_management
from fdam_cgra.make_cgra_accelerator import make_cgra_accelerator

try:
    FDAM_BASEDIR = os.environ['FDAM_BASEDIR']
except:
    print("\033[1;31m")
    print("FDAM environment not configured, run the source command in the setup.fdam file in the scripts folder!")
    print("\033[1;m")
    exit(1)


def create_fdam_project_cli():
    begin_green_fontcolor()
    print("###################################################")
    print("#              FDAM-CGRA Project Creator          #")
    print("###################################################")
    end_green_fontcolor()
    while True:
        prj_name = raw_input('Project name: ')
        if prj_name != '': break

    while True:
        prj_path = raw_input('Project path[%s/fdam-samples/fdam-cgra-samples ]: ' % FDAM_BASEDIR)
        if prj_path == '':
            prj_path = FDAM_BASEDIR + '/fdam-samples/fdam-cgra-samples/'
            if not os.path.exists(prj_path + '/' + prj_name):
                break
            else:
                begin_red_fontcolor()
                print('This project already exists!')
                end_red_fontcolor()
                exit(0)
        else:
            prj_path = os.path.abspath(prj_path)
            if not os.path.exists(prj_path + '/' + prj_name):
                break
            else:
                print('This project already exists!')

    isDebug = False

    while True:
        n = raw_input('Number of CGRAs: ')
        try:
            num_acc = int(n)
            if num_acc > 0:
                break
        except:
            pass

    cgra_array = []
    for i in range(num_acc):
        while True:
            try:
                num_pe = int(raw_input('Number of PEs for CGRA %d[4, 8, 16, 32, 64, 128, 256, 512]: ' % i))
                if num_pe in [4, 8, 16, 32, 64, 128, 256, 512]:
                    break
            except:
                pass
        while True:
            try:
                num_pe_io_in = int(raw_input('Number of PE-IN for CGRA %d[1 .. %d]: ' % (i, num_pe - 1)))
                if num_pe_io_in > 0 and num_pe_io_in <= num_pe - 1:
                    break
            except:
                pass
        while True:
            try:
                num_pe_io_out = int(raw_input('Number of PE-OUT for CGRA %d[1 .. %d]: ' % (i, num_pe - num_pe_io_in)))
                if num_pe_io_out > 0 and num_pe_io_out <= (num_pe - num_pe_io_in):
                    break
            except:
                pass
        while True:
            try:
                radix = int(raw_input('Net Radix for CGRA %d[2, 4, 8]: ' % i))
                if radix in [2, 4, 8]:
                    break
            except:
                pass
        while True:
            try:
                mem_conf_depth = int(raw_input('Instruction Memory Depth for CGRA %d[1 .. 12]: ' % i))
                if mem_conf_depth > 0 and mem_conf_depth < 12:
                    break
            except:
                pass
        while True:
            try:
                data_width = int(raw_input('Data Width for CGRA %d[1, 2, 4, 8, 16, 32, 64]: ' % i))
                if data_width in [1, 2, 4, 8, 16, 32, 64]:
                    break
            except:
                pass

        cgra_array.append((num_pe, num_pe_io_in, num_pe_io_out, radix, mem_conf_depth, data_width))

    return [prj_name, prj_path, isDebug, cgra_array]


def create_dir_project(path_project, numAcc):
    FDAM_BASEDIR = os.environ['FDAM_BASEDIR']
    cmd = 'cp -r %s/fdam-hw-generator/fdam-cgra-model ' % FDAM_BASEDIR + path_project
    result = commands_getoutput(cmd)

    if len(result) != 0:
        print('Failed to create directory for project!')

    cmd = 'mkdir -p ' + path_project + '/hw/rtl/acc_mnt'
    result = commands_getoutput(cmd)
    if len(result) != 0:
        print('Failed to create directory for project!')

    cmd = 'mkdir -p ' + path_project + '/hw/rtl/common'
    result = commands_getoutput(cmd)
    if len(result) != 0:
        print('Failed to create directory for project!')

    for i in range(numAcc):
        cmd = 'mkdir -p ' + path_project + '/hw/rtl/acc%d' % i
        result = commands_getoutput(cmd)
        if len(result) != 0:
            print('Failed to create directory for project!')


def create_fdam_project(prj_name, prj_path, cgra_array):
    try:
        path_for_project = prj_path + '/' + prj_name
        path_for_rtl = path_for_project + '/hw/rtl/acc_mnt/'
        create_dir_project(path_for_project, len(cgra_array))
        acc_array = []
        for i in range(len(cgra_array)):
            num_pe = cgra_array[i][0]
            num_pe_io_in = cgra_array[i][1]
            num_pe_io_out = cgra_array[i][2]
            radix = cgra_array[i][3]
            mem_conf_depth = cgra_array[i][4]
            data_width = cgra_array[i][5]
            cgra_acc = make_cgra_accelerator(i, num_pe, num_pe_io_in, num_pe_io_out, data_width, radix, mem_conf_depth)
            acc_array.append((cgra_array[i][1], cgra_array[i][2], cgra_acc))
        acc_management = make_acc_management(acc_array)
        code = acc_management.to_verilog()
        split_modules(code, path_for_rtl)
        files = os.listdir(path_for_rtl)
        files.sort()
        for f in files:
            name = f.split('_')
            if name[0] != 'fdam':
                src = path_for_rtl + '/' + f
                dst = path_for_project + '/hw/rtl/common/' + f
                shutil.move(src, dst)

        path_for_rtl = path_for_project + '/hw/rtl/common/'
        files = os.listdir(path_for_rtl)
        files.sort()
        for i in range(len(cgra_array)):
            for f in files:
                name = f.split('_')
                if name[0] == 'cgra%d' % i:
                    src = path_for_rtl + f
                    dst = path_for_project + '/hw/rtl/acc%d/' % i + f
                    shutil.move(src, dst)

        return True

    except:
        print(traceback.format_exc())
        return False


def main():
    prj = create_fdam_project_cli()
    prj_name = prj[0]
    prj_path = prj[1]
    isDebug = prj[2]
    acc_array = prj[3]

    if create_fdam_project(prj_name, prj_path, acc_array):
        begin_green_fontcolor()
        print('FDAM-CGRA project created successfully!')
        end_green_fontcolor()
    else:
        begin_red_fontcolor()
        print('Failed to create FDAM-CGRA project!')
        end_red_fontcolor()
        print(traceback.format_exc())
        cmd = 'rm -r %s' % (prj_path + '/' + prj_name)
        commands_getoutput(cmd)
        exit(1)

if __name__ == '__main__':
    main()

