from veriloggen import *

from make_counter import make_counter


def make_dsm_controller():
    m = Module('dsm_controller')
    QTD_WIDTH = m.Parameter('QTD_WIDTH', 32)
    DSM_DATA_WIDTH = m.Parameter('DSM_DATA_WIDTH', 512)
    NUM_INPUT_QUEUES = m.Parameter('NUM_INPUT_QUEUES', 1)
    NUM_OUTPUT_QUEUES = m.Parameter('NUM_OUTPUT_QUEUES', 1)

    clk = m.Input('clk')
    rst = m.Input('rst')
    start = m.Input('start')
    done_rd = m.Input('done_rd', NUM_INPUT_QUEUES)
    done_wr = m.Input('done_wr', NUM_OUTPUT_QUEUES)
    done_acc = m.Input('done_acc')
    has_pending_wr = m.Input('has_pending_wr')

    acc_req_rd_data_en = m.Input('acc_req_rd_data_en', NUM_INPUT_QUEUES)
    acc_req_wr_data_en = m.Input('acc_req_wr_data_en', NUM_OUTPUT_QUEUES)

    acc_dsm_available_write = m.Input('acc_dsm_available_write')
    acc_dsm_request_write = m.OutputReg('acc_dsm_request_write')
    acc_dsm_write_data = m.OutputReg('acc_dsm_write_data', DSM_DATA_WIDTH)

    NUM_CL_DSM_RD = m.Localparam('NUM_CL_DSM_RD', EmbeddedCode('$rtoi($ceil(NUM_INPUT_QUEUES/16.0))'))
    NUM_CL_DSM_WR = m.Localparam('NUM_CL_DSM_WR', EmbeddedCode('$rtoi($ceil(NUM_OUTPUT_QUEUES/16.0))'))
    NUM_CL_DSM_TOTAL = m.Localparam('NUM_CL_DSM_TOTAL', EmbeddedCode('NUM_CL_DSM_RD + NUM_CL_DSM_WR'))
    NUM_CL_DSM_TOTAL_BITS = m.Localparam('NUM_CL_DSM_TOTAL_BITS', EmbeddedCode('$rtoi($ceil($clog2(NUM_CL_DSM_TOTAL)))'))

    dsm_data = m.Reg('dsm_data', DSM_DATA_WIDTH, NUM_CL_DSM_TOTAL)
    done = m.Reg('done', DSM_DATA_WIDTH)
    done_last = m.Reg('done_last', DSM_DATA_WIDTH)
    fsm_update_dsm = m.Reg('fsm_update_dsm', 2)
    acc_req_wr_count = m.Reg('acc_req_wr_count', NUM_CL_DSM_TOTAL_BITS + 1)
    update_dsm = m.Reg('update_dsm')
    acc_in_rd_count = m.Wire('acc_in_rd_count', QTD_WIDTH, EmbeddedCode('(NUM_CL_DSM_RD*16)'))
    acc_in_wr_count = m.Wire('acc_in_wr_count', QTD_WIDTH, EmbeddedCode('(NUM_CL_DSM_WR*16)'))
    dsm_data_wire = m.Wire('dsm_data_wire', DSM_DATA_WIDTH, NUM_CL_DSM_TOTAL)
    idx = m.Integer('idx')
    i = m.Genvar('i')

    counters = make_counter()
    genFor = m.GenerateFor(i(0), i < NUM_INPUT_QUEUES, i.inc(), 'gen_counter_rd')
    params = [('DEPTH_BITS', QTD_WIDTH)]
    con = [('clk', clk), ('rst', rst), ('en', acc_req_rd_data_en[i]), ('dout', acc_in_rd_count[i])]
    genFor.Instance(counters, 'counter_rd', params, con)

    genFor = m.GenerateFor(i(0), i < NUM_OUTPUT_QUEUES, i.inc(), 'gen_counter_wr')
    params = [('DEPTH_BITS', QTD_WIDTH)]
    con = [('clk', clk), ('rst', rst), ('en', acc_req_wr_data_en[i]), ('dout', acc_in_wr_count[i])]
    genFor.Instance(counters, 'counter_wr', params, con)

    genFor = m.GenerateFor(i(EmbeddedCode('(NUM_CL_DSM_RD*16)-1')), i >= NUM_INPUT_QUEUES, i.dec(), 'gen_zero_rd_count')
    genFor.Assign(acc_in_rd_count[i](Repeat(Int(0, 1, 2), QTD_WIDTH)))
    genFor = m.GenerateFor(i(EmbeddedCode('(NUM_CL_DSM_WR*16)-1')), i >= NUM_OUTPUT_QUEUES, i.dec(),
                           'gen_zero_wr_count')
    genFor.Assign(acc_in_wr_count[i](Repeat(Int(0, 1, 2), QTD_WIDTH)))
    genFor = m.GenerateFor(i(0), i < NUM_CL_DSM_RD, i.inc(), 'gen_dsm_data_rd')
    genFor.Assign(dsm_data_wire[i](
        Cat(acc_in_rd_count[i * 16 + 15], acc_in_rd_count[i * 16 + 14], acc_in_rd_count[i * 16 + 13],
            acc_in_rd_count[i * 16 + 12], acc_in_rd_count[i * 16 + 11], acc_in_rd_count[i * 16 + 10],
            acc_in_rd_count[i * 16 + 9], acc_in_rd_count[i * 16 + 8], acc_in_rd_count[i * 16 + 7],
            acc_in_rd_count[i * 16 + 6], acc_in_rd_count[i * 16 + 5], acc_in_rd_count[i * 16 + 4],
            acc_in_rd_count[i * 16 + 3], acc_in_rd_count[i * 16 + 2], acc_in_rd_count[i * 16 + 1],
            acc_in_rd_count[i * 16 + 0])))
    genFor = m.GenerateFor(i(0), i < NUM_CL_DSM_WR, i.inc(), 'gen_dsm_data_wr')
    genFor.Assign(dsm_data_wire[i + NUM_CL_DSM_WR](
        Cat(acc_in_wr_count[i * 16 + 15], acc_in_wr_count[i * 16 + 14], acc_in_wr_count[i * 16 + 13],
            acc_in_wr_count[i * 16 + 12], acc_in_wr_count[i * 16 + 11], acc_in_wr_count[i * 16 + 10],
            acc_in_wr_count[i * 16 + 9], acc_in_wr_count[i * 16 + 8], acc_in_wr_count[i * 16 + 7],
            acc_in_wr_count[i * 16 + 6], acc_in_wr_count[i * 16 + 5], acc_in_wr_count[i * 16 + 4],
            acc_in_wr_count[i * 16 + 3], acc_in_wr_count[i * 16 + 2], acc_in_wr_count[i * 16 + 1],
            acc_in_wr_count[i * 16 + 0])))

    m.Always(Posedge(clk))(
        If(rst)(
            done(Int(0, DSM_DATA_WIDTH.value, 10)),
            update_dsm(Int(0,1,2))
        ).Else(
            done(
                Cat(Repeat(Int(0, 1, 2), DSM_DATA_WIDTH - (NUM_INPUT_QUEUES + NUM_OUTPUT_QUEUES + 1)), done_wr, done_rd,
                    done_acc)),
            update_dsm(Uor((done ^ done_last) & done))
        )
    )
    m.Always(Posedge(clk))(
        For(idx(0), idx < NUM_CL_DSM_TOTAL, idx.inc())(
            dsm_data[idx](dsm_data_wire[idx])
        ),
    )
    m.Always(Posedge(clk))(
        If(rst)(
            acc_dsm_request_write(Int(0, 1, 2)),
            fsm_update_dsm(Int(0,fsm_update_dsm.width, 10)),
            done_last(Int(0, DSM_DATA_WIDTH.value, 10)),
            acc_req_wr_count(0),
            acc_dsm_write_data(0)
        ).Elif(start)(
            acc_dsm_request_write(Int(0, 1, 2)),
            Case(fsm_update_dsm)(
                When(Int(0,fsm_update_dsm.width, 10))(
                    If(update_dsm)(
                        done_last(done),
                        fsm_update_dsm(Int(1, fsm_update_dsm.width, 10)),
                        acc_req_wr_count(0)
                    )
                ),
                When(Int(1,fsm_update_dsm.width, 10))(
                    If(acc_dsm_available_write)(
                        acc_dsm_request_write(Int(1, 1, 2)),
                        acc_dsm_write_data(dsm_data[acc_req_wr_count]),
                        acc_req_wr_count(acc_req_wr_count + 1)
                    ),
                    If(acc_req_wr_count == NUM_CL_DSM_TOTAL - 1)(
                        fsm_update_dsm(Int(2, fsm_update_dsm.width, 10)),
                    )
                ),
                When(Int(2,fsm_update_dsm.width, 10))(
                    If(has_pending_wr)(
                        fsm_update_dsm(Int(3, fsm_update_dsm.width, 10)),
                    ),
                ),
                When(Int(3,fsm_update_dsm.width, 10))(
                    If(AndList(Not(has_pending_wr)))(
                        acc_dsm_request_write(Int(1, 1, 2)),
                        acc_dsm_write_data(done_last),
                        fsm_update_dsm(Int(0,fsm_update_dsm.width, 10)),
                    )
                )
            )
        )
    )

    return m

# make_dsm_controller().to_verilog('dsm_controller_test')
