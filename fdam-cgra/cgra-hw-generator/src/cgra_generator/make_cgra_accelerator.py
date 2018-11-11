import math

from veriloggen import *

from cgra_generator.make_cgra import make_cgra
from cgra_generator.make_control_exec import make_control_exec
from cgra_generator.make_control_initial_conf import make_control_initial_conf
from cgra_generator.make_control_net_conf import make_control_net_conf
from cgra_generator.make_control_pe_conf import make_control_pe_conf
from common.make_dispath_data import make_dispath_data
from common.make_fecth_data import make_fecth_data
from omega_generator.make_omega import make_omega


def make_cgra_accelerator(num_pe, num_pe_io, net_radix, mem_conf_depth):
    conf_id_width = int(math.ceil(math.log(num_pe, 2)) + 1)
    conf_width = 16
    init_conf_width = mem_conf_depth * 3
    net = make_omega(num_pe * 2, 0, net_radix)
    conf_net_width = net.get_ports().get('conf').width
    en_net_bits = net.get_ports().get('en').width
    fd = make_fecth_data()
    dd = make_dispath_data()
    control_initial_pe_conf = make_control_initial_conf(num_pe, num_pe_io, mem_conf_depth)
    control_pe_conf = make_control_pe_conf(num_pe)
    control_net_conf = make_control_net_conf(num_pe, net_radix, mem_conf_depth)

    control_exec = make_control_exec(num_pe, num_pe_io, net_radix)
    cgra = make_cgra(num_pe, num_pe_io, net_radix, mem_conf_depth)

    m = Module('cgra_acc')
    DATA_WIDTH = m.Parameter('DATA_WIDTH', 16)
    clk = m.Input('clk')
    rst = m.Input('rst')
    start = m.Input('start')
    acc_user_done_rd_data = m.Input('acc_user_done_rd_data', num_pe_io)
    acc_user_done_wr_data = m.Input('acc_user_done_wr_data', num_pe_io)
    acc_user_available_read = m.Input('acc_user_available_read', num_pe_io)
    acc_user_request_read = m.Output('acc_user_request_read', num_pe_io)
    acc_user_read_data_valid = m.Input('acc_user_read_data_valid', num_pe_io)
    acc_user_read_data = m.Input('acc_user_read_data', 512 * num_pe_io)
    acc_user_available_write = m.Input('acc_user_available_write', num_pe_io)
    acc_user_request_write = m.Output('acc_user_request_write', num_pe_io)
    acc_user_write_data = m.Output('acc_user_write_data', 512 * num_pe_io)
    acc_user_done = m.Output('acc_user_done')

    request_read = m.Wire('request_read', num_pe_io)
    conf_control_req_rd_data = m.Wire('conf_control_req_rd_data')
    en_fecth_data = m.Wire('en_fecth_data', num_pe_io)
    en_dispath_data = m.Wire('en_dispath_data', num_pe_io)
    en_pe = m.Wire('en_pe', num_pe)
    en_net = m.Wire('en_net', en_net_bits)
    en_pc_net = m.Wire('en_pc_net')
    fifo_in_re = m.Wire('fifo_in_re', num_pe_io)
    fifo_in_data = m.Wire('fifo_in_data', DATA_WIDTH * num_pe_io)
    available_pop = m.Wire('available_pop', num_pe_io)
    fifo_out_we = m.Wire('fifo_out_we', num_pe_io)
    fifo_out_data = m.Wire('fifo_out_data', DATA_WIDTH * num_pe_io)
    available_push = m.Wire('available_push', num_pe_io)
    conf_pe_out_bus = m.Wire('conf_pe_out_bus', conf_id_width + conf_width)
    conf_pe_init_out_bus = m.Wire('conf_pe_init_out_bus', conf_id_width + init_conf_width + 16)
    conf_net_we = m.Wire('conf_net_we')
    conf_net_addr = m.Wire('conf_net_addr', mem_conf_depth)
    conf_net_data_out = m.Wire('conf_net_out', conf_net_width)
    read_fifo_mask = m.Wire('read_fifo_mask', num_pe_io)
    write_fifo_mask = m.Wire('write_fifo_mask', num_pe_io)
    initial_conf_done = m.Wire('initial_conf_done')
    qtd_conf = m.Wire('qtd_conf', 32)
    qtd_net_conf = m.Wire('qtd_net_conf', 32)
    conf_done = m.Wire('conf_done')
    initial_conf_control_req_rd_data = m.Wire('initial_conf_control_req_rd_data')
    net_conf_control_req_rd_data = m.Wire('net_conf_control_req_rd_data')
    net_conf_done = m.Wire('net_conf_done')
    genv = m.Genvar('genv')
    if num_pe_io > 1:
        acc_user_request_read[1:].assign(request_read[1:])
    acc_user_request_read[0].assign(
        request_read[0] | conf_control_req_rd_data | initial_conf_control_req_rd_data | net_conf_control_req_rd_data)

    genInstFor1 = m.GenerateFor(genv(0), genv < num_pe_io, genv.inc(), 'inst_fecth_data')
    genInstFor2 = m.GenerateFor(genv(0), genv < num_pe_io, genv.inc(), 'inst_dispath_data')
    params = [('INPUT_DATA_WIDTH', 512), ('OUTPUT_DATA_WIDTH', DATA_WIDTH)]
    con = [
        ('clk', clk), ('rst', rst), ('en', en_fecth_data[genv]), ('available_read', acc_user_available_read[genv]),
        ('request_read', request_read[genv]), ('data_valid', acc_user_read_data_valid[genv]),
        ('read_data', acc_user_read_data[Mul(genv, 512):Mul(genv + 1, 512)]), ('pop_data', fifo_in_re[genv]),
        ('available_pop', available_pop[genv]),
        ('data_out', fifo_in_data[Mul(genv, DATA_WIDTH):Mul(genv + 1, DATA_WIDTH)])
    ]
    genInstFor1.Instance(fd, 'fecth_data', params, con)

    params = [('INPUT_DATA_WIDTH', DATA_WIDTH), ('OUTPUT_DATA_WIDTH', 512)]
    con = [('clk', clk), ('rst', rst), ('en', en_dispath_data[genv]),
           ('available_write', acc_user_available_write[genv]),
           ('request_write', acc_user_request_write[genv]),
           ('write_data', acc_user_write_data[Mul(genv, 512):Mul(genv + 1, 512)]), ('push_data', fifo_out_we[genv]),
           ('available_push', available_push[genv]),
           ('data_in', fifo_out_data[Mul(genv, DATA_WIDTH):Mul(genv + 1, DATA_WIDTH)])
           ]
    genInstFor2.Instance(dd, 'dispath_data', params, con)

    params = []
    con = [('clk', clk), ('rst', rst), ('start', net_conf_done), ('read_fifo_mask', read_fifo_mask),
           ('write_fifo_mask', write_fifo_mask), ('available_read', acc_user_available_read),
           ('available_write', acc_user_available_write),
           ('available_pop', available_pop), ('available_push', available_push),
           ('read_fifo_done', acc_user_done_rd_data),
           ('write_fifo_done', acc_user_done_wr_data), ('en_pe', en_pe), ('en_net', en_net), ('en_pc_net', en_pc_net),
           ('en_fecth_data', en_fecth_data), ('en_dispath_data', en_dispath_data), ('done', acc_user_done)
           ]
    m.Instance(control_exec, 'control_exec', params, con)

    params = []
    con = [
        ('clk', clk), ('rst', rst), ('start', start), ('available_read', acc_user_available_read[0]),
        ('req_rd_data', initial_conf_control_req_rd_data), ('rd_data', acc_user_read_data[0:512]),
        ('rd_data_valid', acc_user_read_data_valid[0]), ('conf_pe_init_out_bus', conf_pe_init_out_bus),
        ('qtd_conf', qtd_conf), ('qtd_net_conf', qtd_net_conf), ('read_fifo_mask', read_fifo_mask),
        ('write_fifo_mask', write_fifo_mask), ('done', initial_conf_done)
    ]
    m.Instance(control_initial_pe_conf, 'control_initial_pe_conf', params, con)

    con = [
        ('clk', clk), ('rst', rst), ('start', initial_conf_done), ('qtd_conf', qtd_conf),
        ('available_read', acc_user_available_read[0]),
        ('req_rd_data', conf_control_req_rd_data), ('rd_data', acc_user_read_data[0:512]),
        ('rd_data_valid', acc_user_read_data_valid[0]), ('conf_pe_out_bus', conf_pe_out_bus),
        ('done', conf_done)
    ]
    m.Instance(control_pe_conf, 'control_pe_conf', params, con)

    con = [
        ('clk', clk), ('rst', rst), ('start', conf_done), ('qtd_net_conf', qtd_net_conf),
        ('available_read', acc_user_available_read[0]),
        ('req_rd_data', net_conf_control_req_rd_data), ('rd_data', acc_user_read_data[0:512]),
        ('rd_data_valid', acc_user_read_data_valid[0]), ('conf_net_we', conf_net_we), ('conf_net_addr', conf_net_addr),
        ('conf_net_data_out', conf_net_data_out), ('done', net_conf_done)
    ]
    m.Instance(control_net_conf, 'control_net_conf', params, con)

    params = [('DATA_WIDTH', DATA_WIDTH)]
    con = [
        ('clk', clk), ('rst', rst), ('pes_en', en_pe), ('conf_bus_in', conf_pe_out_bus),
        ('init_conf_bus_in', conf_pe_init_out_bus), ('net_en', en_net), ('en_pc_net', en_pc_net),
        ('net_conf_mem_we', conf_net_we),
        ('net_conf_mem_waddr', conf_net_addr), ('net_conf_mem_data_in', conf_net_data_out), ('fifo_in_re', fifo_in_re),
        ('fifo_in_data', fifo_in_data), ('fifo_out_we', fifo_out_we), ('fifo_out_data', fifo_out_data)
    ]
    m.Instance(cgra, 'cgra', params, con)

    return m


make_cgra_accelerator(16, 16, 4, 8).to_verilog(
    '/home/lucas/Documentos/ufv-projects/fdam/fdam-cgra/cgra-samples/cgra_16_16/hw/rtl/acc0')
