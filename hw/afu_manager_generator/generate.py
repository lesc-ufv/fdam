from make_acc_management import make_acc_management

acc_array = [(1, 1)]

acc_managent = make_acc_management(acc_array)

path_for_rtl = "/opt/intelFPGA/intel-fpga-bbb/samples/tutorial/16_loopback_1x1_acc_manager/hw/rtl"

acc_managent.to_verilog(path_for_rtl)
