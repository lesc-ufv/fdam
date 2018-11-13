from veriloggen import *

from cgra_generator.make_conf_reader_pc_net import make_conf_reader_pc_net
from cgra_generator.make_pe import make_pe
from cgra_generator.make_program_counter import make_program_counter
from common.make_memory import make_memory
from common.make_reg_pipe import make_reg_pipe
from omega_generator.make_omega import make_omega


def make_cgra(cgra_id,num_pe, num_pe_io,data_width,net_radix, conf_depth):
    m = Module('cgra%d_top'%cgra_id)

    clk = m.Input('clk')
    rst = m.Input('rst')

    conf_bus_width = 60

    pes_en = m.Input('pes_en', num_pe)

    conf_bus_in = m.Input('conf_bus_in', conf_bus_width)

    net = make_omega(cgra_id,num_pe * 2, 0, net_radix)
    net_en = m.Input('net_en', net.get_ports().get('en').width)
    en_pc_net = m.Input('en_pc_net')
    net_conf_mem_we = m.Input('net_conf_mem_we')
    net_conf_mem_waddr = m.Input('net_conf_mem_waddr', conf_depth)
    net_conf_mem_data_in = m.Input('net_conf_mem_data_in', net.get_ports().get('conf').width)

    fifo_in_re = m.Output('fifo_in_re', num_pe_io)
    fifo_in_data = m.Input('fifo_in_data', data_width * num_pe_io)
    fifo_out_we = m.Output('fifo_out_we', num_pe_io)
    fifo_out_data = m.Output('fifo_out_data', data_width * num_pe_io)
    m.EmbeddedCode('')
    net_conf_re = m.Wire('net_conf_re')
    net_conf_mem_raddr = m.Wire('net_conf_mem_raddr', conf_depth)
    net_conf_mem_data_out = m.Wire('net_conf_mem_data_out', net.get_ports().get('conf').width)
    m.EmbeddedCode('')
    conf_bus = m.Wire('conf_bus', conf_bus_width, num_pe + 1)
    conf_bus_in_reg = m.Wire('conf_bus_in_reg', conf_bus_width)

    net_pc_max = m.Wire('net_pc_max', conf_depth)
    net_pc_loop = m.Wire('net_pc_loop', conf_depth)

    m.EmbeddedCode('')
    pe2neta = m.Wire('pe2neta', data_width, num_pe)
    pe2netb = m.Wire('pe2netb', data_width, num_pe)
    net2pea = m.Wire('net2pea', data_width, num_pe)
    net2peb = m.Wire('net2peb', data_width, num_pe)
    m.EmbeddedCode('')
    genv = m.Genvar('genv')
    m.EmbeddedCode('')

    pc = make_program_counter(cgra_id,conf_depth)
    memory = make_memory(cgra_id)
    pe_io = make_pe(cgra_id,True, pc, memory,data_width, conf_depth)
    pe = make_pe(cgra_id,False, pc, memory,data_width, conf_depth)
    reg_pipe = make_reg_pipe(cgra_id)

    param = [('NUM_STAGES', 1), ('DATA_WIDTH', conf_bus_width)]
    con = [('clk', clk), ('en', Int(1, 1, 2)), ('in', conf_bus_in), ('out', conf_bus_in_reg)]
    m.Instance(reg_pipe, 'reg_conf_bus_in', param, con)

    param = [('NUM_STAGES', 1), ('DATA_WIDTH', conf_bus_width)]
    con = [('clk', clk), ('en', Int(1, 1, 2)), ('in', conf_bus_in), ('out', conf_bus[0])]
    m.Instance(reg_pipe, 'reg_conf_bus_in2', param, con)

    params_pe = [('PE_ID', genv + 1)]

    con_pe_io = [('clk', clk), ('rst', rst), ('en', pes_en[genv]), ('conf_bus_in', conf_bus[genv]),
                 ('fifo_in_re', fifo_in_re[genv]),
                 ('fifo_in_data', fifo_in_data[genv * data_width:(genv + 1) * data_width]),
                 ('fifo_out_we', fifo_out_we[genv]),
                 ('fifo_out_data', fifo_out_data[genv * data_width:(genv + 1) * data_width]), ('ina', net2pea[genv]),
                 ('inb', net2peb[genv]),
                 ('outa', pe2neta[genv]), ('outb', pe2netb[genv])]

    con_pe = [('clk', clk), ('rst', rst), ('en', pes_en[genv]), ('conf_bus_in', conf_bus[genv]),
              ('ina', net2pea[genv]), ('inb', net2peb[genv]),
              ('outa', pe2neta[genv]), ('outb', pe2netb[genv])]

    param_reg_pipe = [('NUM_STAGES', 1), ('DATA_WIDTH', conf_bus_width)]
    con_reg_pipe = [('clk', clk), ('en', Int(1, 1, 2)), ('in', conf_bus[genv]), ('out', conf_bus[genv + 1])]

    genInstPEIOs = m.GenerateFor(genv(0), genv < num_pe_io, genv.inc(), 'inst_pe_io')
    genInstPEIOs.Instance(pe_io, 'pe_io', params_pe, con_pe_io)
    genInstPEIOs.Instance(reg_pipe, 'conf_bus_reg_pe_io', param_reg_pipe, con_reg_pipe)

    genInstPE = m.GenerateFor(genv(num_pe_io), genv < num_pe, genv.inc(), 'inst_pe')
    genInstPE.Instance(pe, 'pe', params_pe, con_pe)
    genInstPE.Instance(reg_pipe, 'conf_bus_reg_pe', param_reg_pipe, con_reg_pipe)

    conf_reader_pc_net = make_conf_reader_pc_net(cgra_id,conf_depth)
    param = [('PE_ID', num_pe + 1)]
    con = [('clk', clk), ('rst', rst), ('conf_bus_in', conf_bus_in_reg), ('pc_max', net_pc_max),
           ('pc_loop', net_pc_loop)]
    m.Instance(conf_reader_pc_net, conf_reader_pc_net.name, param, con)

    param = []
    con = [('clk', clk), ('rst', rst), ('en', en_pc_net), ('max', net_pc_max), ('loop', net_pc_loop),
           ('pc', net_conf_mem_raddr), ('pc_en', net_conf_re)]
    m.Instance(pc, 'pc', param, con)

    conf_width = net.get_ports().get('conf').width
    mem_width = 32

    genInstMemConf = m.GenerateFor(genv(0), genv < conf_width // mem_width, genv.inc(), 'inst_mem_conf')

    net_conf_mem = memory
    param = [('DATA_WIDTH', mem_width), ('ADDR_WIDTH', conf_depth)]
    con = [('clk', clk), ('we', net_conf_mem_we), ('re', net_conf_re), ('raddr', net_conf_mem_raddr),
           ('waddr', net_conf_mem_waddr),
           ('din', net_conf_mem_data_in[Mul((mem_width), genv):Mul((mem_width), (genv + 1))]),
           ('dout', net_conf_mem_data_out[Mul((mem_width), genv):Mul((mem_width), (genv + 1))])]

    genInstMemConf.Instance(net_conf_mem, 'net_conf_mem', param, con)

    params = [('WIDTH', data_width), ('PIPE_EXTRA', 0)]

    con = [('clk', clk), ('en', net_en), ('conf', net_conf_mem_data_out)]

    i, j = 0, 0
    for k in range(0, num_pe * 2, 2):
        con.append(('in%d' % k, pe2neta[i]))
        con.append(('in%d' % (k + 1), pe2netb[j]))
        i += 1
        j += 1

    i, j = 0, 0
    for k in range(0, num_pe * 2, 2):
        con.append(('out%d' % k, net2pea[i]))
        con.append(('out%d' % (k + 1), net2peb[j]))
        i += 1
        j += 1

    m.Instance(net, 'net', params, con)

    return m
