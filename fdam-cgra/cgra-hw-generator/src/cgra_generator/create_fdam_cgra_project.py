from utils import *
from make_acc_management import make_acc_management
from cgra_generator.make_cgra_acc_user import make_cgra_acc_user
import os
import traceback

try:
    FDAM_BASEDIR = os.environ['FDAM_BASEDIR']
except:
    print("\033[1;31m")
    print("Set FDAM_BASEDIR environment variable and rerun the script!")
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
        prj_path = raw_input('Project path[%s/fdam-cgra/cgra-samples ]: ' % FDAM_BASEDIR)
        if prj_path == '':
            prj_path = FDAM_BASEDIR + '/fdam-cgra/cgra-samples'
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
                num_pe = int(raw_input('Number of PEs for CGRA %d: ' % i))
                if num_pe >= 4:
                    break
            except:
                pass
        while True:
            try:
                num_pe_io = int(raw_input('Number of PE-IO for CGRA %d: ' % i))
                if num_pe_io > 0:
                    break
            except:
                pass
        while True:
            try:
                radix = int(raw_input('Net Radix for CGRA %d: ' % i))
                if radix in [2, 4, 8, 16, 32]:
                    break
            except:
                pass
        while True:
            try:
                mem_conf_depth = int(raw_input('Instruction Memory Depth for CGRA %d: ' % i))
                if mem_conf_depth > 0:
                    break
            except:
                pass
        while True:
            try:
                data_width = int(raw_input('Data Width for CGRA %d: ' % i))
                if data_width in [2, 4, 8, 16, 32, 64]:
                    break
            except:
                pass

        cgra_array.append((num_pe, num_pe_io, radix, mem_conf_depth, data_width))

    return [prj_name, prj_path, isDebug, cgra_array]


def create_dir_project(path_project, numAcc):
    FDAM_BASEDIR = os.environ['FDAM_BASEDIR']
    cmd = 'cp -r %s/fdam-hw-generator/fdam-cgra-model ' % FDAM_BASEDIR + path_project
    result = commands_getoutput(cmd)

    if len(result) != 0:
        print('Failed to create directory for project!')

    cmd = 'mkdir -p ' + path_project + '/hw/rtl/fdam_gen'
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
        path_for_rtl = path_for_project + '/hw/rtl/fdam_gen'
        create_dir_project(path_for_project, len(cgra_array))
        acc_array = []
        for i in range(len(cgra_array)):
            acc_array.append((cgra_array[i][1], cgra_array[i][1]))
        acc_management = make_acc_management(acc_array)
        code = acc_management.to_verilog()
        split_modules(code, path_for_rtl)
        for i in range(len(acc_array)):
            num_pe = cgra_array[i][0]
            num_pe_io = cgra_array[i][1]
            radix = cgra_array[i][2]
            mem_conf_depth = cgra_array[i][3]
            data_width = cgra_array[i][4]
            path_from = path_for_project + '/hw/rtl/fdam_gen/fdam_acc_user_%d.v' % i
            cmd = 'rm %s' % path_from
            commands_getoutput(cmd)
            cgra = make_cgra_acc_user(i, num_pe, num_pe_io, radix, mem_conf_depth, data_width)
            code = cgra.to_verilog()
            split_modules(code, path_for_project + '/hw/rtl/acc%d' % i)
        return True

    except:
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
        exit(1)


if __name__ == '__main__':
    main()
