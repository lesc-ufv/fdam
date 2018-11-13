from veriloggen import *

from make_acc_user import make_acc_user
from make_reg_tree_pipeline import make_reg_tree_pipeline


def make_acc(acc_param, input_queue_controller, output_queue_controller, dsm_controller,accelerator_module=None):
    acc_id = acc_param['acc_id']
    addr_width = acc_param['addr_width']
    qtd_width = acc_param['qtd_width']
    data_width = acc_param['data_width']
    conf_id_queue_width = acc_param['conf_id_queue_width']
    initial_input_queue_id = acc_param['initial_input_queue_id']
    initial_output_queue_id = acc_param['initial_output_queue_id']
    num_input_queues = acc_param['num_input_queues']
    num_output_queues = acc_param['num_output_queues']
    tag_width = acc_param['tag_width']

    m = Module('fdam_acc_%d' % acc_id)

    clk = m.Input('clk')
    rst = m.Input('rst')
    start = m.Input('start')
    conf_valid = m.Input('conf_valid', 2)
    conf = m.Input('conf', addr_width + qtd_width + conf_id_queue_width)
    available_read = m.Input('available_read', num_input_queues)
    request_read = m.Output('request_read', num_input_queues)
    request_data = m.Output('request_data', (addr_width + tag_width) * num_input_queues)
    read_data_valid = m.Input('read_data_valid')
    read_queue_id = m.Input('read_queue_id', tag_width)
    read_data = m.Input('read_data', data_width)
    available_write = m.Input('available_write', (num_output_queues + 1))
    request_write = m.Output('request_write', (num_output_queues + 1))
    write_data = m.Output('write_data', (data_width + addr_width + tag_width) * (num_output_queues + 1))
    write_data_valid = m.Input('write_data_valid')
    write_queue_id = m.Input('write_queue_id', tag_width)

    rst_reg = m.Wire('rst_reg', num_input_queues + num_output_queues + 2)
    start_reg = m.Wire('start_reg', num_input_queues + num_output_queues + 2)

    conf_valid_reg = m.Reg('conf_valid_reg', 2)
    conf_reg = m.Reg('conf_reg', addr_width + qtd_width + conf_id_queue_width)
    read_data_valid_reg = m.Reg('read_data_valid_reg')
    read_queue_id_reg = m.Reg('read_queue_id_reg', tag_width)
    read_data_reg = m.Reg('read_data_reg', data_width)
    write_data_valid_reg = m.Reg('write_data_valid_reg')
    write_queue_id_reg = m.Reg('write_queue_id_reg', tag_width)

    acc_user_available_read = m.Wire('acc_user_available_read', num_input_queues)
    acc_user_request_read = m.Wire('acc_user_request_read', num_input_queues)
    acc_user_read_data = m.Wire('acc_user_read_data', data_width * num_input_queues)
    acc_user_read_data_valid = m.Wire('acc_user_read_data_valid', num_input_queues)

    acc_user_available_write = m.Wire('acc_user_available_write', num_output_queues)
    acc_user_request_write = m.Wire('acc_user_request_write', num_output_queues)
    acc_user_write_data = m.Wire('acc_user_write_data', data_width * (num_output_queues))

    acc_user_done = m.Wire('acc_user_done')
    acc_user_done_dsm = m.Wire('acc_user_done_dsm')
    has_peding_rd = m.Wire('has_peding_rd', num_input_queues)
    has_peding_wr = m.Wire('has_peding_wr', num_output_queues)
    has_peding = m.Wire('has_peding')

    input_queue_done = m.Wire('input_queue_done', num_input_queues)
    output_queue_done = m.Wire('output_queue_done', num_output_queues)

    idx_in_queue = m.Genvar('idx_in_queue')
    idx_out_queue = m.Genvar('idx_out_queue')

    reg_tree = make_reg_tree_pipeline(4, rst_reg.width, 2)
    param = [('DATA_WIDTH', 1)]
    con = [('clk', clk), ('in', rst)]
    con1 = [('out_%d' % i, rst_reg[i]) for i in range(rst_reg.width)]
    m.Instance(reg_tree, 'rst_' + reg_tree.name, param, con + con1)

    param = [('DATA_WIDTH', 1)]
    con = [('clk', clk), ('in', start)]
    con1 = [('out_%d' % i, start_reg[i]) for i in range(start_reg.width)]
    m.Instance(reg_tree, 'start_' + reg_tree.name, param, con + con1)

    params = [('ID_QUEUE', Add(initial_input_queue_id, idx_in_queue)), ('ADDR_WIDTH', addr_width),
              ('QTD_WIDTH', qtd_width),
              ('DATA_WIDTH', data_width), ('CONF_ID_QUEUE_WIDTH', conf_id_queue_width), ('TAG_WIDTH', tag_width)]

    con = [('clk', clk), ('rst', rst_reg[idx_in_queue]), ('start', start_reg[idx_in_queue]),
           ('conf_valid', conf_valid_reg), ('conf', conf_reg),
           ('available_read', available_read[idx_in_queue]), ('has_rd_peding', has_peding_rd[idx_in_queue]),
           ('request_read', request_read[idx_in_queue]),
           ('request_data',
            request_data[idx_in_queue * (addr_width + tag_width):idx_in_queue * (addr_width + tag_width) + (
                    addr_width + tag_width)]),
           ('read_data_valid', read_data_valid_reg), ('read_queue_id', read_queue_id_reg), ('read_data', read_data_reg),
           ('acc_user_available_read', acc_user_available_read[idx_in_queue]),
           ('acc_user_request_read', acc_user_request_read[idx_in_queue]),
           ('acc_user_read_data', acc_user_read_data[idx_in_queue * data_width:idx_in_queue * data_width + data_width]),
           ('acc_user_read_data_valid', acc_user_read_data_valid[idx_in_queue]),
           ('done', input_queue_done[idx_in_queue])]
    genInputQueue = m.GenerateFor(idx_in_queue(0), idx_in_queue < num_input_queues, idx_in_queue.inc(),
                                  'gen_in_queue_controller')
    genInputQueue.Instance(input_queue_controller, 'input_queue_controller', params, con)

    params = [('ID_QUEUE', Add(initial_output_queue_id, idx_out_queue)), ('ADDR_WIDTH', addr_width),
              ('QTD_WIDTH', qtd_width), ('DATA_WIDTH', data_width), ('CONF_ID_QUEUE_WIDTH', conf_id_queue_width),
              ('TAG_WIDTH', tag_width)]
    con = [('clk', clk), ('rst', rst_reg[Add(num_input_queues, idx_out_queue)]),
           ('start', start_reg[Add(num_input_queues, idx_out_queue)]), ('conf_valid', conf_valid_reg),
           ('conf', conf_reg),
           ('available_write', available_write[idx_out_queue]), ('has_wr_peding', has_peding_wr[idx_out_queue]),
           ('request_write', request_write[idx_out_queue]),
           ('write_data', write_data[idx_out_queue * (data_width + addr_width + tag_width):idx_out_queue * (
                   data_width + addr_width + tag_width) + (data_width + addr_width + tag_width)]),
           ('write_data_valid', write_data_valid_reg), ('write_queue_id', write_queue_id_reg),
           ('acc_user_available_write', acc_user_available_write[idx_out_queue]),
           ('acc_user_request_write', acc_user_request_write[idx_out_queue]),
           ('acc_user_write_data',
            acc_user_write_data[(idx_out_queue) * data_width:(idx_out_queue) * data_width + data_width]),
           ('acc_user_done', acc_user_done),
           ('done', output_queue_done[idx_out_queue])]
    genOutputQueue = m.GenerateFor(idx_out_queue(0), idx_out_queue < num_output_queues, idx_out_queue.inc(),
                                   'gen_out_queue_controller')
    genOutputQueue.Instance(output_queue_controller, 'output_queue_controller', params, con)

    params = [('ACC_ID', acc_id), ('ADDR_WIDTH', addr_width), ('QTD_WIDTH', qtd_width),
              ('TAG_WIDTH', tag_width),
              ('CONF_ID_QUEUE_WIDTH', conf_id_queue_width), ('DATA_WIDTH', data_width),
              ('NUM_INPUT_QUEUES', num_input_queues), ('NUM_OUTPUT_QUEUES', num_output_queues)]

    con = [('clk', clk), ('rst', rst_reg[num_input_queues + num_output_queues]),
           ('start', start_reg[num_input_queues + num_output_queues]), ('done_rd', input_queue_done),
           ('done_wr', output_queue_done),
           ('done_acc', acc_user_done_dsm),
           ('acc_req_rd_data_en', acc_user_request_read),
           ('acc_req_wr_data_en', acc_user_request_write),
           ('conf_valid', conf_valid_reg),
           ('conf', conf_reg),
           ('available_write', available_write[num_output_queues]),
           ('request_write', request_write[num_output_queues]),
           ('write_data',
            write_data[(num_output_queues) * (data_width + addr_width + tag_width):(num_output_queues) * (
                    data_width + addr_width + tag_width) + (data_width + addr_width + tag_width)]),
           ('write_data_valid', write_data_valid_reg), ('write_queue_id', write_queue_id_reg)]

    m.Instance(dsm_controller, 'dsm_controller', params, con)

    acc_user = make_acc_user(acc_param['acc_id'], acc_param['data_width'], acc_param['num_input_queues'],
                             acc_param['num_output_queues'],accelerator_module)
    params = []
    con = [('clk', clk), ('rst', rst_reg[num_input_queues + num_output_queues + 1]),
           ('start', start_reg[num_input_queues + num_output_queues + 1]), ('acc_user_done_rd_data', input_queue_done),
           ('acc_user_done_wr_data', output_queue_done), ('acc_user_available_read', acc_user_available_read),
           ('acc_user_request_read', acc_user_request_read), ('acc_user_read_data_valid', acc_user_read_data_valid),
           ('acc_user_read_data', acc_user_read_data), ('acc_user_available_write', acc_user_available_write),
           ('acc_user_request_write', acc_user_request_write), ('acc_user_write_data', acc_user_write_data),
           ('acc_user_done', acc_user_done)]
    m.Instance(acc_user, 'acc_user_%d' % acc_id, params, con)

    has_peding.assign(Uor(Cat(has_peding_rd, has_peding_wr)))
    acc_user_done_dsm.assign(AndList(acc_user_done, ~has_peding))

    m.Always(Posedge(clk))(
        conf_valid_reg(conf_valid),
        conf_reg(conf),
        read_data_valid_reg(read_data_valid),
        read_queue_id_reg(read_queue_id),
        read_data_reg(read_data),
        write_data_valid_reg(write_data_valid),
        write_queue_id_reg(write_queue_id)
    )

    return m
