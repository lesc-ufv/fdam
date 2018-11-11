from utils import *
from make_acc_management import make_acc_management
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
    print("#              FDAM Project Creator               #")
    print("###################################################")
    end_green_fontcolor()
    while True:
        prj_name = raw_input('Project name: ')
        if prj_name != '': break

    while True:
        prj_path = raw_input('Project path[%s/fdam-samples ]: ' % FDAM_BASEDIR)
        if prj_path == '':
            prj_path = FDAM_BASEDIR + '/fdam-samples'
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
    # while True:
    # d = raw_input('Debug project[yes/no]: ')
    # if d == 'yes':
    # isDebug =  True
    # break
    # elif d == 'no':
    # break

    while True:
        n = raw_input('Number of accelerators: ')
        try:
            num_acc = int(n)
            if num_acc > 0:
                break
        except:
            pass

    acc_array = []
    for i in range(num_acc):
        while True:
            try:
                nin, nout = map(int, raw_input('Number of input and output queues for acc %d [in out]:' % i).split())
                if nin > 0 and nout > 0:
                    break
            except:
                pass
        acc_array.append((nin, nout))

    return [prj_name, prj_path, isDebug, acc_array]


def create_dir_project(path_project, numAcc):
    FDAM_BASEDIR = os.environ['FDAM_BASEDIR']
    cmd = 'cp -r %s/fdam-hw-generator/fdam-model ' % FDAM_BASEDIR + path_project
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


def create_fdam_project(prj_name, prj_path, acc_array):
    try:
        path_for_project = prj_path + '/' + prj_name
        path_for_rtl = path_for_project + '/hw/rtl/fdam_gen'
        create_dir_project(path_for_project, len(acc_array))
        acc_management = make_acc_management(acc_array)
        code = acc_management.to_verilog()
        split_modules(code, path_for_rtl)
        for i in range(len(acc_array)):
            path_from = path_for_project + '/hw/rtl/fdam_gen/fdam_acc_user_%d.v' % i
            path_to = path_for_project + '/hw/rtl/acc%d' % i
            cmd = 'mv %s %s' % (path_from, path_to)
            commands_getoutput(cmd)

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
        print('FDAM project created successfully!')
        end_green_fontcolor()
    else:
        begin_red_fontcolor()
        print('Failed to create FDAM project!')
        end_red_fontcolor()
        print(traceback.format_exc())
        exit(1)


if __name__ == '__main__':
    main()
