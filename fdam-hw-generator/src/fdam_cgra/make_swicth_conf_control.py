from math import ceil, log

from veriloggen import *

from common.make_cgra_counter import make_thread_counter
from common.make_memory import make_memory
from common.make_mux import make_mux
from common.make_program_counter import make_program_counter
from common.make_reg_pipe import make_reg_pipe
from make_swicth_conf_reader import make_swicth_conf_reader


def make_swicth_conf_control(num_thread, swicth_conf_width, conf_net_depth):
    m = Module('swicth_conf_control_%d_%d' % (swicth_conf_width, conf_net_depth))
    SWICTH_NUMBER = m.Parameter('SWICTH_NUMBER', 0)
    STAGE = m.Parameter('STAGE', 1)
    PE_LATENCY = 3
    clk = m.Input('clk')
    rst = m.Input('rst')

    en_pc_net = m.Input('en_pc_net')
    conf_bus_in = m.Input('conf_bus_in', 64)

    swicth_conf_out = m.Output('swicth_conf_out', swicth_conf_width)

    net_pc_max = m.Wire('net_pc_max', conf_net_depth, num_thread)
    net_pc_max_mem = m.Wire('net_pc_max_mem', conf_net_depth)
    net_pc_max_we = m.Wire('net_pc_max_we')
    net_pc_loop = m.Wire('net_pc_loop', conf_net_depth, num_thread)
    net_pc_loop_mem = m.Wire('net_pc_loop_mem', conf_net_depth)
    net_pc_loop_we = m.Wire('net_pc_loop_we')

    net_pc_out =  m.Wire('net_pc_out', conf_net_depth, num_thread)
    net_conf_mem_raddr = m.Wire('net_conf_mem_raddr', conf_net_depth)
    net_mem_we = m.Wire('net_mem_we')
    net_mem_waddr = m.Wire('net_mem_waddr', conf_net_depth)
    net_mem_din = m.Wire('net_mem_din', swicth_conf_width)
    net_mem_dout = m.Wire('net_mem_dout', swicth_conf_width)
    thread_id = m.Wire('thread_id', int(ceil(log(num_thread, 2))))
    thread_id_dec = m.Wire('thread_id_dec', num_thread)
    thread_idx = m.Wire('thread_idx', int(ceil(log(num_thread, 2))))
    thread_idx_dec = m.Wire('thread_idx_dec', num_thread)
    genv = m.Genvar('genv')

    thread_idx_dec.assign(Sll(1, thread_idx))
    thread_id_dec.assign(Sll(1, thread_id))

    reg_pipe = make_reg_pipe()

    swicth_conf_reader = make_swicth_conf_reader(num_thread, swicth_conf_width, conf_net_depth)
    param = [('SWICTH_NUMBER', SWICTH_NUMBER)]
    con = [('clk', clk), ('rst', rst), ('conf_bus_in', conf_bus_in), ('pc_max', net_pc_max_mem),
           ('pc_max_we', net_pc_max_we), ('pc_loop', net_pc_loop_mem), ('pc_loop_we', net_pc_loop_we),
           ('net_mem_we', net_mem_we), ('net_mem_waddr', net_mem_waddr),
           ('net_mem_data', net_mem_din), ('thread_id', thread_id)]
    m.Instance(swicth_conf_reader, 'swicth_conf_reader', param, con)

    pc = make_program_counter(conf_net_depth)
    param = []
    con = [('clk', clk), ('rst', rst), ('en', thread_idx_dec[genv] & en_pc_net), ('max', net_pc_max[genv]),
           ('loop', net_pc_loop[genv]), ('pc', net_pc_out[genv])]
    m.GenerateFor(genv(0), genv < num_thread, genv.inc(), 'pc_inst').Instance(pc, 'pc', param, con)

    mux_thread = make_mux(num_thread)
    param = [('WIDTH', net_pc_out.width)]
    con1 = [('in%d' % i, net_pc_out[i]) for i in range(num_thread)]
    con = [('sel', thread_idx),('out', net_conf_mem_raddr)]
    m.Instance(mux_thread, 'mux_pc', param, con+con1)

    memory = make_memory()
    param = [('DATA_WIDTH', swicth_conf_width), ('ADDR_WIDTH', conf_net_depth + int(ceil(log(num_thread, 2))))]
    con = [('clk', clk), ('we', net_mem_we),('re',en_pc_net),('raddr', Cat(thread_idx, net_conf_mem_raddr)),
           ('waddr', Cat(thread_id, net_mem_waddr)), ('din', net_mem_din), ('dout', net_mem_dout)]
    m.Instance(memory, 'mem_conf', param, con)

    param = [('NUM_STAGES', 1), ('DATA_WIDTH', conf_net_depth)]
    con = [('clk', clk), ('rst', rst), ('en', thread_id_dec[genv] & net_pc_max_we), ('in', net_pc_max_mem),
           ('out', net_pc_max[genv])]
    m.GenerateFor(genv(0), genv < num_thread, genv.inc(), 'pc_max_inst').Instance(reg_pipe, 'pc_max', param, con)

    param = [('NUM_STAGES', 1), ('DATA_WIDTH', conf_net_depth)]
    con = [('clk', clk), ('rst', rst), ('en', thread_id_dec[genv] & net_pc_loop_we), ('in', net_pc_loop_mem),
           ('out', net_pc_loop[genv])]
    m.GenerateFor(genv(0), genv < num_thread, genv.inc(), 'pc_loop_inst').Instance(reg_pipe, 'pc_loop', param, con)

    thread_counter = make_thread_counter(num_thread)
    param = []
    con = [('clk', clk), ('rst', rst), ('en', en_pc_net), ('out', thread_idx)]
    m.Instance(thread_counter, 'thread_counter', param, con)

    param = [('NUM_STAGES', STAGE+PE_LATENCY), ('DATA_WIDTH', swicth_conf_width)]
    con = [('clk', clk), ('rst', Int(0, 1, 2)), ('en', en_pc_net), ('in', net_mem_dout),
           ('out', swicth_conf_out)]
    m.Instance(reg_pipe, 'reg_net_conf_out', param, con)

    return m
