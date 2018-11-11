import math

from veriloggen import *

from cgra_generator.make_initial_conf import make_initial_conf
from cgra_generator.make_pe import make_pe
from cgra_generator.make_program_counter import make_program_counter
from common.make_memory import make_memory
from common.make_reg_pipe import make_reg_pipe
from omega_generator.make_omega import make_omega


def make_cgra(num_pe, num_pe_io, net_radix, conf_depth):
    m = Module('cgra_top')
    DATA_WIDTH = m.Parameter('DATA_WIDTH', 16)

    if conf_depth > 32:
        conf_depth = 32

    conf_id_width = int(math.ceil(math.log(num_pe, 2)) + 1)
    conf_width = 16
    init_conf_width = conf_depth * 3

    clk = m.Input('clk')
    rst = m.Input('rst')

    pes_en = m.Input('pes_en', num_pe)

    conf_bus_in = m.Input('conf_bus_in', conf_id_width + conf_width)
    init_conf_bus_in = m.Input('init_conf_bus_in', conf_id_width + init_conf_width + 16)

    net = make_omega(num_pe * 2, 0, net_radix)
    net_en = m.Input('net_en', net.get_ports().get('en').width)
    en_pc_net = m.Input('en_pc_net')
    net_conf_mem_we = m.Input('net_conf_mem_we')
    net_conf_mem_waddr = m.Input('net_conf_mem_waddr', conf_depth)
    net_conf_mem_data_in = m.Input('net_conf_mem_data_in', net.get_ports().get('conf').width)

    fifo_in_re = m.Output('fifo_in_re', num_pe_io)
    fifo_in_data = m.Input('fifo_in_data', DATA_WIDTH * num_pe_io)
    fifo_out_we = m.Output('fifo_out_we', num_pe_io)
    fifo_out_data = m.Output('fifo_out_data', DATA_WIDTH * num_pe_io)
    m.EmbeddedCode('')
    net_conf_mem_raddr = m.Wire('net_conf_mem_raddr', conf_depth)
    net_conf_mem_data_out = m.Wire('net_conf_mem_data_out', net.get_ports().get('conf').width)
    m.EmbeddedCode('')
    conf_bus = m.Wire('conf_bus', conf_id_width + conf_width, num_pe + 1)
    init_conf_bus = m.Wire('init_conf_bus', conf_id_width + init_conf_width+16, num_pe + 1)
    init_conf_bus_out = m.Wire('init_conf_bus_out', conf_id_width + init_conf_width+16)
    init_conf_we = m.Wire('init_conf_we')
    init_conf_const = m.Wire('init_conf_const', 16)
    pc_max = m.Wire('pc_max', conf_depth)
    pc_loop = m.Wire('pc_loop', conf_depth)
    ignore_until = m.Wire('ignore_until', conf_depth)
    net_conf_re = m.Wire('net_conf_re')
    m.EmbeddedCode('')
    pe2neta = m.Wire('pe2neta', DATA_WIDTH, num_pe)
    pe2netb = m.Wire('pe2netb', DATA_WIDTH, num_pe)
    net2pea = m.Wire('net2pea', DATA_WIDTH, num_pe)
    net2peb = m.Wire('net2peb', DATA_WIDTH, num_pe)
    m.EmbeddedCode('')
    genv = m.Genvar('genv')
    m.EmbeddedCode('')

    init_conf = make_initial_conf(conf_id_width, init_conf_width)
    param = []
    con = [('clk', clk), ('rst', rst), ('initial_conf_in', init_conf_bus_in), ('initial_conf_out', init_conf_bus_out),
           ('init_conf_we',init_conf_we),('init_const', init_conf_const), ('pc_max', pc_max), ('pc_loop', pc_loop),
           ('ignore_until', ignore_until)]
    m.Instance(init_conf, 'init_conf', param, con)

    reg_pipe = make_reg_pipe()
    param = [('NUM_STAGES', 1), ('DATA_WIDTH', conf_id_width + conf_width)]
    con = [('clk', clk), ('en', Int(1, 1, 2)), ('in', conf_bus_in), ('out', conf_bus[0])]
    m.Instance(reg_pipe, 'conf_bus_in_reg', param, con)

    param = [('NUM_STAGES', 1), ('DATA_WIDTH', conf_id_width + init_conf_width + 16)]
    con = [('clk', clk), ('en', Int(1, 1, 2)), ('in', init_conf_bus_out), ('out', init_conf_bus[0])]
    m.Instance(reg_pipe, 'init_conf_bus_in_reg', param, con)

    pc = make_program_counter(conf_depth)
    memory = make_memory()
    pe_io = make_pe(True, pc, memory, conf_id_width, conf_width, conf_depth, init_conf_width)
    pe = make_pe(False, pc, memory, conf_id_width, conf_width, conf_depth, init_conf_width)

    params_pe = [('DATA_WIDTH', DATA_WIDTH), ('PE_ID', genv + 1)]

    con_pe_io = [('clk', clk), ('rst', rst), ('en', pes_en[genv]), ('conf_bus_in', conf_bus[genv]),
                 ('conf_bus_out', conf_bus[genv + 1]), ('init_conf_bus_in', init_conf_bus[genv]),
                 ('init_conf_bus_out', init_conf_bus[genv + 1]),
                 ('fifo_in_re', fifo_in_re[genv]),
                 ('fifo_in_data', fifo_in_data[genv * DATA_WIDTH:(genv + 1) * DATA_WIDTH]),
                 ('fifo_out_we', fifo_out_we[genv]),
                 ('fifo_out_data', fifo_out_data[genv * DATA_WIDTH:(genv + 1) * DATA_WIDTH]), ('ina', net2pea[genv]),
                 ('inb', net2peb[genv]),
                 ('outa', pe2neta[genv]), ('outb', pe2netb[genv])]

    con_pe = [('clk', clk), ('rst', rst), ('en', pes_en[genv]), ('conf_bus_in', conf_bus[genv]),
              ('conf_bus_out', conf_bus[genv + 1]),
              ('init_conf_bus_in', init_conf_bus[genv]), ('init_conf_bus_out', init_conf_bus[genv + 1]),
              ('ina', net2pea[genv]),
              ('inb', net2peb[genv]), ('outa', pe2neta[genv]), ('outb', pe2netb[genv])]

    genInstPEIOs = m.GenerateFor(genv(0), genv < num_pe_io, genv.inc(), 'inst_pe_io')
    genInstPEIOs.Instance(pe_io, 'pe_io', params_pe, con_pe_io)

    genInstPEIOs = m.GenerateFor(genv(num_pe_io), genv < num_pe, genv.inc(), 'inst_pe')
    genInstPEIOs.Instance(pe, 'pe', params_pe, con_pe)

    param = []
    con = [('clk', clk), ('rst', rst), ('en', en_pc_net), ('max', pc_max), ('loop', pc_loop),
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

    params = [('WIDTH', DATA_WIDTH), ('PIPE_EXTRA', 0)]

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

# make_cgra(256, 32, 4, 8).to_verilog('../../../src/cgra_test_256')
