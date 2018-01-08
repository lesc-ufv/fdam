from make_afu_manager_controller import make_afu_manager_controller

cache_data_width = 512
fifo_depth = 5

# (x,y) = x -> quantidade de entradas da interface
#         y -> quantidade de saídas da interface
#afus = [(1, 1), (1, 1), (1, 1), (1, 1), (1, 1), (1, 1), (1, 1), (1, 1), (1, 1), (1, 1), (1, 1), (1, 1), (1, 1), (1, 1),
#        (1, 1), (1, 1), (1, 1), (1, 1), (1, 1), (1, 1), (1, 1), (1, 1), (1, 1), (1, 1), (1, 1), (1, 1), (1, 1), (1, 1),
#        (1, 1), (1, 1), (1, 1), (1, 1)]

afus = [(1, 1), (1, 1), (1, 1), (1, 1), (1, 1), (1, 1), (1, 1), (1, 1), (1, 1), (1, 1), (1, 1), (1, 1), (1, 1), (1, 1),(1 ,1),(1, 1)]

# path = '/home/lucas/Documents/HARP2/OPAESDK/intel-fpga-bbb/samples/tutorial/08_hello_fpga/hw/rtl'
path = '../gen1'
make_afu_manager_controller(cache_data_width, fifo_depth, afus).to_verilog(path)
