from math import ceil

from veriloggen import *

from make_afu import make_afu
from make_arbiter_controller import make_arbiter_controller_tree
from make_conf_receiver import make_conf_receiver
from make_dsm_controller import make_dsm_controller
from make_input_queue_controller import make_input_queue_controller
from make_output_queue_controller import make_output_queue_controller
from util import numBits


def afu_manager_args_process(afus):
    afu_count = 0
    qtd_in_queue = 0
    qtd_out_queue = 0
    if len(afus) < 1:
        print('At least one afu should be created!')
        print('Process aborted.')
        exit()
    for i in afus:
        if i[0] == 0 or i[1] == 0:
            print('The quantities of inputs or outputs of afu %d can not be equal to 0!' % afu_count)
            print('Process aborted.')
            exit(255)
        qtd_in_queue = qtd_in_queue + i[0]
        qtd_out_queue = qtd_out_queue + i[1]
        afu_count = afu_count + 1

    return afu_count, qtd_in_queue, qtd_out_queue


def make_afu_manager(afus):
    try:
        afu_count, qtd_in_queue, qtd_out_queue = afu_manager_args_process(afus)
        RADIX = 4
        m = Module('afu_manager')
        ADDR_WIDTH = m.Parameter('ADDR_WIDTH', 64)
        QTD_WIDTH = m.Parameter('QTD_WIDTH', 32)
        DATA_WIDTH = m.Parameter('DATA_WIDTH', 512)
        CONF_ID_QUEUE_WIDTH = m.Parameter('CONF_ID_QUEUE_WIDTH', 32)
        TAG_WIDTH = m.Parameter('TAG_WIDTH', 16)
        FIFO_DEPTH_BITS = m.Parameter('FIFO_DEPTH_BITS', 4)
        FIFO_FULL = m.Parameter('FIFO_FULL', EmbeddedCode('2 ** FIFO_DEPTH_BITS'))

        clk = m.Input('clk')
        rst = m.Input('rst')
        rst_afus = m.Input('rst_afus', afu_count)
        start_afus = m.Input('start_afus', afu_count)

        conf_valid = m.Input('conf_valid', 2)
        conf = m.Input('conf', ADDR_WIDTH + QTD_WIDTH + CONF_ID_QUEUE_WIDTH)

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

        NUM_CL_DSM_RD = m.Localparam('NUM_CL_DSM_RD', int(ceil(qtd_in_queue / 16)))
        NUM_CL_DSM_WR = m.Localparam('NUM_CL_DSM_WR', int(ceil(qtd_out_queue / 16)))
        NUM_CL_DSM_TOTAL = m.Localparam('NUM_CL_DSM_TOTAL', EmbeddedCode('1 + NUM_CL_DSM_RD + NUM_CL_DSM_WR'))
        NUM_CL_DSM_TOTAL_BITS = m.Localparam('NUM_CL_DSM_TOTAL_BITS',
                                             numBits(1 + NUM_CL_DSM_RD.value + NUM_CL_DSM_WR.value))

        DSM_DATA_WIDTH = m.Localparam('DSM_DATA_WIDTH', DATA_WIDTH)
        m.EmbeddedCode('')
        req_wr_en_in = m.Wire('req_wr_en_in', qtd_out_queue)
        req_wr_data_in = m.Wire('req_wr_data_in',
                                EmbeddedCode('(DATA_WIDTH + ADDR_WIDTH + TAG_WIDTH) * %d' % qtd_out_queue))
        req_wr_available_in = m.Wire('req_wr_available_in', qtd_out_queue)
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
        output_queue_controller = make_output_queue_controller(conf_receiver, False)
        output_queue_controller_dsm = make_output_queue_controller(conf_receiver, True)
        dsm_controller_afus = make_dsm_controller()

        afu_id = 0
        ini_in_queue_id = 0
        ini_out_queue_id = 0
        for num_in_queue, num_out_queue in afus:
            afu = make_afu(afu_id, input_queue_controller, output_queue_controller, output_queue_controller_dsm,
                           dsm_controller_afus)
            range1 = '%d*(%d*(ADDR_WIDTH+TAG_WIDTH)+(ADDR_WIDTH+TAG_WIDTH))-1:%d*(%d*(ADDR_WIDTH+TAG_WIDTH))' % (
                num_in_queue, ini_in_queue_id, num_in_queue, ini_in_queue_id)
            range2 = '%d*(%d*(DATA_WIDTH+ADDR_WIDTH + TAG_WIDTH)+(DATA_WIDTH+ADDR_WIDTH + TAG_WIDTH))-1:%d*(%d*(DATA_WIDTH+ADDR_WIDTH + TAG_WIDTH))' % (
                num_out_queue, ini_out_queue_id, num_out_queue, ini_out_queue_id)
            params = [('ADDR_WIDTH', ADDR_WIDTH), ('QTD_WIDTH', QTD_WIDTH), ('DATA_WIDTH', DATA_WIDTH),
                      ('CONF_ID_QUEUE_WIDTH', CONF_ID_QUEUE_WIDTH), ('INITIAL_INPUT_QUEUE_ID', ini_in_queue_id),
                      ('INITIAL_OUTPUT_QUEUE_ID', ini_out_queue_id), ('NUM_INPUT_QUEUES', num_in_queue),
                      ('NUM_OUTPUT_QUEUES', num_out_queue), ('TAG_WIDTH', TAG_WIDTH),
                      ('FIFO_DEPTH_BITS', FIFO_DEPTH_BITS), ('FIFO_FULL', FIFO_FULL),
                      ('DSM_DATA_WIDTH', DSM_DATA_WIDTH),
                      ('NUM_CL_DSM_RD', NUM_CL_DSM_RD), ('NUM_CL_DSM_WR', NUM_CL_DSM_WR),
                      ('NUM_CL_DSM_TOTAL', NUM_CL_DSM_TOTAL), ('NUM_CL_DSM_TOTAL_BITS', NUM_CL_DSM_TOTAL_BITS)]
            con = [('clk', clk), ('rst', rst | rst_afus[afu_id]), ('start', start_afus[afu_id]),
                   ('conf_valid', conf_valid), ('conf', conf),
                   ('available_read', req_rd_available_in[ini_in_queue_id:ini_in_queue_id + num_in_queue]),
                   ('request_read', req_rd_en_in[ini_in_queue_id:ini_in_queue_id + num_in_queue]),
                   ('request_data', req_rd_data_in[EmbeddedCode(range1)]),

                   ('read_data_valid', resp_rd_valid), ('read_queue_id', resp_rd_mdata), ('read_data', resp_rd_data),
                   ('available_write', req_wr_available_in[ini_out_queue_id:ini_out_queue_id + num_out_queue]),
                   ('request_write', req_wr_en_in[ini_out_queue_id:ini_out_queue_id + num_out_queue]),
                   ('write_data', req_wr_data_in[EmbeddedCode(range2)]),
                   ('write_data_valid', resp_wr_valid),
                   ('write_queue_id', resp_wr_mdata)]
            m.Instance(afu, 'afu_%d_%dx%d' % (afu_id, num_in_queue, num_out_queue), params, con)
            afu_id = afu_id + 1
            ini_in_queue_id = ini_in_queue_id + num_in_queue
            ini_out_queue_id = ini_out_queue_id + num_out_queue

        ac_in = make_arbiter_controller_tree(RADIX, qtd_in_queue)
        params = [('DATA_WIDTH', ADDR_WIDTH + TAG_WIDTH), ('INPUT_FIFO_DEPTH_BITS', FIFO_DEPTH_BITS),
                  ('OUTPUT_FIFO_DEPTH_BITS', FIFO_DEPTH_BITS)]
        con = [('clk', clk), ('rst', rst), ('req_wr_en_in', req_rd_en_in), ('req_wr_data_in', req_rd_data_in),
               ('req_wr_available_in', req_rd_available_in), ('req_wr_available_out', req_rd_available_out),
               ('req_wr_en_out', req_rd_en_out), ('req_wr_data_out', req_rd_data_out)]
        m.Instance(ac_in, 'arbiter_controller_tree_in', params, con)

        if qtd_in_queue == qtd_out_queue:
            ac_out = ac_in
        else:
            ac_out = make_arbiter_controller_tree(RADIX, qtd_out_queue)

        params = [('DATA_WIDTH', DATA_WIDTH + ADDR_WIDTH + TAG_WIDTH), ('INPUT_FIFO_DEPTH_BITS', FIFO_DEPTH_BITS),
                  ('OUTPUT_FIFO_DEPTH_BITS', FIFO_DEPTH_BITS)]
        con = [('clk', clk), ('rst', rst), ('req_wr_en_in', req_wr_en_in), ('req_wr_data_in', req_wr_data_in),
               ('req_wr_available_in', req_wr_available_in), ('req_wr_available_out', req_wr_available_out),
               ('req_wr_en_out', req_wr_en_out), ('req_wr_data_out', req_wr_data_out)]
        m.Instance(ac_out, 'arbiter_controller_tree_out', params, con)

        req_rd_available_out.assign(req_rd_available)
        req_wr_available_out.assign(req_wr_available)

        m.Always(Posedge(clk))(
            If(rst)(
                req_rd_en(Int(0, 1, 2)),
                req_rd_addr(Repeat(Int(0, 1, 2), ADDR_WIDTH)),
                req_rd_mdata(Repeat(Int(0, 1, 2), TAG_WIDTH))
            ).Else(
                req_rd_en(req_rd_en_out),
                req_rd_addr(req_rd_data_out[TAG_WIDTH:TAG_WIDTH + ADDR_WIDTH]),
                req_rd_mdata(req_rd_data_out[0:TAG_WIDTH])
            )
        )

        m.Always(Posedge(clk))(
            If(rst)(
                req_wr_en(Int(0, 1, 2)),
                req_wr_addr(Repeat(Int(0, 1, 2), ADDR_WIDTH)),
                req_wr_mdata(Repeat(Int(0, 1, 2), TAG_WIDTH)),
                req_wr_data(Repeat(Int(0, 1, 2), DATA_WIDTH))
            ).Else(
                req_wr_en(req_wr_en_out),
                req_wr_data(req_wr_data_out[TAG_WIDTH + ADDR_WIDTH:DATA_WIDTH + TAG_WIDTH + ADDR_WIDTH]),
                req_wr_addr(req_wr_data_out[TAG_WIDTH:TAG_WIDTH + ADDR_WIDTH]),
                req_wr_mdata(req_wr_data_out[0:TAG_WIDTH])
            )
        )

        # Informações sobre o controlador de buffers para o software
        j = 0
        code = ''
        for i in afus:
            code = code + 'info[%d:%d] <= {8\'d%d, 8\'d%d};\n' % (j + 15, j, i[1], i[0])
            j = j + 16
        m.Always(Posedge(clk))(
            If(rst)(
                info(Int(0, info.width, 10))
            ).Else(
                EmbeddedCode(code)
            )
        )
        print('AFU manager successfully created!')
        print('Amount of AFUs: %d' % len(afus))
        print('Amount of Input Queue: %d' % qtd_in_queue)
        print('Amount of Output Queue: %d' % (qtd_out_queue))
        return m
    except:
        print('Failed to create the AFU manager!')
        print("Error:", sys.exc_info())
        exit(255)
        raise


