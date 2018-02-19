from veriloggen import *

from make_counter import make_counter


def make_dsm_controller():
    m = Module('dsm_controller')
    QTD_WIDTH = m.Parameter('QTD_WIDTH', 32)
    DSM_DATA_WIDTH = m.Parameter('DSM_DATA_WIDTH', 512)
    NUM_INPUT_QUEUES = m.Parameter('NUM_INPUT_QUEUES', 1)
    NUM_OUTPUT_QUEUES = m.Parameter('NUM_OUTPUT_QUEUES', 1)
    NUM_CL_DSM_RD = m.Parameter('NUM_CL_DSM_RD', EmbeddedCode('$rtoi($ceil(NUM_INPUT_QUEUES/8))'))
    NUM_CL_DSM_WR = m.Parameter('NUM_CL_DSM_WR', EmbeddedCode('$rtoi($ceil(NUM_OUTPUT_QUEUES/8))'))
    NUM_CL_DSM_TOTAL = m.Parameter('NUM_CL_DSM_TOTAL', EmbeddedCode('1 + NUM_CL_DSM_RD + NUM_CL_DSM_WR'))
    NUM_CL_DSM_TOTAL_BITS = m.Parameter('NUM_CL_DSM_TOTAL_BITS', EmbeddedCode('$rtoi($ceil($clog2(NUM_CL_DSM_TOTAL)))'))

    clk = m.Input('clk')
    rst = m.Input('rst')

    done_rd = m.Input('done_rd', NUM_INPUT_QUEUES)
    done_wr = m.Input('done_wr', NUM_OUTPUT_QUEUES)
    done_afu = m.Input('done_afu')

    afu_req_rd_data_en = m.Input('afu_req_rd_data_en', NUM_INPUT_QUEUES)
    afu_req_wr_data_en = m.Input('afu_req_wr_data_en', NUM_OUTPUT_QUEUES)

    afu_dsm_req_rd = m.Input('afu_dsm_req_rd')
    afu_dsm_addr = m.Input('afu_dsm_addr', NUM_CL_DSM_TOTAL_BITS)
    afu_dsm_update = m.OutputReg('afu_dsm_update')
    afu_dsm_valid = m.OutputReg('afu_dsm_valid')
    afu_dsm_data = m.OutputReg('afu_dsm_data', DSM_DATA_WIDTH)

    dsm_data = m.Reg('dsm_data',DSM_DATA_WIDTH,NUM_CL_DSM_TOTAL)
    done = m.Reg('done', DSM_DATA_WIDTH)
    done_last = m.Reg('done_last', DSM_DATA_WIDTH)
    fsm_update_dsm = m.Reg('fsm_update_dsm',1)
    update_dsm = m.Wire('update_dsm')
    afu_in_rd_count = m.Wire('afu_in_rd_count', QTD_WIDTH, EmbeddedCode('(NUM_CL_DSM_RD*8)'))
    afu_in_wr_count = m.Wire('afu_in_wr_count', QTD_WIDTH, EmbeddedCode('(NUM_CL_DSM_WR*8)'))
    dsm_data_wire = m.Wire('dsm_data_wire', DSM_DATA_WIDTH, NUM_CL_DSM_TOTAL)
    idx = m.Integer('idx')
    i = m.Genvar('i')

    counters = make_counter()
    genFor = m.GenerateFor(i(0), i < NUM_INPUT_QUEUES, i.inc(), 'gen_counter_rd')
    params = [('DEPTH_BITS', QTD_WIDTH)]
    con = [('clk', clk), ('rst', rst), ('en', afu_req_rd_data_en[i]), ('dout', afu_in_rd_count[i])]
    genFor.Instance(counters, 'counter_rd', params, con)

    genFor = m.GenerateFor(i(0), i < NUM_OUTPUT_QUEUES, i.inc(), 'gen_counter_wr')
    params = [('DEPTH_BITS', QTD_WIDTH)]
    con = [('clk', clk), ('rst', rst), ('en', afu_req_wr_data_en[i]), ('dout', afu_in_wr_count[i])]
    genFor.Instance(counters, 'counter_wr', params, con)

    genFor = m.GenerateFor(i(EmbeddedCode('(NUM_CL_DSM_RD*8)-1')), i >= NUM_INPUT_QUEUES, i.dec(), 'gen_zero_rd_count')
    genFor.Assign(afu_in_rd_count[i](Repeat(Int(0, 1, 2), QTD_WIDTH)))
    genFor = m.GenerateFor(i(EmbeddedCode('(NUM_CL_DSM_WR*8)-1')), i >= NUM_OUTPUT_QUEUES, i.dec(), 'gen_zero_wr_count')
    genFor.Assign(afu_in_wr_count[i](Repeat(Int(0, 1, 2), QTD_WIDTH)))
    genFor = m.GenerateFor(i(0), i < NUM_CL_DSM_RD, i.inc(), 'gen_dsm_data_rd')
    genFor.Assign(dsm_data_wire[i](
        Cat(afu_in_rd_count[i * 8 + 7], afu_in_rd_count[i * 8 + 6], afu_in_rd_count[i * 8 + 5],
            afu_in_rd_count[i * 8 + 4], afu_in_rd_count[i * 8 + 3], afu_in_rd_count[i * 8 + 2],
            afu_in_rd_count[i * 8 + 1], afu_in_rd_count[i * 8 + 0])))
    genFor = m.GenerateFor(i(0), i < NUM_CL_DSM_WR, i.inc(), 'gen_dsm_data_wr')
    genFor.Assign(dsm_data_wire[i + NUM_CL_DSM_WR](
        Cat(afu_in_wr_count[i * 8 + 7], afu_in_wr_count[i * 8 + 6], afu_in_wr_count[i * 8 + 5],
            afu_in_wr_count[i * 8 + 4], afu_in_wr_count[i * 8 + 3], afu_in_wr_count[i * 8 + 2],
            afu_in_wr_count[i * 8 + 1], afu_in_wr_count[i * 8 + 0])))

    dsm_data_wire[NUM_CL_DSM_RD+NUM_CL_DSM_WR].assign(done)

    update_dsm.assign((done ^ done_last) & done),

    m.Always(Posedge(clk))(
        If(rst)(
            done(Int(0, DSM_DATA_WIDTH.value, 10)),
        ).Else(

            done(
                Cat(Repeat(Int(0, 1, 2), DSM_DATA_WIDTH - (NUM_INPUT_QUEUES + NUM_OUTPUT_QUEUES + 1)), done_wr, done_rd,
                    done_afu))
        )
    )
    m.Always(Posedge(clk))(
        afu_dsm_valid(Int(0, 1, 2)),
        For(idx(0), idx < NUM_CL_DSM_TOTAL, idx.inc())(
            dsm_data[idx](dsm_data_wire[idx])
        ),
        If(afu_dsm_req_rd)(
            afu_dsm_data(dsm_data[afu_dsm_addr]),
            afu_dsm_valid(Int(1, 1, 2))
        )
    )
    m.Always(Posedge(clk))(
        If(rst)(
            afu_dsm_update(Int(0, 1, 2)),
            fsm_update_dsm(Int(0,2,10)),
            done_last(Int(0, DSM_DATA_WIDTH.value, 10))
        ).Else(
            Case(fsm_update_dsm)(
                When(Int(0,2,10))(
                    If(update_dsm)(
                        afu_dsm_update(Int(1, 1, 2)),
                        done_last(done),
                        fsm_update_dsm(Int(1,2,10))
                    )
                ),
                When(Int(1,2,10))(
                    If(AndList(afu_dsm_req_rd,afu_dsm_addr == (NUM_CL_DSM_TOTAL-1)))(
                        fsm_update_dsm(Int(0,2,10)),
                        afu_dsm_update(Int(0, 1, 2))
                    )
                ),
                When()()
            )
        )
    )

    return m
