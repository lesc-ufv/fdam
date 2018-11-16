from veriloggen import *

from common.make_program_counter import make_program_counter
from fdam_cgra.omega_generator.make_omega import make_omega
from make_pe import make_pe
from common.make_memory import make_memory
from common.make_reg_pipe import make_reg_pipe

def make_cgra(cgra_id, num_pe, num_pe_io, data_width, net_radix, conf_depth):
    m = Module('cgra%d_top' % cgra_id)

    clk = m.Input('clk')
    rst = m.Input('rst')

    conf_bus_width = 64

    pes_en = m.Input('pes_en', num_pe)
    conf_bus_in = m.Input('conf_bus_in', conf_bus_width)

    net = make_omega(num_pe * 2, 0, net_radix, conf_depth)

    net_en = m.Input('net_en', net.get_ports().get('en').width)
    en_pc_net = m.Input('en_pc_net', net.get_ports().get('en_pc_net').width)

    fifo_in_re = m.Output('fifo_in_re', num_pe_io)
    fifo_in_data = m.Input('fifo_in_data', data_width * num_pe_io)
    fifo_out_we = m.Output('fifo_out_we', num_pe_io)
    fifo_out_data = m.Output('fifo_out_data', data_width * num_pe_io)
    m.EmbeddedCode('')

    conf_bus = m.Wire('conf_bus', conf_bus_width, num_pe + 1)

    m.EmbeddedCode('')
    pe2neta = m.Wire('pe2neta', data_width, num_pe)
    pe2netb = m.Wire('pe2netb', data_width, num_pe)
    net2pea = m.Wire('net2pea', data_width, num_pe)
    net2peb = m.Wire('net2peb', data_width, num_pe)
    m.EmbeddedCode('')
    genv = m.Genvar('genv')
    m.EmbeddedCode('')

    pc = make_program_counter(conf_depth)
    memory = make_memory()
    pe_io = make_pe(cgra_id, True, pc, memory, data_width, conf_depth)
    pe = make_pe(cgra_id, False, pc, memory, data_width, conf_depth)
    reg_pipe = make_reg_pipe()

    param = [('NUM_STAGES', 1), ('DATA_WIDTH', conf_bus_width)]
    con = [('clk', clk), ('rst', Int(0, 1, 2)), ('en', Int(1, 1, 2)), ('in', conf_bus_in), ('out', conf_bus[0])]
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
    con_reg_pipe = [('clk', clk), ('rst', Int(0, 1, 2)), ('en', Int(1, 1, 2)), ('in', conf_bus[genv]),
                    ('out', conf_bus[genv + 1])]

    genInstPEIOs = m.GenerateFor(genv(0), genv < num_pe_io, genv.inc(), 'inst_pe_io')
    genInstPEIOs.Instance(pe_io, 'pe_io', params_pe, con_pe_io)
    genInstPEIOs.Instance(reg_pipe, 'conf_bus_reg_pe_io', param_reg_pipe, con_reg_pipe)

    genInstPE = m.GenerateFor(genv(num_pe_io), genv < num_pe, genv.inc(), 'inst_pe')
    genInstPE.Instance(pe, 'pe', params_pe, con_pe)
    genInstPE.Instance(reg_pipe, 'conf_bus_reg_pe', param_reg_pipe, con_reg_pipe)

    params = [('WIDTH', data_width), ('PIPE_EXTRA', 0)]
    con = [('clk', clk), ('rst', rst), ('en_pc_net', en_pc_net), ('en', net_en), ('net_conf_bus_in', conf_bus[num_pe])]
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
