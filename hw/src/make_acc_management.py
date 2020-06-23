import traceback

from veriloggen import *

from common.make_reg_tree_pipeline import make_reg_tree_pipeline
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
        FIFO_DEPTH_BITS = 5

        m = Module('fdam_acc_management')

        addr_width = 48
        qtd_width = 32
        data_width = 512
        conf_id_queue_width = 32
        tag_width = 16

        clk = m.Input('clk')
        rst = m.Input('rst')
        rst_accs = m.Input('rst_accs', MAX_AFUS)
        start_accs = m.Input('start_accs', MAX_AFUS)

        conf_valid = m.Input('conf_valid', 2)
        conf = m.Input('conf', 128)

        # Reading
        req_rd_en = m.OutputReg('req_rd_en')
        req_rd_addr = m.OutputReg('req_rd_addr', addr_width)
        req_rd_mdata = m.OutputReg('req_rd_mdata', tag_width)
        req_rd_available = m.Input('req_rd_available')
        resp_rd_valid = m.Input('resp_rd_valid')
        resp_rd_data = m.Input('resp_rd_data', data_width)
        resp_rd_mdata = m.Input('resp_rd_mdata', tag_width)

        # Writing
        req_wr_available = m.Input('req_wr_available')
        req_wr_en = m.OutputReg('req_wr_en')
        req_wr_addr = m.OutputReg('req_wr_addr', addr_width)
        req_wr_mdata = m.OutputReg('req_wr_mdata', tag_width)
        req_wr_data = m.OutputReg('req_wr_data', data_width)
        resp_wr_valid = m.Input('resp_wr_valid')
        resp_wr_mdata = m.Input('resp_wr_mdata', tag_width)

        # Infos
        bits_info = (16 * 32)
        info = m.OutputReg('info', bits_info)

        m.EmbeddedCode('')
        req_wr_en_in = m.Wire('req_wr_en_in', (qtd_out_queue + acc_count))
        req_wr_data_in = m.Wire('req_wr_data_in', ((data_width + addr_width + tag_width) * (qtd_out_queue + acc_count)))
        req_wr_available_in = m.Wire('req_wr_available_in', (qtd_out_queue + acc_count))
        m.EmbeddedCode('')
        req_wr_available_out = m.Wire('req_wr_available_out')
        req_wr_en_out = m.Wire('req_wr_en_out')
        req_wr_data_out = m.Wire('req_wr_data_out', data_width + addr_width + tag_width)
        m.EmbeddedCode('')
        req_rd_en_in = m.Wire('req_rd_en_in', qtd_in_queue)
        req_rd_data_in = m.Wire('req_rd_data_in', (addr_width + tag_width) * qtd_in_queue)
        req_rd_available_in = m.Wire('req_rd_available_in', qtd_in_queue)
        m.EmbeddedCode('')
        req_rd_available_out = m.Wire('req_rd_available_out')
        req_rd_en_out = m.Wire('req_rd_en_out')
        req_rd_data_out = m.Wire('req_rd_data_out', addr_width + tag_width)
        m.EmbeddedCode('')
        num_accs = len(accs)
        rst_reg = m.Wire('rst_reg', num_accs + 5)
        conf_receiver = make_conf_receiver()
        input_queue_controller = make_input_queue_controller(conf_receiver)
        output_queue_controller = make_output_queue_controller(conf_receiver)
        dsm_controller_accs = make_dsm_controller(conf_receiver)

        reg_tree = make_reg_tree_pipeline(4, rst_reg.width, 2)
        param = [('DATA_WIDTH', 1)]
        con = [('clk', clk), ('in', rst)]
        con1 = [('out_%d' % i, rst_reg[i]) for i in range(rst_reg.width)]
        m.Instance(reg_tree, 'rst_' + reg_tree.name, param, con + con1)

        acc_id = 0
        ini_in_queue_id = 0
        ini_out_queue_id = 0
        con_out_wire = 0
        for num_in_queue, num_out_queue, acc_module in accs:
            acc_param = {
                'acc_id': acc_id,
                'addr_width': addr_width,
                'qtd_width': qtd_width,
                'data_width': data_width,
                'conf_id_queue_width': conf_id_queue_width,
                'initial_input_queue_id': ini_in_queue_id,
                'initial_output_queue_id': ini_out_queue_id,
                'num_input_queues': num_in_queue,
                'num_output_queues': num_out_queue,
                'tag_width': tag_width
            }
            acc = make_acc(acc_param, input_queue_controller, output_queue_controller, dsm_controller_accs, acc_module)

            begin = ini_in_queue_id * (addr_width + tag_width)
            end = (ini_in_queue_id + num_in_queue) * (addr_width + tag_width)

            begin2 = (con_out_wire) * (data_width + addr_width + tag_width)
            end2 = (con_out_wire + num_out_queue + 1) * (data_width + addr_width + tag_width)

            params = []
            con = [('clk', clk), ('rst', rst_reg[acc_id] | rst_accs[acc_id]), ('start', start_accs[acc_id]),
                   ('conf_valid', conf_valid), ('conf', conf[0:(addr_width + qtd_width + conf_id_queue_width)]),
                   ('available_read', req_rd_available_in[ini_in_queue_id:ini_in_queue_id + num_in_queue]),
                   ('request_read', req_rd_en_in[ini_in_queue_id:ini_in_queue_id + num_in_queue]),
                   ('request_data', req_rd_data_in[begin:end]), ('read_data_valid', resp_rd_valid),
                   ('read_queue_id', resp_rd_mdata), ('read_data', resp_rd_data),
                   ('available_write', req_wr_available_in[con_out_wire:con_out_wire + (num_out_queue + 1)]),
                   ('request_write', req_wr_en_in[con_out_wire:con_out_wire + (num_out_queue + 1)]),
                   ('write_data', req_wr_data_in[begin2:end2]), ('write_data_valid', resp_wr_valid),
                   ('write_queue_id', resp_wr_mdata)]
            m.Instance(acc, 'acc_%d_%dx%d' % (acc_id, num_in_queue, num_out_queue), params, con)
            acc_id = acc_id + 1
            ini_in_queue_id = ini_in_queue_id + num_in_queue
            ini_out_queue_id = ini_out_queue_id + num_out_queue
            con_out_wire = con_out_wire + (num_out_queue + 1)

        ac_in = make_arbiter_controller_rd_req_tree(RADIX, qtd_in_queue)
        params = [('DATA_WIDTH', addr_width + tag_width), ('INPUT_FIFO_DEPTH_BITS', FIFO_DEPTH_BITS),
                  ('OUTPUT_FIFO_DEPTH_BITS', FIFO_DEPTH_BITS)]
        con = [('clk', clk), ('rst', rst_reg[num_accs]), ('req_wr_en_in', req_rd_en_in),
               ('req_wr_data_in', req_rd_data_in),
               ('req_wr_available_in', req_rd_available_in), ('req_wr_available_out', req_rd_available_out),
               ('req_wr_en_out', req_rd_en_out), ('req_wr_data_out', req_rd_data_out)]
        m.Instance(ac_in, 'arbiter_controller_rd_req_tree', params, con)

        ac_out = make_arbiter_controller_wr_req_tree(RADIX, (qtd_out_queue + acc_count))

        params = [('DATA_WIDTH', data_width + addr_width + tag_width), ('INPUT_FIFO_DEPTH_BITS', FIFO_DEPTH_BITS),
                  ('OUTPUT_FIFO_DEPTH_BITS', FIFO_DEPTH_BITS)]
        con = [('clk', clk), ('rst', rst_reg[num_accs + 1]), ('req_wr_en_in', req_wr_en_in),
               ('req_wr_data_in', req_wr_data_in),
               ('req_wr_available_in', req_wr_available_in), ('req_wr_available_out', req_wr_available_out),
               ('req_wr_en_out', req_wr_en_out), ('req_wr_data_out', req_wr_data_out)]
        m.Instance(ac_out, 'arbiter_controller_wr_req_tree', params, con)

        req_rd_available_out.assign(req_rd_available)
        req_wr_available_out.assign(req_wr_available)

        m.Always(Posedge(clk))(
            If(rst_reg[num_accs + 2])(
                req_rd_en(Int(0, 1, 2))
            ).Else(
                req_rd_en(req_rd_en_out),
                req_rd_addr(req_rd_data_out[tag_width:tag_width + addr_width]),
                req_rd_mdata(req_rd_data_out[0:tag_width])
            )
        )

        m.Always(Posedge(clk))(
            If(rst_reg[num_accs + 3])(
                req_wr_en(Int(0, 1, 2))
            ).Else(
                req_wr_en(req_wr_en_out),
                req_wr_data(req_wr_data_out[tag_width + addr_width:data_width + tag_width + addr_width]),
                req_wr_addr(req_wr_data_out[tag_width:tag_width + addr_width]),
                req_wr_mdata(req_wr_data_out[0:tag_width])
            )
        )
        j = 0
        code = ''
        for i in accs:
            code = code + 'info[%d:%d] <= {8\'d%d, 8\'d%d};\n' % (j + 15, j, i[1], i[0])
            j = j + 16
        m.Always(Posedge(clk))(
            If(rst_reg[num_accs + 4])(
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
