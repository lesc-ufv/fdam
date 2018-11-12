from veriloggen import *

from cgra_generator.make_cgra_accelerator import make_cgra_accelerator


def make_cgra_acc_user(acc_id, num_pe, num_pe_io, radix, mem_conf_depth, data_width):
    m = Module('fdam_acc_user_%d' % acc_id)
    DATA_WIDTH = m.Parameter('DATA_WIDTH', 512)
    NUM_INPUT_QUEUES = m.Parameter('NUM_INPUT_QUEUES', 1)
    NUM_OUTPUT_QUEUES = m.Parameter('NUM_OUTPUT_QUEUES', 1)
    clk = m.Input('clk')
    rst = m.Input('rst')
    start = m.Input('start')
    acc_user_done_rd_data = m.Input('acc_user_done_rd_data', NUM_INPUT_QUEUES)
    acc_user_done_wr_data = m.Input('acc_user_done_wr_data', NUM_OUTPUT_QUEUES)
    acc_user_available_read = m.Input('acc_user_available_read', NUM_INPUT_QUEUES)
    acc_user_request_read = m.OutputReg('acc_user_request_read', NUM_INPUT_QUEUES)
    acc_user_read_data_valid = m.Input('acc_user_read_data_valid', NUM_INPUT_QUEUES)
    acc_user_read_data = m.Input('acc_user_read_data', DATA_WIDTH * NUM_INPUT_QUEUES)
    acc_user_available_write = m.Input('acc_user_available_write', NUM_OUTPUT_QUEUES)
    acc_user_request_write = m.OutputReg('acc_user_request_write', NUM_OUTPUT_QUEUES)
    acc_user_write_data = m.OutputReg('acc_user_write_data', DATA_WIDTH * NUM_OUTPUT_QUEUES)
    acc_user_done = m.OutputReg('acc_user_done')

    cgra_acc = make_cgra_accelerator(num_pe, num_pe_io, data_width, radix, mem_conf_depth)

    m.Instance(cgra_acc, 'cgra_acc', [], m.get_ports())

    return m
