from make_acc_management import make_acc_management

acc_array = [(1,1)]

acc_management = make_acc_management(acc_array)

path_for_rtl = "../gen"

acc_management.to_verilog(path_for_rtl)
