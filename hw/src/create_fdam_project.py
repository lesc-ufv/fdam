import os
import shutil
import traceback

from common.utils import *
from make_acc_management import make_acc_management

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
    print("#              FDAM Project Creator               #")
    print("###################################################")
    end_green_fontcolor()
    while True:
        prj_name = raw_input('Project name: ')
        if prj_name != '': break

    while True:
        prj_path = raw_input('Project path[%s/fdam-samples/fdam-acc-samples ]: ' % FDAM_BASEDIR)
        if prj_path == '':
            prj_path = FDAM_BASEDIR + '/fdam-samples/fdam-acc-samples'
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
        acc_array.append((nin, nout, None))

    return [prj_name, prj_path, isDebug, acc_array]


def create_dir_project(path_project, numAcc):
    FDAM_BASEDIR = os.environ['FDAM_BASEDIR']
    cmd = 'cp -r %s/fdam-hw-generator/fdam-model ' % FDAM_BASEDIR + path_project
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


def create_fdam_project(prj_name, prj_path, acc_array):
    try:
        path_for_project = prj_path + '/' + prj_name
        path_for_rtl = path_for_project + '/hw/rtl/acc_mnt'
        create_dir_project(path_for_project, len(acc_array))
        acc_management = make_acc_management(acc_array)
        code = acc_management.to_verilog()
        split_modules(code, path_for_rtl)
        for i in range(len(acc_array)):
            path_from = path_for_project + '/hw/rtl/acc_mnt/fdam_acc_user_%d.v' % i
            path_to = path_for_project + '/hw/rtl/acc%d' % i
            cmd = 'mv %s %s' % (path_from, path_to)
            commands_getoutput(cmd)

        files = os.listdir(path_for_rtl)
        files.sort()
        for f in files:
            name = f.split('_')
            if (name[0] != 'fdam'):
                src = path_for_rtl + '/' + f
                dst = path_for_project + '/hw/rtl/common/' + f
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
        print('FDAM project created successfully!')
        end_green_fontcolor()
    else:
        begin_red_fontcolor()
        print('Failed to create FDAM project!')
        end_red_fontcolor()
        cmd = 'rm -r %s' % (prj_path + '/' + prj_name)
        commands_getoutput(cmd)
        exit(1)


if __name__ == '__main__':
    main()
