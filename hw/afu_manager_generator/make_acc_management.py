import traceback

from veriloggen import *

from make_acc import make_acc
from make_arbiter_controller_rd_req_tree import make_arbiter_controller_rd_req_tree
from make_arbiter_controller_wr_req_tree import make_arbiter_controller_wr_req_tree
from make_conf_receiver import make_conf_receiver
from make_dsm_controller import make_dsm_controller
from make_input_queue_controller import make_input_queue_controller
from make_output_queue_controller import make_output_queue_controller


def acc_management_args_process(accs):
    acc_count = 0
    qtd_in_queue = 0
    qtd_out_queue = 0
    if len(accs) < 1:
        print('At least one acc should be created!')
        print('Process aborted.')
        exit()
    for i in accs:
        if i[0] == 0 or i[1] == 0:
            print('The quantities of inputs or outputs of acc %d can not be equal to 0!' % acc_count)
            print('Process aborted.')
            exit(255)
        qtd_in_queue = qtd_in_queue + i[0]
        qtd_out_queue = qtd_out_queue + i[1]
        acc_count = acc_count + 1

    return acc_count, qtd_in_queue, qtd_out_queue


def make_acc_management(accs):
    try:
        acc_count, qtd_in_queue, qtd_out_queue = acc_management_args_process(accs)
        RADIX = 8
        MAX_AFUS = 64
        m = Module('acc_management')
        ADDR_WIDTH = m.Parameter('ADDR_WIDTH', 48)
        QTD_WIDTH = m.Parameter('QTD_WIDTH', 32)
        DATA_WIDTH = m.Parameter('DATA_WIDTH', 512)
        CONF_ID_QUEUE_WIDTH = m.Parameter('CONF_ID_QUEUE_WIDTH', 32)
        TAG_WIDTH = m.Parameter('TAG_WIDTH', 16)

        clk = m.Input('clk')
        rst = m.Input('rst')
        rst_accs = m.Input('rst_accs', MAX_AFUS)
        start_accs = m.Input('start_accs', MAX_AFUS)

        conf_valid = m.Input('conf_valid', 2)
        conf = m.Input('conf', 128)

        # Reading
        req_rd_en = m.OutputReg('req_rd_en')
        req_rd_addr = m.OutputReg('req_rd_addr', ADDR_WIDTH)
        req_rd_mdata = m.OutputReg('req_rd_mdata', TAG_WIDTH)
        req_rd_available = m.Input('req_rd_available')
        resp_rd_valid = m.Input('resp_rd_valid')
        resp_rd_data = m.Input('resp_rd_data', DATA_WIDTH)
        resp_rd_mdata = m.Input('resp_rd_mdata', TAG_WIDTH)

        # Writing
        req_wr_available = m.Input('req_wr_available')
        req_wr_en = m.OutputReg('req_wr_en')
        req_wr_addr = m.OutputReg('req_wr_addr', ADDR_WIDTH)
        req_wr_mdata = m.OutputReg('req_wr_mdata', TAG_WIDTH)
        req_wr_data = m.OutputReg('req_wr_data', DATA_WIDTH)
        resp_wr_valid = m.Input('resp_wr_valid')
        resp_wr_mdata = m.Input('resp_wr_mdata', TAG_WIDTH)

        # Infos
        bits_info = (16 * 32)
        info = m.OutputReg('info', bits_info)

        m.EmbeddedCode('')
        req_wr_en_in = m.Wire('req_wr_en_in', (qtd_out_queue + acc_count))
        req_wr_data_in = m.Wire('req_wr_data_in',
                                EmbeddedCode(
                                    '(DATA_WIDTH + ADDR_WIDTH + TAG_WIDTH) * %d' % (qtd_out_queue + acc_count)))
        req_wr_available_in = m.Wire('req_wr_available_in', (qtd_out_queue + acc_count))
        m.EmbeddedCode('')
        req_wr_available_out = m.Wire('req_wr_available_out')
        req_wr_en_out = m.Wire('req_wr_en_out')
        req_wr_data_out = m.Wire('req_wr_data_out', DATA_WIDTH + ADDR_WIDTH + TAG_WIDTH)
        m.EmbeddedCode('')
        req_rd_en_in = m.Wire('req_rd_en_in', qtd_in_queue)
        req_rd_data_in = m.Wire('req_rd_data_in', EmbeddedCode('(ADDR_WIDTH + TAG_WIDTH) * %d' % qtd_in_queue))
        req_rd_available_in = m.Wire('req_rd_available_in', qtd_in_queue)
        m.EmbeddedCode('')
        req_rd_available_out = m.Wire('req_rd_available_out')
        req_rd_en_out = m.Wire('req_rd_en_out')
        req_rd_data_out = m.Wire('req_rd_data_out', ADDR_WIDTH + TAG_WIDTH)
        m.EmbeddedCode('')
        conf_receiver = make_conf_receiver()
        input_queue_controller = make_input_queue_controller(conf_receiver)
        output_queue_controller = make_output_queue_controller(conf_receiver)
        output_queue_controller_dsm = make_output_queue_controller(conf_receiver)
        dsm_controller_accs = make_dsm_controller(conf_receiver)

        acc_id = 0
        ini_in_queue_id = 0
        ini_out_queue_id = 0
        con_out_wire = 0
        for num_in_queue, num_out_queue in accs:
            acc = make_acc(acc_id, input_queue_controller, output_queue_controller, output_queue_controller_dsm,
                           dsm_controller_accs)
            range1 = '%d*(%d*(ADDR_WIDTH+TAG_WIDTH)+(ADDR_WIDTH+TAG_WIDTH))-1:%d*(%d*(ADDR_WIDTH+TAG_WIDTH))' % (
                num_in_queue, acc_id, num_in_queue, acc_id)
            range2 = '%d*(%d*(DATA_WIDTH+ADDR_WIDTH + TAG_WIDTH)+(DATA_WIDTH+ADDR_WIDTH + TAG_WIDTH))-1:%d*(%d*(DATA_WIDTH+ADDR_WIDTH + TAG_WIDTH))' % (
                (num_out_queue + 1), acc_id, (num_out_queue + 1), acc_id)
            params = [('ADDR_WIDTH', ADDR_WIDTH), ('QTD_WIDTH', QTD_WIDTH), ('DATA_WIDTH', DATA_WIDTH),
                      ('CONF_ID_QUEUE_WIDTH', CONF_ID_QUEUE_WIDTH), ('INITIAL_INPUT_QUEUE_ID', ini_in_queue_id),
                      ('INITIAL_OUTPUT_QUEUE_ID', ini_out_queue_id), ('NUM_INPUT_QUEUES', num_in_queue),
                      ('NUM_OUTPUT_QUEUES', num_out_queue), ('TAG_WIDTH', TAG_WIDTH)]
            con = [('clk', clk), ('rst', rst | rst_accs[acc_id]), ('start', start_accs[acc_id]),
                   ('conf_valid', conf_valid), ('conf', conf[0:(ADDR_WIDTH + QTD_WIDTH + CONF_ID_QUEUE_WIDTH)]),
                   ('available_read', req_rd_available_in[ini_in_queue_id:ini_in_queue_id + num_in_queue]),
                   ('request_read', req_rd_en_in[ini_in_queue_id:ini_in_queue_id + num_in_queue]),
                   ('request_data', req_rd_data_in[EmbeddedCode(range1)]),

                   ('read_data_valid', resp_rd_valid), ('read_queue_id', resp_rd_mdata), ('read_data', resp_rd_data),
                   ('available_write', req_wr_available_in[con_out_wire:con_out_wire + (num_out_queue + 1)]),
                   ('request_write', req_wr_en_in[con_out_wire:con_out_wire + (num_out_queue + 1)]),
                   ('write_data', req_wr_data_in[EmbeddedCode(range2)]),
                   ('write_data_valid', resp_wr_valid),
                   ('write_queue_id', resp_wr_mdata)]
            m.Instance(acc, 'acc_%d_%dx%d' % (acc_id, num_in_queue, num_out_queue), params, con)
            acc_id = acc_id + 1
            ini_in_queue_id = ini_in_queue_id + num_in_queue
            ini_out_queue_id = ini_out_queue_id + num_out_queue
            con_out_wire = con_out_wire + (num_out_queue + 1)

        ac_in = make_arbiter_controller_rd_req_tree(RADIX, qtd_in_queue)
        params = [('DATA_WIDTH', ADDR_WIDTH + TAG_WIDTH), ('INPUT_FIFO_DEPTH_BITS', 10), ('OUTPUT_FIFO_DEPTH_BITS', 10)]
        con = [('clk', clk), ('rst', rst), ('req_wr_en_in', req_rd_en_in), ('req_wr_data_in', req_rd_data_in),
               ('req_wr_available_in', req_rd_available_in), ('req_wr_available_out', req_rd_available_out),
               ('req_wr_en_out', req_rd_en_out), ('req_wr_data_out', req_rd_data_out)]
        m.Instance(ac_in, 'arbiter_controller_rd_req_tree', params, con)

        ac_out = make_arbiter_controller_wr_req_tree(RADIX, (qtd_out_queue + acc_count))

        params = [('DATA_WIDTH', DATA_WIDTH + ADDR_WIDTH + TAG_WIDTH), ('INPUT_FIFO_DEPTH_BITS', 10),
                  ('OUTPUT_FIFO_DEPTH_BITS', 10)]
        con = [('clk', clk), ('rst', rst), ('req_wr_en_in', req_wr_en_in), ('req_wr_data_in', req_wr_data_in),
               ('req_wr_available_in', req_wr_available_in), ('req_wr_available_out', req_wr_available_out),
               ('req_wr_en_out', req_wr_en_out), ('req_wr_data_out', req_wr_data_out)]
        m.Instance(ac_out, 'arbiter_controller_wr_req_tree', params, con)

        req_rd_available_out.assign(req_rd_available)
        req_wr_available_out.assign(req_wr_available)

        m.Always(Posedge(clk))(
            If(rst)(
                req_rd_en(Int(0, 1, 2)),
                req_rd_addr(0),
                req_rd_mdata(0)
            ).Else(
                req_rd_en(req_rd_en_out),
                req_rd_addr(req_rd_data_out[TAG_WIDTH:TAG_WIDTH + ADDR_WIDTH]),
                req_rd_mdata(req_rd_data_out[0:TAG_WIDTH])
            )
        )

        m.Always(Posedge(clk))(
            If(rst)(
                req_wr_en(Int(0, 1, 2)),
                req_wr_addr(0),
                req_wr_mdata(0),
                req_wr_data(0)
            ).Else(
                req_wr_en(req_wr_en_out),
                req_wr_data(req_wr_data_out[TAG_WIDTH + ADDR_WIDTH:DATA_WIDTH + TAG_WIDTH + ADDR_WIDTH]),
                req_wr_addr(req_wr_data_out[TAG_WIDTH:TAG_WIDTH + ADDR_WIDTH]),
                req_wr_mdata(req_wr_data_out[0:TAG_WIDTH])
            )
        )
        j = 0
        code = ''
        for i in accs:
            code = code + 'info[%d:%d] <= {8\'d%d, 8\'d%d};\n' % (j + 15, j, i[1], i[0])
            j = j + 16
        m.Always(Posedge(clk))(
            If(rst)(
                info(Int(0, info.width, 10))
            ).Else(
                EmbeddedCode(code)
            )
        )
        print('Accelerator management successfully created!')
        print('Amount of accelerator: %d' % len(accs))
        print('Amount of Input Queue: %d' % qtd_in_queue)
        print('Amount of Output Queue: %d' % (qtd_out_queue))
        return m
    except:
        print('Failed to create the accelerator management!')
        print(traceback.format_exc())
        exit(255)
        raise
