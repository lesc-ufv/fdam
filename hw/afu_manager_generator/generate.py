from make_afu_manager import make_afu_manager

# (x,y) = (quantidade de entradas da afu, quantidade de saídas da afu)
numAFU = 4
afus = []
for i in range(numAFU):
    afus.append((1,1))

path = '../gen'

make_afu_manager(afus).to_verilog(path)
