from veriloggen import *


def make_acc_user(acc_id):
    m = Module('acc_user_%d' % acc_id)
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

    code = '  /* user code here! */\n\
  initial begin\n\
    assert(NUM_INPUT_QUEUES  == NUM_OUTPUT_QUEUES) else begin\n\
        $fatal("Erro NUM_INPUT_QUEUES != NUM_OUTPUT_QUEUES");\n\
    end \n\
  end \n\
  wire [NUM_INPUT_QUEUES-1:0] loopback_done;\n\
  assign acc_user_done = &loopback_done;\n\
  genvar i;\n\
  generate \n\
     for(i = 0;i < NUM_INPUT_QUEUES; i = i + 1)begin : gen_loopback\n\
        loopback_1x1 #(.DATA_WIDTH(DATA_WIDTH))loopback_1x1(\n\
           .clk(clk),\n\
           .rst(rst),\n\
           .start(start),\n\
           .acc_user_done_rd_data(acc_user_done_rd_data[i]),\n\
           .acc_user_done_wr_data(acc_user_done_wr_data[i]),\n\
           .acc_user_available_read(acc_user_available_read[i]),\n\
           .acc_user_request_read(acc_user_request_read[i]),\n\
           .acc_user_read_data_valid(acc_user_read_data_valid[i]),\n\
           .acc_user_read_data(acc_user_read_data[i*DATA_WIDTH + (DATA_WIDTH-1):i*DATA_WIDTH]),\n\
           .acc_user_available_write(acc_user_available_write[i]),\n\
           .acc_user_request_write(acc_user_request_write[i]),\n\
           .acc_user_write_data(acc_user_write_data[i*DATA_WIDTH+(DATA_WIDTH-1):i*DATA_WIDTH]),\n\
           .acc_user_done(loopback_done[i])\n\
        );\n\
     end \n\
  endgenerate'

    m.EmbeddedCode(code)

    return m