'''
def afu_manager_args_process(afus):
    afu_count = 0
    qtd_in_queue = 0
    qtd_out_queue = 0
    if len(afus) < 1:
        print('At least one afu should be created!')
        print('Process aborted.')
        exit()
    for i in afus:
        if i[0] == 0 or i[1] == 0:
            print('The quantities of inputs or outputs of afu %d can not be equal to 0!' % afu_count)
            print('Process aborted.')
            exit(255)
        qtd_in_queue = qtd_in_queue + i[0]
        qtd_out_queue = qtd_out_queue + i[1]
        afu_count = afu_count + 1

    return afu_count, qtd_in_queue, qtd_out_queue


def make_afu_manager(afus):
    try:
        afu_count, qtd_in_queue, qtd_out_queue = afu_manager_args_process(afus)
        RADIX = 4
        m = Module('afu_manager')
        ADDR_WIDTH = m.Parameter('ADDR_WIDTH', 64)
        QTD_WIDTH = m.Parameter('QTD_WIDTH', 32)
        DATA_WIDTH = m.Parameter('DATA_WIDTH', 512)
        CONF_ID_QUEUE_WIDTH = m.Parameter('CONF_ID_QUEUE_WIDTH', 32)
        TAG_WIDTH = m.Parameter('TAG_WIDTH', 16)
        FIFO_DEPTH_BITS = m.Parameter('FIFO_DEPTH_BITS', 4)
        FIFO_FULL = m.Parameter('FIFO_FULL', EmbeddedCode('2 ** FIFO_DEPTH_BITS'))

        clk = m.Input('clk')
        rst = m.Input('rst')
        rst_afus = m.Input('rst_afus', afu_count)
        start_afus = m.Input('start_afus', afu_count)

        conf_valid = m.Input('conf_valid', 2)
        conf = m.Input('conf', ADDR_WIDTH + QTD_WIDTH + CONF_ID_QUEUE_WIDTH)

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

        NUM_CL_DSM_RD = m.Localparam('NUM_CL_DSM_RD', int(ceil(qtd_in_queue / 16)))
        NUM_CL_DSM_WR = m.Localparam('NUM_CL_DSM_WR', int(ceil(qtd_out_queue / 16)))
        NUM_CL_DSM_TOTAL = m.Localparam('NUM_CL_DSM_TOTAL', EmbeddedCode('1 + NUM_CL_DSM_RD + NUM_CL_DSM_WR'))
        NUM_CL_DSM_TOTAL_BITS = m.Localparam('NUM_CL_DSM_TOTAL_BITS',
                                             numBits(1 + NUM_CL_DSM_RD.value + NUM_CL_DSM_WR.value))

        DSM_DATA_WIDTH = m.Localparam('DSM_DATA_WIDTH', DATA_WIDTH)
        FSM_REQ_RD_IDLE = m.Localparam('FSM_REQ_RD_IDLE', 0)
        FSM_REQ_RD_WAIT = m.Localparam('FSM_REQ_RD_WAIT', 1)

        FSM_REQ_WR_IDLE = m.Localparam('FSM_REQ_WR_IDLE', 0)
        FSM_REQ_WR_WAIT = m.Localparam('FSM_REQ_WR_WAIT', 1)
        m.EmbeddedCode('\n')
        rd_req_arbiter_request = m.Reg('rd_req_arbiter_request', qtd_in_queue)
        rd_req_arbiter_grant = m.Wire('rd_req_arbiter_grant', qtd_in_queue)
        rd_req_arbiter_grant_valid = m.Wire('rd_req_arbiter_grant_valid')
        rd_req_arbiter_grant_encoded = m.Wire('rd_req_arbiter_grant_encoded', numBits(qtd_in_queue))
        m.EmbeddedCode('\n')
        wr_req_arbiter_request = m.Reg('wr_req_arbiter_request', qtd_out_queue)
        wr_req_arbiter_grant = m.Wire('wr_req_arbiter_grant', qtd_out_queue)
        wr_req_arbiter_grant_valid = m.Wire('wr_req_arbiter_grant_valid')
        wr_req_arbiter_grant_encoded = m.Wire('wr_req_arbiter_grant_encoded', numBits(qtd_out_queue))
        m.EmbeddedCode('\n')
        idx_req_rd_fifo = m.Genvar('idx_req_rd_fifo')
        req_rd_fifo_re = m.Reg('req_rd_fifo_re', qtd_in_queue)
        req_rd_fifo_valid = m.Wire('req_rd_fifo_valid', qtd_in_queue)
        req_rd_fifo_empty = m.Wire('req_rd_fifo_empty', qtd_in_queue)
        req_rd_fifo_amostempty = m.Wire('req_rd_fifo_amostempty', qtd_in_queue)
        req_rd_fifo_dout = m.Wire('req_rd_fifo_dout', (ADDR_WIDTH + TAG_WIDTH) * qtd_in_queue)
        afu_req_rd_fifo_we = m.Wire('afu_req_rd_fifo_we', qtd_in_queue)
        afu_req_rd_fifo_din = m.Wire('afu_req_rd_fifo_din', (ADDR_WIDTH + TAG_WIDTH) * qtd_in_queue)
        afu_req_rd_fifo_full = m.Wire('afu_req_rd_fifo_full', qtd_in_queue)
        afu_req_rd_fifo_almostfull = m.Wire('afu_req_rd_fifo_almostfull', qtd_in_queue)
        count_width_in = (FIFO_DEPTH_BITS + 1) * qtd_in_queue
        req_rd_fifo_count = m.Wire('req_rd_fifo_count', count_width_in)
        m.EmbeddedCode('\n')
        idx_req_wr_fifo = m.Genvar('idx_req_wr_fifo')
        req_wr_fifo_re = m.Reg('req_wr_fifo_re', qtd_out_queue)
        req_wr_fifo_valid = m.Wire('req_wr_fifo_valid', qtd_out_queue)
        req_wr_fifo_empty = m.Wire('req_wr_fifo_empty', qtd_out_queue)
        req_wr_fifo_amostempty = m.Wire('req_wr_fifo_amostempty', qtd_out_queue)
        req_wr_fifo_dout = m.Wire('req_wr_fifo_dout', (ADDR_WIDTH + DATA_WIDTH + TAG_WIDTH) * qtd_out_queue)
        afu_req_wr_fifo_we = m.Wire('afu_req_wr_fifo_we', qtd_out_queue)
        afu_req_wr_fifo_din = m.Wire('afu_req_wr_fifo_din', (ADDR_WIDTH + DATA_WIDTH + TAG_WIDTH) * qtd_out_queue)
        afu_req_wr_fifo_full = m.Wire('afu_req_wr_fifo_full', qtd_out_queue)
        afu_req_wr_fifo_almostfull = m.Wire('afu_req_wr_fifo_almostfull', qtd_out_queue)
        count_width_out = (FIFO_DEPTH_BITS + 1) * qtd_out_queue
        req_wr_fifo_count = m.Wire('req_wr_fifo_count', count_width_out)
        m.EmbeddedCode('\n')
        req_rd_fifo_sel_dout = m.Wire('req_rd_fifo_sel_dout', (ADDR_WIDTH + TAG_WIDTH))
        req_rd_fifo_sel_dout_valid = m.Wire('req_rd_fifo_sel_dout_valid')
        req_wr_fifo_sel_dout = m.Wire('req_wr_fifo_sel_dout', (DATA_WIDTH + ADDR_WIDTH + TAG_WIDTH))
        req_wr_fifo_sel_dout_valid = m.Wire('req_wr_fifo_sel_dout_valid')
        m.EmbeddedCode('\n')

        fsm_req_rd = m.Reg('fsm_req_rd', 1)

        fsm_req_wr = m.Reg('fsm_req_wr', 1)
        arbiter = make_arbiter()
        params = [('PORTS', qtd_in_queue), ('TYPE', "ROUND_ROBIN"), ('BLOCK', "NONE"), ('LSB_PRIORITY', "LOW")]
        con = [('clk', clk), ('rst', rst), ('request', rd_req_arbiter_request),
               ('acknowledge', Int(0, qtd_in_queue, 10)),
               ('grant', rd_req_arbiter_grant), ('grant_valid', rd_req_arbiter_grant_valid),
               ('grant_encoded', rd_req_arbiter_grant_encoded)]
        m.Instance(arbiter, 'rd_req_arbiter', params, con)

        params = [('PORTS', qtd_out_queue), ('TYPE', "ROUND_ROBIN"), ('BLOCK', "NONE"), ('LSB_PRIORITY', "LOW")]
        con = [('clk', clk), ('rst', rst), ('request', wr_req_arbiter_request),
               ('acknowledge', Int(0, qtd_out_queue, 10)),
               ('grant', wr_req_arbiter_grant), ('grant_valid', wr_req_arbiter_grant_valid),
               ('grant_encoded', wr_req_arbiter_grant_encoded)]
        m.Instance(arbiter, 'wr_req_arbiter', params, con)

        fifo = make_fifo()
        params = [('FIFO_WIDTH', ADDR_WIDTH + TAG_WIDTH), ('FIFO_DEPTH_BITS', FIFO_DEPTH_BITS),
                  ('FIFO_ALMOSTFULL_THRESHOLD', FIFO_FULL - 4), ('FIFO_ALMOSTEMPTY_THRESHOLD', 2)]
        con = [('clk', clk), ('rst', rst), ('we', afu_req_rd_fifo_we[idx_req_rd_fifo]),
               ('din', afu_req_rd_fifo_din[
                       idx_req_rd_fifo * (ADDR_WIDTH + TAG_WIDTH):idx_req_rd_fifo * (ADDR_WIDTH + TAG_WIDTH) + (
                               ADDR_WIDTH + TAG_WIDTH)]),
               ('re', req_rd_fifo_re[idx_req_rd_fifo]), ('valid', req_rd_fifo_valid[idx_req_rd_fifo]),
               ('dout', req_rd_fifo_dout[
                        idx_req_rd_fifo * (ADDR_WIDTH + TAG_WIDTH):idx_req_rd_fifo * (ADDR_WIDTH + TAG_WIDTH) + (
                                ADDR_WIDTH + TAG_WIDTH)]),
               ('count',
                req_rd_fifo_count[idx_req_rd_fifo * (FIFO_DEPTH_BITS + 1):idx_req_rd_fifo * (FIFO_DEPTH_BITS + 1) + (
                        FIFO_DEPTH_BITS + 1)]),
               ('empty', req_rd_fifo_empty[idx_req_rd_fifo]), ('full', afu_req_rd_fifo_full[idx_req_rd_fifo]),
               ('almostfull', afu_req_rd_fifo_almostfull[idx_req_rd_fifo]),
               ('almostempty', req_rd_fifo_amostempty[idx_req_rd_fifo])]
        genReqRdFifos = m.GenerateFor(idx_req_rd_fifo(0), idx_req_rd_fifo < qtd_in_queue, idx_req_rd_fifo.inc(),
                                      'gen_req_rd_fifos')
        genReqRdFifos.Instance(fifo, 'req_rd_fifo', params, con)

        params = [('FIFO_WIDTH', ADDR_WIDTH + DATA_WIDTH + TAG_WIDTH), ('FIFO_DEPTH_BITS', FIFO_DEPTH_BITS),
                  ('FIFO_ALMOSTFULL_THRESHOLD', FIFO_FULL - 4), ('FIFO_ALMOSTEMPTY_THRESHOLD', 2)]
        con = [('clk', clk), ('rst', rst), ('we', afu_req_wr_fifo_we[idx_req_wr_fifo]),
               ('din', afu_req_wr_fifo_din[
                       idx_req_wr_fifo * (ADDR_WIDTH + DATA_WIDTH + TAG_WIDTH):idx_req_wr_fifo * (
                               ADDR_WIDTH + DATA_WIDTH + TAG_WIDTH) + (ADDR_WIDTH + DATA_WIDTH + TAG_WIDTH)]),
               ('re', req_wr_fifo_re[idx_req_wr_fifo]), ('valid', req_wr_fifo_valid[idx_req_wr_fifo]),
               ('dout', req_wr_fifo_dout[
                        idx_req_wr_fifo * (ADDR_WIDTH + DATA_WIDTH + TAG_WIDTH):idx_req_wr_fifo * (
                                ADDR_WIDTH + DATA_WIDTH + TAG_WIDTH) + (ADDR_WIDTH + DATA_WIDTH + TAG_WIDTH)]),
               ('count', req_wr_fifo_count[
                         idx_req_wr_fifo * (FIFO_DEPTH_BITS + 1):idx_req_wr_fifo * (FIFO_DEPTH_BITS + 1) + (
                                 FIFO_DEPTH_BITS + 1)]),
               ('empty', req_wr_fifo_empty[idx_req_wr_fifo]), ('full', afu_req_wr_fifo_full[idx_req_wr_fifo]),
               ('almostfull', afu_req_wr_fifo_almostfull[idx_req_wr_fifo]),
               ('almostempty', req_wr_fifo_amostempty[idx_req_wr_fifo])]
        genReqWrFifos = m.GenerateFor(idx_req_wr_fifo(0), idx_req_wr_fifo < qtd_out_queue, idx_req_wr_fifo.inc(),
                                      'gen_req_wr_fifos')
        genReqWrFifos.Instance(fifo, 'req_wr_fifo', params, con)

        conf_receiver = make_conf_receiver()
        input_queue_controller = make_input_queue_controller(conf_receiver)
        output_queue_controller = make_output_queue_controller(conf_receiver,False)
        output_queue_controller_dsm = make_output_queue_controller(conf_receiver,True)
        dsm_controller_afus = make_dsm_controller()

        afu_id = 0
        ini_in_queue_id = 0
        ini_out_queue_id = 0
        for num_in_queue, num_out_queue in afus:
            afu = make_afu(afu_id, input_queue_controller, output_queue_controller,output_queue_controller_dsm,dsm_controller_afus)
            range1 = '%d*(%d*(ADDR_WIDTH+TAG_WIDTH)+(ADDR_WIDTH+TAG_WIDTH))-1:%d*(%d*(ADDR_WIDTH+TAG_WIDTH))' % (
                num_in_queue, ini_in_queue_id, num_in_queue, ini_in_queue_id)
            range2 = '%d*(%d*(DATA_WIDTH+ADDR_WIDTH + TAG_WIDTH)+(DATA_WIDTH+ADDR_WIDTH + TAG_WIDTH))-1:%d*(%d*(DATA_WIDTH+ADDR_WIDTH + TAG_WIDTH))' % (
                num_out_queue, ini_out_queue_id, num_out_queue, ini_out_queue_id)
            params = [('ADDR_WIDTH', ADDR_WIDTH), ('QTD_WIDTH', QTD_WIDTH), ('DATA_WIDTH', DATA_WIDTH),
                      ('CONF_ID_QUEUE_WIDTH', CONF_ID_QUEUE_WIDTH), ('INITIAL_INPUT_QUEUE_ID', ini_in_queue_id),
                      ('INITIAL_OUTPUT_QUEUE_ID', ini_out_queue_id), ('NUM_INPUT_QUEUES', num_in_queue),
                      ('NUM_OUTPUT_QUEUES', num_out_queue), ('TAG_WIDTH', TAG_WIDTH),
                      ('FIFO_DEPTH_BITS', FIFO_DEPTH_BITS), ('FIFO_FULL', FIFO_FULL),
                      ('DSM_DATA_WIDTH', DSM_DATA_WIDTH),
                      ('NUM_CL_DSM_RD', NUM_CL_DSM_RD), ('NUM_CL_DSM_WR', NUM_CL_DSM_WR),
                      ('NUM_CL_DSM_TOTAL', NUM_CL_DSM_TOTAL), ('NUM_CL_DSM_TOTAL_BITS', NUM_CL_DSM_TOTAL_BITS)]
            con = [('clk', clk), ('rst', rst | rst_afus[afu_id]), ('start', start_afus[afu_id]),
                   ('conf_valid', conf_valid), ('conf', conf),
                   ('available_read', ~afu_req_rd_fifo_almostfull[ini_in_queue_id:ini_in_queue_id + num_in_queue]),
                   ('request_read', afu_req_rd_fifo_we[ini_in_queue_id:ini_in_queue_id + num_in_queue]),
                   ('request_data', afu_req_rd_fifo_din[EmbeddedCode(range1)]),
                   ('read_data_valid', resp_rd_valid), ('read_queue_id', resp_rd_mdata), ('read_data', resp_rd_data),
                   ('available_write', ~afu_req_wr_fifo_almostfull[ini_out_queue_id:ini_out_queue_id + num_out_queue]),
                   ('request_write', afu_req_wr_fifo_we[ini_out_queue_id:ini_out_queue_id + num_out_queue]),
                   ('write_data', afu_req_wr_fifo_din[EmbeddedCode(range2)]),
                   ('write_data_valid', resp_wr_valid),
                   ('write_queue_id', resp_wr_mdata)]
            m.Instance(afu, 'afu_%d_%dx%d' % (afu_id, num_in_queue, num_out_queue), params, con)
            afu_id = afu_id + 1
            ini_in_queue_id = ini_in_queue_id + num_in_queue
            ini_out_queue_id = ini_out_queue_id + num_out_queue

        req_rd_fifo_sel = make_select_tree(RADIX, qtd_in_queue)
        params = [('DATA_WIDTH', ADDR_WIDTH + TAG_WIDTH)]
        con = [('clk', clk), ('rst', rst), ('data_in_valid', req_rd_fifo_valid)]
        for i in range(qtd_in_queue):
            ran = '((%d*(ADDR_WIDTH+TAG_WIDTH)+ADDR_WIDTH+TAG_WIDTH)-1:%d*(ADDR_WIDTH+TAG_WIDTH))' % (i, i)
            con.append(('data_in_%d' % i, req_rd_fifo_dout[EmbeddedCode(ran)]))
        con.append(('data_out_valid', req_rd_fifo_sel_dout_valid))
        con.append(('data_out', req_rd_fifo_sel_dout))
        m.Instance(req_rd_fifo_sel, 'req_rd_fifo_sel', params, con)

        req_wr_fifo_sel = req_rd_fifo_sel
        if qtd_in_queue != qtd_out_queue:
            req_wr_fifo_sel = make_select_tree(RADIX, qtd_out_queue)
        params = [('DATA_WIDTH', DATA_WIDTH + ADDR_WIDTH + TAG_WIDTH)]
        con = [('clk', clk), ('rst', rst), ('data_in_valid', req_wr_fifo_valid)]
        for i in range(qtd_out_queue):
            ran = '((%d*(DATA_WIDTH+ADDR_WIDTH+TAG_WIDTH)+DATA_WIDTH+ADDR_WIDTH+TAG_WIDTH)-1:%d*(DATA_WIDTH+ADDR_WIDTH+TAG_WIDTH))' % (
                i, i)
            con.append(('data_in_%d' % i, req_wr_fifo_dout[EmbeddedCode(ran)]))
        con.append(('data_out_valid', req_wr_fifo_sel_dout_valid))
        con.append(('data_out', req_wr_fifo_sel_dout))
        m.Instance(req_wr_fifo_sel, 'req_wr_fifo_sel', params, con)

        m.Always(Posedge(clk))(
            If(rst)(
                fsm_req_rd(0),
                rd_req_arbiter_request(0),
                req_rd_fifo_re(0)
            ).Else(
                rd_req_arbiter_request(0),
                req_rd_fifo_re(0),
                Case(fsm_req_rd)(
                    When(FSM_REQ_RD_IDLE)(
                        If(req_rd_available)(
                            rd_req_arbiter_request(~req_rd_fifo_empty)
                        ),
                        If(rd_req_arbiter_grant_valid)(
                            req_rd_fifo_re(rd_req_arbiter_grant),
                            fsm_req_rd(FSM_REQ_RD_WAIT)
                        )
                    ),
                    When(FSM_REQ_RD_WAIT)(
                        If(EmbeddedCode('|req_rd_fifo_valid'))(
                            fsm_req_rd(FSM_REQ_RD_IDLE)
                        )
                    )
                )
            )

        )

        m.Always(Posedge(clk))(
            If(rst)(
                fsm_req_wr(0),
                wr_req_arbiter_request(0),
                req_wr_fifo_re(0)
            ).Else(
                wr_req_arbiter_request(0),
                req_wr_fifo_re(0),
                Case(fsm_req_wr)(
                    When(FSM_REQ_WR_IDLE)(
                        If(req_wr_available)(
                            wr_req_arbiter_request(~req_wr_fifo_empty)
                        ),
                        If(wr_req_arbiter_grant_valid)(
                            req_wr_fifo_re(wr_req_arbiter_grant),
                            fsm_req_wr(FSM_REQ_WR_WAIT)
                        )
                    ),
                    When(FSM_REQ_WR_WAIT)(
                        If(EmbeddedCode('|req_wr_fifo_valid'))(
                            fsm_req_wr(FSM_REQ_WR_IDLE)
                        )
                    )
                )
            )

        )

        m.Always(Posedge(clk))(
            If(rst)(
                req_rd_en(Int(0, 1, 2)),
                req_rd_addr(0),
                req_rd_mdata(0)
            ).Else(
                req_rd_en(Int(0, 1, 2)),
                If(req_rd_fifo_sel_dout_valid)(
                    req_rd_en(Int(1, 1, 2)),
                    req_rd_mdata(req_rd_fifo_sel_dout[0:TAG_WIDTH]),
                    req_rd_addr(req_rd_fifo_sel_dout[TAG_WIDTH:ADDR_WIDTH + TAG_WIDTH])
                )
            )
        )

        m.Always(Posedge(clk))(
            If(rst)(
                req_wr_en(Int(0, 1, 2)),
                req_wr_addr(0),
                req_wr_mdata(0),
                req_wr_data(0)
            ).Else(
                req_wr_en(Int(0, 1, 2)),
                If(req_wr_fifo_sel_dout_valid)(
                    req_wr_en(Int(1, 1, 2)),
                    req_wr_mdata(req_wr_fifo_sel_dout[0:TAG_WIDTH]),
                    req_wr_addr(req_wr_fifo_sel_dout[TAG_WIDTH:ADDR_WIDTH + TAG_WIDTH]),
                    req_wr_data(req_wr_fifo_sel_dout[ADDR_WIDTH + TAG_WIDTH:DATA_WIDTH + ADDR_WIDTH + TAG_WIDTH])
                )
            )
        )

        # Informações sobre o controlador de buffers para o software
        j = 0
        code = ''
        for i in afus:
            code = code + 'info[%d:%d] <= {8\'d%d, 8\'d%d};\n' % (j + 15, j, i[0], i[1])
            j = j + 16
        m.Always(Posedge(clk))(
            If(rst)(
                info(Int(0, info.width, 10))
            ).Else(
                EmbeddedCode(code)
            )
        )
        print('AFU manager successfully created!')
        print('Amount of AFUs: %d' % len(afus))
        print('Amount of Input Queue: %d' % qtd_in_queue)
        print('Amount of Output Queue: %d' % (qtd_out_queue))
        return m
    except:
        print('Failed to create the AFU manager!')
        print("Error:", sys.exc_info())
        exit(255)
        raise
'''
