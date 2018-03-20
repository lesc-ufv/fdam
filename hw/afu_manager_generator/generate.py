from make_afu_manager import make_afu_manager

# (x,y) = (quantidade de entradas da afu, quantidade de saídas da afu)
numAFU = 1
afus = []
for i in range(numAFU):
    afus.append((1,1))

path = '../gen'

make_afu_manager(afus).to_verilog(path)

#from make_acc_management import make_acc_management

#acc_array = [(1 , 2),(1 , 1),(6 , 8),(12 , 12),(8 , 1),(1 , 4)]

#acc_managent = make_acc_management(acc_array)

#path_for_rtl = "<path>"

#acc_managent.to_verilog(path_for_rtl)
