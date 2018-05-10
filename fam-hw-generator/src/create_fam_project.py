import os
import sys
import traceback

if sys.version_info < (2, 7):
    import commands
else:
    pass

try:
    FAM_BASEDIR = os.environ['FAM_BASEDIR']
    sys.path.insert(0, FAM_BASEDIR + '/fam-hw-generator')
except:
    print("\033[1;31m")
    print("Set FAM_BASEDIR environment variable and rerun the script!")
    print("\033[1;m")
    exit(1)

from src.utils import *
from src.make_acc_management import make_acc_management

def create_fam_project():
    begin_green_fontcolor()
    print("###################################################")
    print("#              FAM Project Creator                #")
    print("###################################################")
    end_green_fontcolor()
    while True:
        prj_name = input('Project name: ')
        if prj_name != '': break

    while True:
        prj_path = input('Project path[%s/fam-samples ]: ' %FAM_BASEDIR)
        if prj_path == '':
            prj_path = FAM_BASEDIR + '/fam-samples'
            if not os.path.exists(prj_path + '/' + prj_name):
                break
            else:
                begin_red_fontcolor()
                print('This project already exists!')
                end_red_fontcolor()
                exit(0)
        elif not os.path.exists(prj_path + '/' + prj_name):
            break

    isDebug = False
    # while True:
    # d = input('Debug project[yes/no]: ')
    # if d == 'yes':
    # isDebug =  True
    # break
    # elif d == 'no':
    # break

    while True:
        n = input('Number of accelerators: ')
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
                nin, nout = map(int, input('Number of input and output queues for acc %d [in out]:' % i).split())
                if nin > 0 and nout > 0:
                    break
            except:
                pass
        acc_array.append((nin, nout))

    return [prj_name, prj_path, isDebug, acc_array]


def create_dir_project(path_project, numAcc):
    FAM_BASEDIR = os.environ['FAM_BASEDIR']
    cmd = 'cp -r %s/fam-hw-generator/model ' % FAM_BASEDIR + path_project
    result = commands_getoutput(cmd)

    if len(result) != 0:
        print('Failed to create directory for project!')

    for i in range(numAcc):
        cmd = 'mkdir -p ' + path_project + '/hw/rtl/acc%d' % i
        result = commands_getoutput(cmd)
        if len(result) != 0:
            print('Failed to create directory for project!')

def main():
    try:
        prj = create_fam_project()
        prj_name = prj[0]
        prj_path = prj[1]
        isDebug = prj[2]
        acc_array = prj[3]
        path_for_project = prj_path + '/' + prj_name
        path_for_rtl = path_for_project + '/hw/rtl/fam_gen'
        create_dir_project(path_for_project, len(acc_array))
        acc_management = make_acc_management(acc_array)
        acc_management.to_verilog(path_for_rtl)
        for i in range(len(acc_array)):
            path_from = path_for_project + '/hw/rtl/fam_gen/acc_user_%d.v'%i
            path_to = path_for_project + '/hw/rtl/acc%d'%i
            cmd = 'mv %s %s' % (path_from, path_to)
            commands_getoutput(cmd)

        begin_green_fontcolor()
        print('FAM project created successfully!')
        end_green_fontcolor()
    except:
        begin_red_fontcolor()
        print('Failed to create FAM project!')
        end_red_fontcolor()
        print(traceback.format_exc())
        exit(1)


if __name__ == '__main__':
    main()
