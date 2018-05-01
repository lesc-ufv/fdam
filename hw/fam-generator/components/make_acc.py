from veriloggen import *

from components.make_acc_user import make_acc_user


def make_acc(acc_id, input_queue_controller, output_queue_controller, output_queue_controller_dsm, dsm_controller):
    m = Module('acc_%d' % acc_id)
    ADDR_WIDTH = m.Parameter('ADDR_WIDTH', 64)
    QTD_WIDTH = m.Parameter('QTD_WIDTH', 32)
    DATA_WIDTH = m.Parameter('DATA_WIDTH', 512)
    CONF_ID_QUEUE_WIDTH = m.Parameter('CONF_ID_QUEUE_WIDTH', 32)
    INITIAL_INPUT_QUEUE_ID = m.Parameter('INITIAL_INPUT_QUEUE_ID', 0)
    INITIAL_OUTPUT_QUEUE_ID = m.Parameter('INITIAL_OUTPUT_QUEUE_ID', 0)
    NUM_INPUT_QUEUES = m.Parameter('NUM_INPUT_QUEUES', 1)
    NUM_OUTPUT_QUEUES = m.Parameter('NUM_OUTPUT_QUEUES', 1)
    TAG_WIDTH = m.Parameter('TAG_WIDTH', 16)

    clk = m.Input('clk')
    rst = m.Input('rst')
    start = m.Input('start')
    conf_valid = m.Input('conf_valid', 2)
    conf = m.Input('conf', EmbeddedCode('ADDR_WIDTH + QTD_WIDTH + CONF_ID_QUEUE_WIDTH'))
    available_read = m.Input('available_read', NUM_INPUT_QUEUES)
    request_read = m.Output('request_read', NUM_INPUT_QUEUES)
    request_data = m.Output('request_data', EmbeddedCode('(ADDR_WIDTH+TAG_WIDTH)*NUM_INPUT_QUEUES'))
    read_data_valid = m.Input('read_data_valid')
    read_queue_id = m.Input('read_queue_id', TAG_WIDTH)
    read_data = m.Input('read_data', DATA_WIDTH)
    available_write = m.Input('available_write', (NUM_OUTPUT_QUEUES + 1))
    request_write = m.Output('request_write', (NUM_OUTPUT_QUEUES + 1))
    write_data = m.Output('write_data', EmbeddedCode('(DATA_WIDTH+ADDR_WIDTH+TAG_WIDTH)*(NUM_OUTPUT_QUEUES+1)'))
    write_data_valid = m.Input('write_data_valid')
    write_queue_id = m.Input('write_queue_id', TAG_WIDTH)

    rst_reg = m.Reg('rst_reg')
    start_reg = m.Reg('start_reg')
    conf_valid_reg = m.Reg('conf_valid_reg', 2)
    conf_reg = m.Reg('conf_reg', EmbeddedCode('ADDR_WIDTH + QTD_WIDTH + CONF_ID_QUEUE_WIDTH'))
    read_data_valid_reg = m.Reg('read_data_valid_reg')
    read_queue_id_reg = m.Reg('read_queue_id_reg', TAG_WIDTH)
    read_data_reg = m.Reg('read_data_reg', DATA_WIDTH)
    write_data_valid_reg = m.Reg('write_data_valid_reg')
    write_queue_id_reg = m.Reg('write_queue_id_reg', TAG_WIDTH)

    acc_user_available_read = m.Wire('acc_user_available_read', NUM_INPUT_QUEUES)
    acc_user_request_read = m.Wire('acc_user_request_read', NUM_INPUT_QUEUES)
    acc_user_read_data = m.Wire('acc_user_read_data', EmbeddedCode('DATA_WIDTH*NUM_INPUT_QUEUES'))
    acc_user_read_data_valid = m.Wire('acc_user_read_data_valid', NUM_INPUT_QUEUES)

    acc_user_available_write = m.Wire('acc_user_available_write', NUM_OUTPUT_QUEUES)
    acc_user_request_write = m.Wire('acc_user_request_write', NUM_OUTPUT_QUEUES)
    acc_user_write_data = m.Wire('acc_user_write_data', EmbeddedCode('DATA_WIDTH*(NUM_OUTPUT_QUEUES)'))

    acc_user_done = m.Wire('acc_user_done')
    acc_user_done_dsm = m.Wire('acc_user_done_dsm')
    has_peding_rd = m.Wire('has_peding_rd', NUM_INPUT_QUEUES)
    has_peding_wr = m.Wire('has_peding_wr', NUM_OUTPUT_QUEUES)
    has_peding = m.Wire('has_peding')

    input_queue_done = m.Wire('input_queue_done', NUM_INPUT_QUEUES)
    output_queue_done = m.Wire('output_queue_done', NUM_OUTPUT_QUEUES)

    idx_in_queue = m.Genvar('idx_in_queue')
    idx_out_queue = m.Genvar('idx_out_queue')

    params = [('ID_QUEUE', INITIAL_INPUT_QUEUE_ID + idx_in_queue), ('ADDR_WIDTH', ADDR_WIDTH), ('QTD_WIDTH', QTD_WIDTH),
              ('DATA_WIDTH', DATA_WIDTH), ('CONF_ID_QUEUE_WIDTH', CONF_ID_QUEUE_WIDTH), ('TAG_WIDTH', TAG_WIDTH)]
    con = [('clk', clk), ('rst', rst_reg), ('start', start_reg), ('conf_valid', conf_valid_reg), ('conf', conf_reg),
           ('available_read', available_read[idx_in_queue]), ('has_rd_peding', has_peding_rd[idx_in_queue]),
           ('request_read', request_read[idx_in_queue]),
           ('request_data', request_data[
                            idx_in_queue * (ADDR_WIDTH + TAG_WIDTH):idx_in_queue * (ADDR_WIDTH + TAG_WIDTH) + (
                                    ADDR_WIDTH + TAG_WIDTH)]),
           ('read_data_valid', read_data_valid_reg), ('read_queue_id', read_queue_id_reg), ('read_data', read_data_reg),
           ('acc_user_available_read', acc_user_available_read[idx_in_queue]),
           ('acc_user_request_read', acc_user_request_read[idx_in_queue]),
           ('acc_user_read_data', acc_user_read_data[idx_in_queue * DATA_WIDTH:idx_in_queue * DATA_WIDTH + DATA_WIDTH]),
           ('acc_user_read_data_valid', acc_user_read_data_valid[idx_in_queue]),
           ('done', input_queue_done[idx_in_queue])]
    genInputQueue = m.GenerateFor(idx_in_queue(0), idx_in_queue < NUM_INPUT_QUEUES, idx_in_queue.inc(),
                                  'gen_in_queue_controller')
    genInputQueue.Instance(input_queue_controller, 'input_queue_controller', params, con)

    params = [('ID_QUEUE', INITIAL_OUTPUT_QUEUE_ID + idx_out_queue), ('ADDR_WIDTH', ADDR_WIDTH),
              ('QTD_WIDTH', QTD_WIDTH), ('DATA_WIDTH', DATA_WIDTH), ('CONF_ID_QUEUE_WIDTH', CONF_ID_QUEUE_WIDTH),
              ('TAG_WIDTH', TAG_WIDTH)]
    con = [('clk', clk), ('rst', rst_reg), ('start', start_reg), ('conf_valid', conf_valid_reg), ('conf', conf_reg),
           ('available_write', available_write[idx_out_queue]), ('has_wr_peding', has_peding_wr[idx_out_queue]),
           ('request_write', request_write[idx_out_queue]),
           ('write_data', write_data[idx_out_queue * (DATA_WIDTH + ADDR_WIDTH + TAG_WIDTH):idx_out_queue * (
                   DATA_WIDTH + ADDR_WIDTH + TAG_WIDTH) + (DATA_WIDTH + ADDR_WIDTH + TAG_WIDTH)]),
           ('write_data_valid', write_data_valid_reg), ('write_queue_id', write_queue_id_reg),
           ('acc_user_available_write', acc_user_available_write[idx_out_queue]),
           ('acc_user_request_write', acc_user_request_write[idx_out_queue]),
           ('acc_user_write_data',
            acc_user_write_data[(idx_out_queue) * DATA_WIDTH:(idx_out_queue) * DATA_WIDTH + DATA_WIDTH]),
           ('acc_user_done', acc_user_done),
           ('done', output_queue_done[idx_out_queue])]
    genOutputQueue = m.GenerateFor(idx_out_queue(0), idx_out_queue < NUM_OUTPUT_QUEUES, idx_out_queue.inc(),
                                   'gen_out_queue_controller')
    genOutputQueue.Instance(output_queue_controller, 'output_queue_controller', params, con)

    params = [('ACC_ID', acc_id), ('ADDR_WIDTH', ADDR_WIDTH), ('QTD_WIDTH', QTD_WIDTH),
              ('TAG_WIDTH', TAG_WIDTH),
              ('CONF_ID_QUEUE_WIDTH', CONF_ID_QUEUE_WIDTH), ('DATA_WIDTH', DATA_WIDTH),
              ('NUM_INPUT_QUEUES', NUM_INPUT_QUEUES), ('NUM_OUTPUT_QUEUES', NUM_OUTPUT_QUEUES)]

    con = [('clk', clk), ('rst', rst_reg), ('start', start_reg), ('done_rd', input_queue_done),
           ('done_wr', output_queue_done),
           ('done_acc', acc_user_done_dsm),
           ('acc_req_rd_data_en', acc_user_request_read),
           ('acc_req_wr_data_en', acc_user_request_write),
           ('conf_valid', conf_valid_reg),
           ('conf', conf_reg),
           ('available_write', available_write[NUM_OUTPUT_QUEUES]),
           ('request_write', request_write[NUM_OUTPUT_QUEUES]),
           ('write_data',
            write_data[(NUM_OUTPUT_QUEUES) * (DATA_WIDTH + ADDR_WIDTH + TAG_WIDTH):(NUM_OUTPUT_QUEUES) * (
                    DATA_WIDTH + ADDR_WIDTH + TAG_WIDTH) + (DATA_WIDTH + ADDR_WIDTH + TAG_WIDTH)]),
           ('write_data_valid', write_data_valid_reg), ('write_queue_id', write_queue_id_reg)]

    m.Instance(dsm_controller, 'dsm_controller', params, con)

    acc_user = make_acc_user(acc_id)
    params = [('DATA_WIDTH', DATA_WIDTH), ('NUM_INPUT_QUEUES', NUM_INPUT_QUEUES),
              ('NUM_OUTPUT_QUEUES', NUM_OUTPUT_QUEUES)]
    con = [('clk', clk), ('rst', rst_reg), ('start', start_reg), ('acc_user_done_rd_data', input_queue_done),
           ('acc_user_done_wr_data', output_queue_done), ('acc_user_available_read', acc_user_available_read),
           ('acc_user_request_read', acc_user_request_read), ('acc_user_read_data_valid', acc_user_read_data_valid),
           ('acc_user_read_data', acc_user_read_data), ('acc_user_available_write', acc_user_available_write),
           ('acc_user_request_write', acc_user_request_write), ('acc_user_write_data', acc_user_write_data),
           ('acc_user_done', acc_user_done)]
    m.Instance(acc_user, 'acc_user_%d' % acc_id, params, con)

    has_peding.assign(Uor(Cat(has_peding_rd, has_peding_wr)))
    acc_user_done_dsm.assign(AndList(acc_user_done, ~has_peding))

    m.Always(Posedge(clk))(
        rst_reg(rst),
        start_reg(start),
        conf_valid_reg(conf_valid),
        conf_reg(conf),
        read_data_valid_reg(read_data_valid),
        read_queue_id_reg(read_queue_id),
        read_data_reg(read_data),
        write_data_valid_reg(write_data_valid),
        write_queue_id_reg(write_queue_id)
    )

    return m
