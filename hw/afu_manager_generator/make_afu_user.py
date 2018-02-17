from veriloggen import *

def make_afu_user(afu_id):
    m = Module('afu_user_%d'%afu_id)
    DATA_WIDTH = m.Parameter('DATA_WIDTH',512)
    NUM_INPUT_QUEUES = m.Parameter('NUM_INPUT_QUEUES',1)
    NUM_OUTPUT_QUEUES = m.Parameter('NUM_OUTPUT_QUEUES',1)
    clk = m.Input('clk')
    rst = m.Input('rst')
    start = m.Input('start')
    afu_user_done_rd_data = m.Input('afu_user_done_rd_data',NUM_INPUT_QUEUES)
    afu_user_done_wr_data = m.Input('afu_user_done_wr_data', NUM_OUTPUT_QUEUES)
    afu_user_available_read = m.Input('afu_user_available_read',NUM_INPUT_QUEUES)
    afu_user_request_read = m.OutputReg('afu_user_request_read',NUM_INPUT_QUEUES)
    afu_user_read_data_valid = m.Input('afu_user_read_data_valid',NUM_INPUT_QUEUES)
    afu_user_read_data = m.Input('afu_user_read_data',DATA_WIDTH*NUM_INPUT_QUEUES)
    afu_user_available_write = m.Input('afu_user_available_write',NUM_OUTPUT_QUEUES)
    afu_user_request_write = m.OutputReg('afu_user_request_write',NUM_OUTPUT_QUEUES)
    afu_user_write_data = m.OutputReg('afu_user_write_data',DATA_WIDTH*NUM_OUTPUT_QUEUES)
    afu_user_done = m.OutputReg('afu_user_done')

    m.EmbeddedCode("\n/* user code here! */\n")


    return m