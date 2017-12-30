from make_buffer_controller import make_buffer_controller

cache_data_width = 512
fifo_depth = 5

# (x,y) = x -> quantidade de entradas da interface
#         y -> quantidade de saídas da interface
interfaces = [(1, 1), (1, 1)]

# path = '/home/lucas/Documents/HARP2/OPAESDK/intel-fpga-bbb/samples/tutorial/08_hello_fpga/hw/rtl'
path = '../gen'
make_buffer_controller(cache_data_width, fifo_depth, interfaces).to_verilog(path)
