from make_afu_manager import make_afu_manager

# (x,y) = (quantidade de entradas da afu, quantidade de saídas da afu)
afus = [(9,9)]
#path = '/opt/intelFPGA/intel-fpga-bbb/samples/tutorial/07_loopback_1x1/hw/rtl_gen'
path = '../gen_test'
make_afu_manager(afus).to_verilog(path)
