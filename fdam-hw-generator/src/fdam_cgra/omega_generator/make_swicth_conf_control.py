from veriloggen import *

from common.make_memory import make_memory
from common.make_reg_pipe import make_reg_pipe
from common.make_program_counter import make_program_counter
from make_swicth_conf_reader import make_swicth_conf_reader


def make_swicth_conf_control(swicth_conf_width, conf_net_depth):
    m = Module('swicth_conf_control_%d_%d' %(swicth_conf_width,conf_net_depth) )
    SWICTH_NUMBER = m.Parameter('SWICTH_NUMBER', 0)
    STAGE = m.Parameter('STAGE', 1)

    clk = m.Input('clk')
    rst = m.Input('rst')

    en_pc_net = m.Input('en_pc_net')
    conf_bus_in = m.Input('conf_bus_in', 64)

    swicth_conf_out = m.Output('swicth_conf_out', swicth_conf_width)

    net_pc_max = m.Wire('net_pc_max', conf_net_depth)
    net_pc_loop = m.Wire('net_pc_loop', conf_net_depth)
    net_conf_mem_raddr = m.Wire('net_conf_mem_raddr', conf_net_depth)
    net_mem_we = m.Wire('net_mem_we')
    net_mem_waddr = m.Wire('net_mem_waddr', conf_net_depth)
    net_mem_din = m.Wire('net_mem_din', swicth_conf_width)
    net_mem_dout = m.Wire('net_mem_dout', swicth_conf_width)

    swicth_conf_reader = make_swicth_conf_reader(swicth_conf_width, conf_net_depth)
    param = [('SWICTH_NUMBER', SWICTH_NUMBER)]
    con = [('clk', clk), ('rst', rst), ('conf_bus_in', conf_bus_in), ('pc_max', net_pc_max),
           ('pc_loop', net_pc_loop), ('net_mem_we', net_mem_we), ('net_mem_waddr', net_mem_waddr),
           ('net_mem_data', net_mem_din)]
    m.Instance(swicth_conf_reader, 'swicth_conf_reader', param, con)

    pc = make_program_counter(conf_net_depth)
    param = []
    con = [('clk', clk), ('rst', rst), ('en', en_pc_net), ('max', net_pc_max), ('loop', net_pc_loop),
           ('pc', net_conf_mem_raddr)]
    m.Instance(pc, 'pc', param, con)

    memory = make_memory()
    param = [('DATA_WIDTH', swicth_conf_width), ('ADDR_WIDTH', conf_net_depth)]
    con = [('clk', clk), ('we', net_mem_we), ('re', Int(1, 1, 2)), ('raddr', net_conf_mem_raddr),
           ('waddr', net_conf_mem_raddr),
           ('din', net_mem_din),
           ('dout', net_mem_dout)]
    m.Instance(memory, 'mem_conf', param, con)

    reg_pipe = make_reg_pipe()
    param = [('NUM_STAGES', STAGE), ('DATA_WIDTH', swicth_conf_width)]
    con = [('clk', clk), ('rst', Int(0, 1, 2)), ('en', en_pc_net), ('in', net_mem_dout),
           ('out', swicth_conf_out)]
    m.Instance(reg_pipe, 'reg_net_conf_out', param, con)

    return m
