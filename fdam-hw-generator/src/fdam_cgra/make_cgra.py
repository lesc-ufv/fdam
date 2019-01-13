from math import ceil, log

from veriloggen import *

from common.make_memory import make_memory
from common.make_program_counter import make_program_counter
from common.make_reg_pipe import make_reg_pipe
from fdam_cgra.make_omega import make_omega
from make_pe import make_pe


def make_cgra(cgra_id, num_pe, num_pe_in, num_pe_out, data_width, net_radix, extra_stagies, conf_depth):
    m = Module('cgra%d_top' % cgra_id)

    clk = m.Input('clk')
    rst = m.Input('rst')

    PIPELINE_PE = 3
    conf_bus_width = 64
    num_thread = int(ceil(log(num_pe * 2, net_radix)) + extra_stagies) + 1 + PIPELINE_PE

    pes_en = m.Input('pes_en', num_pe)
    conf_bus_in = m.Input('conf_bus_in', conf_bus_width)

    num_stages1 = int(ceil(log(num_pe * 2, net_radix))) + extra_stagies
    num_stages2 = int(ceil(log(num_pe, net_radix)))
    stagies_extra = num_stages1 - num_stages2

    net = make_omega(num_thread, num_pe * 2, extra_stagies, net_radix, conf_depth, False, True)
    net_branch = make_omega(num_thread, num_pe, stagies_extra, net_radix, conf_depth, True, True)

    net_en = m.Input('net_en', net.get_ports().get('en').width)
    en_pc_net = m.Input('en_pc_net', net.get_ports().get('en_pc_net').width)

    fifo_in_re = m.Output('fifo_in_re', num_pe_in)
    fifo_in_data = m.Input('fifo_in_data', data_width * num_pe_in)
    fifo_out_we = m.Output('fifo_out_we', num_pe_out)
    fifo_out_data = m.Output('fifo_out_data', data_width * num_pe_out)
    m.EmbeddedCode('')

    conf_bus = [m.Wire('conf_bus_%d' % i, conf_bus_width) for i in range(num_pe + 2)]
    pe2neta = [m.Wire('pe2neta_%d' % i, data_width) for i in range(num_pe)]
    pe2netb = [m.Wire('pe2netb_%d' % i, data_width) for i in range(num_pe)]
    net2pea = [m.Wire('net2pea_%d' % i, data_width) for i in range(num_pe)]
    net2peb = [m.Wire('net2peb_%d' % i, data_width) for i in range(num_pe)]
    pe2net_branch = [m.Wire('pe2net_branch_%d' % i) for i in range(num_pe)]
    net_branch2pe = [m.Wire('net_branch2peb_%d' % i) for i in range(num_pe)]
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

    pe_id = 0
    for _ in range(num_pe_in):
        params_pe = [('PE_ID', pe_id + 1)]
        con_pe = [('clk', clk), ('rst', rst), ('en', pes_en[pe_id]), ('conf_bus_in', conf_bus[pe_id]),
                  ('branch_in', net_branch2pe[pe_id]),
                  ('branch_out', pe2net_branch[pe_id]),
                  ('fifo_re', fifo_in_re[pe_id]),
                  ('fifo_data', fifo_in_data[pe_id * data_width:(pe_id + 1) * data_width]),
                  ('ina', net2pea[pe_id]), ('inb', net2peb[pe_id]), ('outa', pe2neta[pe_id]),
                  ('outb', pe2netb[pe_id])]
        m.Instance(pe_in, 'pe_io_in_%d' % (pe_id + 1), params_pe, con_pe)
        pe_id = pe_id + 1

    for i in range(num_pe_out):
        params_pe = [('PE_ID', pe_id + 1)]
        con_pe = [('clk', clk), ('rst', rst), ('en', pes_en[pe_id]),
                  ('conf_bus_in', conf_bus[pe_id]),
                  ('branch_in', net_branch2pe[pe_id]),
                  ('branch_out', pe2net_branch[pe_id]),
                  ('fifo_we', fifo_out_we[i]),
                  ('fifo_data', fifo_out_data[i * data_width:(i + 1) * data_width]),
                  ('ina', net2pea[pe_id]),
                  ('inb', net2peb[pe_id]), ('outa', pe2neta[pe_id]),
                  ('outb', pe2netb[pe_id])]
        m.Instance(pe_out, 'pe_io_out_%d' % (pe_id + 1), params_pe, con_pe)
        pe_id = pe_id + 1

    for _ in range(num_pe - (num_pe_in + num_pe_out)):
        params_pe = [('PE_ID', pe_id + 1)]
        con_pe = [('clk', clk), ('rst', rst), ('en', pes_en[pe_id]),
                  ('conf_bus_in', conf_bus[pe_id]),
                  ('branch_in', net_branch2pe[pe_id]),
                  ('branch_out', pe2net_branch[pe_id]),
                  ('ina', net2pea[pe_id]),
                  ('inb', net2peb[pe_id]), ('outa', pe2neta[pe_id]),
                  ('outb', pe2netb[pe_id])]
        m.Instance(pe, 'pe_%d' % (pe_id + 1), params_pe, con_pe)
        pe_id = pe_id + 1

    for i in range(num_pe):
        param_reg_pipe = [('NUM_STAGES', 1), ('DATA_WIDTH', conf_bus_width)]
        con_reg_pipe = [('clk', clk), ('rst', Int(0, 1, 2)), ('en', Int(1, 1, 2)), ('in', conf_bus[i]),
                        ('out', conf_bus[i + 1])]
        m.Instance(reg_pipe, 'conf_bus_reg_%d' % i, param_reg_pipe, con_reg_pipe)

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

    params = [('WIDTH', 1), ('PIPE_EXTRA', 0)]
    con = [('clk', clk), ('rst', rst), ('en_pc_net', en_pc_net[0:net_branch.get_ports().get('en_pc_net').width]),
           ('en', net_en[0:net_branch.get_ports().get('en').width]), ('net_conf_bus_in', conf_bus[num_pe])]
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
