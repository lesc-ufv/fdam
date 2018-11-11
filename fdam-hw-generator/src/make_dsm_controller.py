from veriloggen import *

from make_counter import make_counter


def make_dsm_controller(conf_receiver):
    m = Module('fdam_dsm_controller')
    ACC_ID = m.Parameter('ACC_ID', 1)
    ADDR_WIDTH = m.Parameter('ADDR_WIDTH', 48)
    QTD_WIDTH = m.Parameter('QTD_WIDTH', 32)
    TAG_WIDTH = m.Parameter('TAG_WIDTH', 16)
    CONF_ID_QUEUE_WIDTH = m.Parameter('CONF_ID_QUEUE_WIDTH', 32)
    DATA_WIDTH = m.Parameter('DATA_WIDTH', 512)
    NUM_INPUT_QUEUES = m.Parameter('NUM_INPUT_QUEUES', 1)
    NUM_OUTPUT_QUEUES = m.Parameter('NUM_OUTPUT_QUEUES', 1)

    clk = m.Input('clk')
    rst = m.Input('rst')
    start = m.Input('start')

    conf_valid = m.Input('conf_valid', 2)
    conf = m.Input('conf', EmbeddedCode('ADDR_WIDTH + QTD_WIDTH + CONF_ID_QUEUE_WIDTH'))

    done_rd = m.Input('done_rd', NUM_INPUT_QUEUES)
    done_wr = m.Input('done_wr', NUM_OUTPUT_QUEUES)
    done_acc = m.Input('done_acc')

    acc_req_rd_data_en = m.Input('acc_req_rd_data_en', NUM_INPUT_QUEUES)
    acc_req_wr_data_en = m.Input('acc_req_wr_data_en', NUM_OUTPUT_QUEUES)

    write_data_valid = m.Input('write_data_valid')
    write_queue_id = m.Input('write_queue_id', TAG_WIDTH)

    available_write = m.Input('available_write')
    request_write = m.OutputReg('request_write')
    write_data = m.OutputReg('write_data', DATA_WIDTH + ADDR_WIDTH + TAG_WIDTH)

    NUM_CL_DSM_RD = m.Localparam('NUM_CL_DSM_RD', EmbeddedCode('$rtoi($ceil(NUM_INPUT_QUEUES/16.0))'))
    NUM_CL_DSM_WR = m.Localparam('NUM_CL_DSM_WR', EmbeddedCode('$rtoi($ceil(NUM_OUTPUT_QUEUES/16.0))'))
    NUM_CL_DSM_TOTAL = m.Localparam('NUM_CL_DSM_TOTAL', EmbeddedCode('NUM_CL_DSM_RD + NUM_CL_DSM_WR'))
    code = '$rtoi($ceil(NUM_CL_DSM_TOTAL/4.0)*4.0)'
    NUM_CL_DSM_TOTAL_ALIGN = m.Localparam('NUM_CL_DSM_TOTAL_ALIGN', EmbeddedCode(code))
    NUM_CL_DSM_TOTAL_BITS = m.Localparam('NUM_CL_DSM_TOTAL_BITS',
                                         EmbeddedCode('$rtoi($ceil($clog2(NUM_CL_DSM_TOTAL_ALIGN)))'))
    CONF_TYPE_OUT_DSM = m.Localparam('CONF_TYPE_OUT_DSM', 3)

    FSM_DSM_UPDATE_IDLE = m.Localparam('FSM_DSM_UPDATE_IDLE', 0)
    FSM_DSM_UPDATE_DELAY = m.Localparam('FSM_DSM_UPDATE_DELAY', 1)
    FSM_DSM_UPDATE_WR1 = m.Localparam('FSM_DSM_UPDATE_WR1', 2)
    FSM_DSM_UPDATE_WR2 = m.Localparam('FSM_DSM_UPDATE_WR2', 3)
    FSM_DSM_UPDATE_WAIT_RESP = m.Localparam('FSM_DSM_UPDATE_WAIT_RESP', 4)

    count_dsm_delay = m.Reg('count_dsm_delay', 10)
    dsm_data = m.Reg('dsm_data', DATA_WIDTH, NUM_CL_DSM_TOTAL)
    done = m.Reg('done', DATA_WIDTH)
    done_last = m.Reg('done_last', DATA_WIDTH)
    fsm_update_dsm = m.Reg('fsm_update_dsm', 3)
    acc_req_wr_count = m.Reg('acc_req_wr_count', NUM_CL_DSM_TOTAL_BITS + 1)
    update_dsm = m.Reg('update_dsm')
    dsm_count_cl = m.Reg('dsm_count_cl', NUM_CL_DSM_TOTAL_BITS + 1)
    dsm_addr_base = m.Reg('dsm_addr_base', ADDR_WIDTH)
    dsm_qtd_data_cl = m.Reg('dsm_qtd_data_cl', QTD_WIDTH)
    dsm_conf_ready = m.Reg('dsm_conf_ready')
    dsm_addr_write_next = m.Reg('dsm_addr_write_next', ADDR_WIDTH)

    reset_dsm_count_cl = m.Reg('reset_dsm_count_cl')
    dsm_write_data_valid_queue = m.Wire('dsm_write_data_valid_queue')

    dsm_conf_rd_valid = m.Wire('dsm_conf_rd_valid')
    dsm_rst_internal = m.Wire('dsm_rst_internal')
    dsm_conf_rd = m.Wire('conf_rd', EmbeddedCode(ADDR_WIDTH + QTD_WIDTH + CONF_ID_QUEUE_WIDTH))

    acc_in_rd_count = m.Wire('acc_in_rd_count', QTD_WIDTH, EmbeddedCode('(NUM_CL_DSM_RD*16)'))
    acc_in_wr_count = m.Wire('acc_in_wr_count', QTD_WIDTH, EmbeddedCode('(NUM_CL_DSM_WR*16)'))
    dsm_data_wire = m.Wire('dsm_data_wire', DATA_WIDTH, NUM_CL_DSM_TOTAL)
    idx = m.Integer('idx')
    i = m.Genvar('i')

    counters = make_counter()
    genFor = m.GenerateFor(i(0), i < NUM_INPUT_QUEUES, i.inc(), 'gen_counter_rd')
    params = [('DEPTH_BITS', QTD_WIDTH)]
    con = [('clk', clk), ('rst', dsm_rst_internal), ('en', acc_req_rd_data_en[i]),
           ('dout', acc_in_rd_count[i])]
    genFor.Instance(counters, 'counter_rd', params, con)

    genFor = m.GenerateFor(i(0), i < NUM_OUTPUT_QUEUES, i.inc(), 'gen_counter_wr')
    params = [('DEPTH_BITS', QTD_WIDTH)]
    con = [('clk', clk), ('rst', dsm_rst_internal), ('en', acc_req_wr_data_en[i]),
           ('dout', acc_in_wr_count[i])]
    genFor.Instance(counters, 'counter_wr', params, con)

    params = [('CONF_TYPE', CONF_TYPE_OUT_DSM), ('CONF_ID', ACC_ID), ('CONF_ID_WIDTH', CONF_ID_QUEUE_WIDTH),
              ('CONF_WIDTH', EmbeddedCode('ADDR_WIDTH + QTD_WIDTH + CONF_ID_QUEUE_WIDTH'))]
    con = [('clk', clk), ('rst', rst), ('conf_in_valid', conf_valid), ('conf_in_data', conf),
           ('conf_out_valid', dsm_conf_rd_valid),
           ('conf_out_data', dsm_conf_rd), ('conf_reset_out', dsm_rst_internal)]
    m.Instance(conf_receiver, 'dsm_conf_receiver', params, con)

    dsm_write_data_valid_queue.assign(
        AndList(write_data_valid, (write_queue_id == Cat(Int(1, 1, 2), ACC_ID[0:TAG_WIDTH - 1]))))

    genFor = m.GenerateFor(i(EmbeddedCode('(NUM_CL_DSM_RD*16)-1')), i >= NUM_INPUT_QUEUES, i.dec(), 'gen_zero_rd_count')
    genFor.Assign(acc_in_rd_count[i](Repeat(Int(0, 1, 2), QTD_WIDTH)))
    genFor = m.GenerateFor(i(EmbeddedCode('(NUM_CL_DSM_WR*16)-1')), i >= NUM_OUTPUT_QUEUES, i.dec(),
                           'gen_zero_wr_count')
    genFor.Assign(acc_in_wr_count[i](Repeat(Int(0, 1, 2), QTD_WIDTH)))
    genFor = m.GenerateFor(i(0), i < NUM_CL_DSM_RD, i.inc(), 'gen_dsm_data_rd')
    genFor.Assign(dsm_data_wire[i](
        Cat(acc_in_rd_count[i * 16 + 15], acc_in_rd_count[i * 16 + 14],
            acc_in_rd_count[i * 16 + 13],
            acc_in_rd_count[i * 16 + 12], acc_in_rd_count[i * 16 + 11],
            acc_in_rd_count[i * 16 + 10],
            acc_in_rd_count[i * 16 + 9], acc_in_rd_count[i * 16 + 8],
            acc_in_rd_count[i * 16 + 7],
            acc_in_rd_count[i * 16 + 6], acc_in_rd_count[i * 16 + 5],
            acc_in_rd_count[i * 16 + 4],
            acc_in_rd_count[i * 16 + 3], acc_in_rd_count[i * 16 + 2],
            acc_in_rd_count[i * 16 + 1],
            acc_in_rd_count[i * 16 + 0])))
    genFor = m.GenerateFor(i(0), i < NUM_CL_DSM_WR, i.inc(), 'gen_dsm_data_wr')
    genFor.Assign(dsm_data_wire[i + NUM_CL_DSM_WR](
        Cat(acc_in_wr_count[i * 16 + 15], acc_in_wr_count[i * 16 + 14],
            acc_in_wr_count[i * 16 + 13],
            acc_in_wr_count[i * 16 + 12], acc_in_wr_count[i * 16 + 11],
            acc_in_wr_count[i * 16 + 10],
            acc_in_wr_count[i * 16 + 9], acc_in_wr_count[i * 16 + 8],
            acc_in_wr_count[i * 16 + 7],
            acc_in_wr_count[i * 16 + 6], acc_in_wr_count[i * 16 + 5],
            acc_in_wr_count[i * 16 + 4],
            acc_in_wr_count[i * 16 + 3], acc_in_wr_count[i * 16 + 2],
            acc_in_wr_count[i * 16 + 1],
            acc_in_wr_count[i * 16 + 0])))

    m.Always(Posedge(clk))(
        If(dsm_rst_internal)(
            dsm_addr_base(0),
            dsm_qtd_data_cl(0),
            dsm_conf_ready(Int(0, 1, 2)),
        ).Else(
            If(dsm_conf_rd_valid)(
                dsm_qtd_data_cl(dsm_conf_rd[CONF_ID_QUEUE_WIDTH:CONF_ID_QUEUE_WIDTH + QTD_WIDTH]),
                dsm_addr_base(
                    dsm_conf_rd[CONF_ID_QUEUE_WIDTH + QTD_WIDTH:CONF_ID_QUEUE_WIDTH + QTD_WIDTH + ADDR_WIDTH]),
                dsm_conf_ready(Int(1, 1, 2))
            )
        )
    )

    m.Always(Posedge(clk))(
        If(dsm_rst_internal)(
            dsm_count_cl(0),
        ).Else(
            If(reset_dsm_count_cl)(
                dsm_count_cl(0),
            ).Else(
                If(dsm_write_data_valid_queue)(
                    dsm_count_cl(dsm_count_cl + 4)
                )
            )
        )
    )

    m.Always(Posedge(clk))(
        If(dsm_rst_internal)(
            update_dsm(Int(0, 1, 2))
        ).Else(
            done(
                Cat(done_acc, Repeat(Int(0, 1, 2), DATA_WIDTH - (NUM_INPUT_QUEUES + NUM_OUTPUT_QUEUES + 1)), done_wr,
                    done_rd)),
            update_dsm(Uor((done ^ done_last) & done))
        )
    )
    m.Always(Posedge(clk))(
        For(idx(0), idx < NUM_CL_DSM_TOTAL, idx.inc())(
            dsm_data[idx](dsm_data_wire[idx])
        ),
    )

    m.Always(Posedge(clk))(
        If(dsm_rst_internal)(
            request_write(Int(0, 1, 2)),
            fsm_update_dsm(FSM_DSM_UPDATE_IDLE),
            done_last(Int(0)),
            acc_req_wr_count(0),
            dsm_addr_write_next(0),
            reset_dsm_count_cl(Int(0, 1, 2)),
            count_dsm_delay(Int(0, count_dsm_delay.width, 10))
        ).Elif(start)(
            request_write(Int(0, 1, 2)),
            reset_dsm_count_cl(Int(0, 1, 2)),
            Case(fsm_update_dsm)(
                When(FSM_DSM_UPDATE_IDLE)(
                    If(update_dsm & dsm_conf_ready)(
                        acc_req_wr_count(0),
                        dsm_addr_write_next(dsm_addr_base),
                        fsm_update_dsm(FSM_DSM_UPDATE_DELAY)
                    )
                ),
                When(FSM_DSM_UPDATE_DELAY)(
                    If(count_dsm_delay[count_dsm_delay.width - 1])(
                        done_last(done),
                        count_dsm_delay[count_dsm_delay.width - 1](Int(0, 1, 2)),
                        reset_dsm_count_cl(Int(1, 1, 2)),
                        fsm_update_dsm(FSM_DSM_UPDATE_WR1)
                    ).Else(
                        count_dsm_delay(count_dsm_delay + Int(1, count_dsm_delay.width, 10))
                    )
                ),
                When(FSM_DSM_UPDATE_WR1)(
                    If(available_write)(
                        request_write(Int(1, 1, 2)),
                        write_data(Cat(done_acc, dsm_data[acc_req_wr_count][EmbeddedCode('DATA_WIDTH-2:0')],
                                       dsm_addr_write_next,
                                       Cat(Int(1, 1, 2), ACC_ID[0:TAG_WIDTH - 1]))),
                        acc_req_wr_count(acc_req_wr_count + 1),
                        dsm_addr_write_next(dsm_addr_write_next + 1)
                    ),
                    If(acc_req_wr_count == NUM_CL_DSM_TOTAL - 1)(
                        fsm_update_dsm(FSM_DSM_UPDATE_WR2),
                    )
                ),
                When(FSM_DSM_UPDATE_WR2)(
                    write_data(Cat(done_last, dsm_addr_write_next, Cat(Int(1, 1, 2), ACC_ID[0:TAG_WIDTH - 1]))),
                    If(available_write)(
                        request_write(Int(1, 1, 2)),
                        acc_req_wr_count(acc_req_wr_count + 1),
                        dsm_addr_write_next(dsm_addr_write_next + 1)
                    ),
                    If(acc_req_wr_count == NUM_CL_DSM_TOTAL_ALIGN - 1)(
                        fsm_update_dsm(FSM_DSM_UPDATE_WAIT_RESP),
                    ),
                ),
                When(FSM_DSM_UPDATE_WAIT_RESP)(
                    If(dsm_count_cl == NUM_CL_DSM_TOTAL_ALIGN)(
                        fsm_update_dsm(FSM_DSM_UPDATE_IDLE),
                    )
                )
            )
        )
    )

    return m

# make_dsm_controller().to_verilog('dsm_controller_test')
