from make_afu_manager import make_afu_manager

# (x,y) = (quantidade de entradas da afu, quantidade de saídas da afu)
numAFU = 1
afus = []
for i in range(numAFU):
    afus.append((16, 16))

# path = '/opt/intelFPGA/intel-fpga-bbb/samples/tutorial/07_loopback_1x1/hw/rtl_gen'
path = '../gen'
make_afu_manager(afus).to_verilog(path)
