from math import ceil, log

from veriloggen import *

from common.make_memory import make_memory
from common.make_program_counter import make_program_counter
from common.make_reg_pipe import make_reg_pipe
from common.utils import make_pe_list
from fdam_cgra.make_omega import make_omega
from fdam_cgra.make_pe import make_pe


def make_cgra(cgra_id, num_pe, num_pe_in, num_pe_out, data_width, net_radix, extra_stagies, conf_depth):
    m = Module('cgra%d_top' % cgra_id)

    clk = m.Input('clk')
    rst = m.Input('rst')

    PIPELINE_PE = 3
    conf_bus_width = 64
    num_thread = int(ceil(log(num_pe * 2, net_radix)) + extra_stagies) + 1 + PIPELINE_PE

    en = m.Input('en')
    conf_bus_in = m.Input('conf_bus_in', conf_bus_width)

    num_stages1 = int(ceil(log(num_pe * 2, net_radix))) + extra_stagies
    num_stages2 = int(ceil(log(num_pe, net_radix)))
    stagies_extra = num_stages1 - num_stages2

    net = make_omega(num_thread, num_pe * 2, extra_stagies, net_radix, conf_depth, False, True)
    net_branch = make_omega(num_thread, num_pe, stagies_extra, net_radix, conf_depth, True, True)

    fifo_in_re = m.Output('fifo_in_re', num_pe_in)
    fifo_in_data = m.Input('fifo_in_data', data_width * num_pe_in)
    fifo_out_we = m.Output('fifo_out_we', num_pe_out)
    fifo_out_data = m.Output('fifo_out_data', data_width * num_pe_out)
    m.EmbeddedCode('')

    conf_bus = m.Wire('conf_bus', conf_bus_width, num_pe + 2)
    pe2neta = m.Wire('pe2neta', data_width, num_pe)
    pe2netb = m.Wire('pe2netb', data_width, num_pe)
    net2pea = m.Wire('net2pea', data_width, num_pe)
    net2peb = m.Wire('net2peb', data_width, num_pe)
    pe2net_branch = m.Wire('pe2net_branch', num_pe)
    net_branch2pe = m.Wire('net_branch2peb', num_pe)

    m.EmbeddedCode('')

    reg_pipe = make_reg_pipe()
    pc = make_program_counter(conf_depth)
    memory = make_memory()
    pe_in = make_pe(cgra_id, 1, num_thread, pc, memory, reg_pipe, data_width, conf_depth)
    pe_out = make_pe(cgra_id, 2, num_thread, pc, memory, reg_pipe, data_width, conf_depth)
    pe = make_pe(cgra_id, 0, num_thread, pc, memory, reg_pipe, data_width, conf_depth)

    param = [('NUM_STAGES', 1), ('DATA_WIDTH', conf_bus_width)]
    con = [('clk', clk), ('rst', Int(0, 1, 2)), ('en', Int(1, 1, 2)), ('in', conf_bus_in), ('out', conf_bus[0])]
    m.Instance(reg_pipe, 'reg_conf_bus_in2', param, con)

    pelist = make_pe_list(num_pe, num_pe_in, num_pe_out)
    fin = 0
    fout = 0
    for p in pelist.keys():
        if pelist[p] == 1:
            params_pe = [('PE_ID', p + 1)]
            con_pe = [('clk', clk), ('rst', rst), ('en', en), ('conf_bus_in', conf_bus[p]),
                      ('branch_in', net_branch2pe[p]),
                      ('branch_out', pe2net_branch[p]),
                      ('fifo_re', fifo_in_re[fin]),
                      ('fifo_data', fifo_in_data[fin * data_width:(fin + 1) * data_width]),
                      ('ina', net2pea[p]), ('inb', net2peb[p]), ('outa', pe2neta[p]),
                      ('outb', pe2netb[p])]
            m.Instance(pe_in, 'pe_io_in_%d' % (p + 1), params_pe, con_pe)
            fin = fin + 1

        elif pelist[p] == 2:
            params_pe = [('PE_ID', p + 1)]
            con_pe = [('clk', clk), ('rst', rst), ('en', en),
                      ('conf_bus_in', conf_bus[p]),
                      ('branch_in', net_branch2pe[p]),
                      ('branch_out', pe2net_branch[p]),
                      ('fifo_we', fifo_out_we[fout]),
                      ('fifo_data', fifo_out_data[fout * data_width:(fout + 1) * data_width]),
                      ('ina', net2pea[p]),
                      ('inb', net2peb[p]), ('outa', pe2neta[p]),
                      ('outb', pe2netb[p])]
            m.Instance(pe_out, 'pe_io_out_%d' % (p + 1), params_pe, con_pe)
            fout = fout + 1
        else:
            params_pe = [('PE_ID', p + 1)]
            con_pe = [('clk', clk), ('rst', rst), ('en', en),
                      ('conf_bus_in', conf_bus[p]),
                      ('branch_in', net_branch2pe[p]),
                      ('branch_out', pe2net_branch[p]),
                      ('ina', net2pea[p]),
                      ('inb', net2peb[p]), ('outa', pe2neta[p]),
                      ('outb', pe2netb[p])]
            m.Instance(pe, 'pe_%d' % (p + 1), params_pe, con_pe)

    for i in range(num_pe):
        param_reg_pipe = [('NUM_STAGES', 1), ('DATA_WIDTH', conf_bus_width)]
        con_reg_pipe = [('clk', clk), ('rst', Int(0, 1, 2)), ('en', Int(1, 1, 2)), ('in', conf_bus[i]),
                        ('out', conf_bus[i + 1])]
        m.Instance(reg_pipe, 'conf_bus_reg_%d' % i, param_reg_pipe, con_reg_pipe)

    params = [('WIDTH', data_width), ('PIPE_EXTRA', 0)]
    con = [('clk', clk), ('rst', rst), ('en', en), ('net_conf_bus_in', conf_bus[num_pe])]
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

    params = [('WIDTH', 1), ('PIPE_EXTRA', 0)]
    con = [('clk', clk), ('rst', rst), ('en', en), ('net_conf_bus_in', conf_bus[num_pe])]
    i = 0
    for k in range(0, num_pe):
        con.append(('in%d' % k, pe2net_branch[i]))
        i += 1

    i = 0
    for k in range(0, num_pe):
        con.append(('out%d' % k, net_branch2pe[i]))
        i += 1

    m.Instance(net_branch, 'net_branch', params, con)

    return m
