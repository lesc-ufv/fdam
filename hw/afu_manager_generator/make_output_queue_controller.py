from veriloggen import *

from make_fifo import make_fifo
from util import make_const


def make_output_queue_controller(conf_receiver, isDsmOutput):
    name = 'output_queue_controller'
    if isDsmOutput:
        name = 'output_queue_controller_dsm'
    m = Module(name)

    if isDsmOutput:
        AFU_ID = m.Parameter('AFU_ID', 0)
    ID_QUEUE = m.Parameter('ID_QUEUE', 0)
    ADDR_WIDTH = m.Parameter('ADDR_WIDTH', 64)
    QTD_WIDTH = m.Parameter('QTD_WIDTH', 32)
    DATA_WIDTH = m.Parameter('DATA_WIDTH', 512)
    CONF_ID_QUEUE_WIDTH = m.Parameter('CONF_ID_QUEUE_WIDTH', 32)
    TAG_WIDTH = m.Parameter('TAG_WIDTH', 16)
    FIFO_DEPTH_BITS = m.Parameter('FIFO_DEPTH_BITS', 4)
    FIFO_FULL = m.Parameter('FIFO_FULL', EmbeddedCode('2 ** FIFO_DEPTH_BITS'))

    clk = m.Input('clk')
    rst = m.Input('rst')
    start = m.Input('start')

    conf_valid = m.Input('conf_valid', 2)
    conf = m.Input('conf', EmbeddedCode('ADDR_WIDTH + QTD_WIDTH + CONF_ID_QUEUE_WIDTH'))

    available_write = m.Input('available_write')
    has_wr_peding = m.OutputReg('has_wr_peding')
    request_write = m.OutputReg('request_write')
    write_data = m.OutputReg('write_data', DATA_WIDTH + ADDR_WIDTH + TAG_WIDTH)

    write_data_valid = m.Input('write_data_valid')
    write_queue_id = m.Input('write_queue_id', TAG_WIDTH)

    afu_user_available_write = m.Output('afu_user_available_write')
    afu_user_request_write = m.Input('afu_user_request_write')
    afu_user_write_data = m.Input('afu_user_write_data', DATA_WIDTH)
    if isDsmOutput:
        dsm_has_wr_peding = m.OutputReg('has_wr_peding_dsm')
        afu_dsm_available_write = m.Output('afu_dsm_available_write')
        afu_dsm_request_write = m.Input('afu_dsm_request_write')
        afu_dsm_write_data = m.Input('afu_dsm_write_data', DATA_WIDTH)

    done = m.OutputReg('done')

    CONF_TYPE_OUT_DATA = m.Localparam('CONF_TYPE_OUT_DATA', 2)
    CONF_TYPE_OUT_DSM = m.Localparam('CONF_TYPE_OUT_DSM', 3)

    conf_ready = m.Reg('conf_ready')
    addr_base = m.Reg('addr_base', ADDR_WIDTH)
    addr_write_next = m.Reg('addr_write_next', ADDR_WIDTH)
    qtd_data_cl = m.Reg('qtd_data_cl', QTD_WIDTH)
    count_req_cl = m.Reg('count_req_cl', QTD_WIDTH)
    count_cl = m.Reg('count_cl', QTD_WIDTH)
    write_peding = m.Reg('write_peding', 16)
    flag_addr_init = m.Reg('flag_addr_init')
    fifo_re = m.Reg('fifo_re')
    issue_req_data = m.Wire('issue_req_data')
    fifo_empty = m.Wire('fifo_empty')
    fifo_count = m.Wire('fifo_count', FIFO_DEPTH_BITS + 1)
    fifo_full = m.Wire('fifo_full')
    fifo_almostfull = m.Wire('fifo_almostfull')
    fifo_almostempty = m.Wire('fifo_almostempty')
    write_data_valid_queue = m.Wire('write_data_valid_queue')
    fifo_dout_valid = m.Wire('fifo_dout_valid')
    fifo_dout = m.Wire('fifo_dout', DATA_WIDTH)
    end_req_wr_data = m.Wire('end_req_rd_data')
    conf_rd_valid = m.Wire('conf_rd_valid')
    conf_rd = m.Wire('conf_rd', EmbeddedCode(ADDR_WIDTH + QTD_WIDTH + CONF_ID_QUEUE_WIDTH))

    if isDsmOutput:
        dsm_conf_ready = m.Reg('dsm_conf_ready')
        dsm_addr_base = m.Reg('dsm_addr_base', ADDR_WIDTH)
        dsm_addr_write_next = m.Reg('dsm_addr_write_next', ADDR_WIDTH)
        dsm_qtd_data_cl = m.Reg('dsm_qtd_data_cl', QTD_WIDTH)
        dsm_count_req_cl = m.Reg('dsm_count_req_cl', QTD_WIDTH)
        dsm_count_cl = m.Reg('dsm_count_cl', QTD_WIDTH)
        dsm_write_peding = m.Reg('dsm_write_peding', QTD_WIDTH)
        dsm_flag_addr_init = m.Reg('dsm_flag_addr_init')
        dsm_fifo_re = m.Reg('dsm_fifo_re')
        dsm_issue_req_data = m.Wire('dsm_issue_req_data')
        dsm_fifo_empty = m.Wire('dsm_fifo_empty')
        dsm_fifo_count = m.Wire('dsm_fifo_count', FIFO_DEPTH_BITS + 1)
        dsm_fifo_full = m.Wire('dsm_fifo_full')
        dsm_fifo_almostfull = m.Wire('dsm_fifo_almostfull')
        dsm_fifo_almostempty = m.Wire('dsm_fifo_almostempty')
        dsm_write_data_valid_queue = m.Wire('dsm_write_data_valid_queue')
        dsm_fifo_dout_valid = m.Wire('dsm_fifo_dout_valid')
        dsm_fifo_dout = m.Wire('dsm_fifo_dout', DATA_WIDTH)
        dsm_end_req_wr_data = m.Wire('dsm_end_req_wr_data')
        dsm_conf_rd_valid = m.Wire('dsm_conf_rd_valid')
        dsm_conf_rd = m.Wire('dsm_conf_rd', EmbeddedCode(ADDR_WIDTH + QTD_WIDTH + CONF_ID_QUEUE_WIDTH))
        dsm_rst_internal = m.Wire('dsm_rst_internal')

    rst_internal = m.Wire('rst_internal')

    params = [('CONF_TYPE', CONF_TYPE_OUT_DATA), ('CONF_ID', ID_QUEUE), ('CONF_ID_WIDTH', CONF_ID_QUEUE_WIDTH),
              ('CONF_WIDTH', EmbeddedCode('ADDR_WIDTH + QTD_WIDTH + CONF_ID_QUEUE_WIDTH'))]
    con = [('clk', clk), ('rst', rst), ('conf_in_valid', conf_valid), ('conf_in_data', conf),
           ('conf_out_valid', conf_rd_valid),
           ('conf_out_data', conf_rd), ('conf_reset_out', rst_internal)]
    m.Instance(conf_receiver, 'conf_receiver', params, con)

    if isDsmOutput:
        params = [('CONF_TYPE', CONF_TYPE_OUT_DSM), ('CONF_ID', AFU_ID), ('CONF_ID_WIDTH', CONF_ID_QUEUE_WIDTH),
                  ('CONF_WIDTH', EmbeddedCode('ADDR_WIDTH + QTD_WIDTH + CONF_ID_QUEUE_WIDTH'))]
        con = [('clk', clk), ('rst', rst), ('conf_in_valid', conf_valid), ('conf_in_data', conf),
               ('conf_out_valid', dsm_conf_rd_valid),
               ('conf_out_data', dsm_conf_rd), ('conf_reset_out', dsm_rst_internal)]
        m.Instance(conf_receiver, 'dsm_conf_receiver', params, con)

    fifo = make_fifo()
    params = [('FIFO_WIDTH', DATA_WIDTH), ('FIFO_DEPTH_BITS', FIFO_DEPTH_BITS),
              ('FIFO_ALMOSTFULL_THRESHOLD', FIFO_FULL - 4), ('FIFO_ALMOSTEMPTY_THRESHOLD', 2)]
    con = [('clk', clk), ('rst', rst_internal), ('we', afu_user_request_write), ('din', afu_user_write_data),
           ('re', fifo_re), ('valid', fifo_dout_valid), ('dout', fifo_dout), ('count', fifo_count),
           ('empty', fifo_empty), ('full', fifo_full), ('almostfull', fifo_almostfull),
           ('almostempty', fifo_almostempty)]
    m.Instance(fifo, 'fifo', params, con)

    if isDsmOutput:
        params = [('FIFO_WIDTH', DATA_WIDTH), ('FIFO_DEPTH_BITS', FIFO_DEPTH_BITS),
                  ('FIFO_ALMOSTFULL_THRESHOLD', FIFO_FULL - 4), ('FIFO_ALMOSTEMPTY_THRESHOLD', 2)]
        con = [('clk', clk), ('rst', dsm_rst_internal), ('we', afu_dsm_request_write), ('din', afu_dsm_write_data),
               ('re', dsm_fifo_re), ('valid', dsm_fifo_dout_valid), ('dout', dsm_fifo_dout), ('count', dsm_fifo_count),
               ('empty', dsm_fifo_empty), ('full', dsm_fifo_full), ('almostfull', dsm_fifo_almostfull),
               ('almostempty', dsm_fifo_almostempty)]

        m.Instance(fifo, 'dms_fifo', params, con)

    end_req_wr_data.assign((count_req_cl >= qtd_data_cl))
    issue_req_data.assign(AndList(start & conf_ready & ~end_req_wr_data & available_write,
                                  Mux(fifo_almostempty, (~fifo_empty & ~fifo_re), Int(1, 1, 2))))
    afu_user_available_write.assign(~fifo_almostfull)
    write_data_valid_queue.assign(AndList(write_data_valid, (write_queue_id == ID_QUEUE)))

    if isDsmOutput:
        dsm_end_req_wr_data.assign((dsm_count_req_cl >= dsm_qtd_data_cl))
        dsm_issue_req_data.assign(AndList(start & dsm_conf_ready & available_write,
                                          Mux(dsm_fifo_almostempty, (~dsm_fifo_empty & ~dsm_fifo_re), Int(1, 1, 2))))
        afu_dsm_available_write.assign(~dsm_fifo_almostfull)
        dsm_write_data_valid_queue.assign(
            AndList(write_data_valid, (write_queue_id == Cat(Int(1, 1, 2), AFU_ID[0:TAG_WIDTH - 1]))))
        m.Always(Posedge(clk))(
            If(dsm_rst_internal)(
                dsm_has_wr_peding(Int(0, 1, 2))
            ).Else(
                dsm_has_wr_peding(Mux(dsm_write_peding > make_const(0, QTD_WIDTH), Int(1, 1, 2), Int(0, 1, 2)))
            )
        )

        m.Always(Posedge(clk))(
            If(rst_internal)(
                done(Int(0, 1, 2)),
                has_wr_peding(Int(0, 1, 2))
            ).Else(
                done(AndList((count_cl >= qtd_data_cl), start)),
                has_wr_peding(Mux(write_peding > make_const(0, QTD_WIDTH), Int(1, 1, 2), Int(0, 1, 2)))
            )
        )

        m.Always(Posedge(clk))(
            If(dsm_rst_internal)(
                dsm_addr_base(make_const(0, ADDR_WIDTH)),
                dsm_qtd_data_cl(make_const(0, QTD_WIDTH)),
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
    else:
        m.Always(Posedge(clk))(
            If(rst_internal)(
                done(Int(0, 1, 2)),
                has_wr_peding(Int(0, 1, 2))
            ).Else(
                done(AndList((count_cl >= qtd_data_cl), start, fifo_empty)),
                has_wr_peding(Mux(write_peding > make_const(0, QTD_WIDTH), Int(1, 1, 2), Int(0, 1, 2)))
            )
        )

    m.Always(Posedge(clk))(
        If(rst_internal)(
            addr_base(make_const(0, ADDR_WIDTH)),
            qtd_data_cl(make_const(0, QTD_WIDTH)),
            conf_ready(Int(0, 1, 2)),
        ).Else(
            If(conf_rd_valid)(
                qtd_data_cl(conf_rd[CONF_ID_QUEUE_WIDTH:CONF_ID_QUEUE_WIDTH + QTD_WIDTH]),
                addr_base(conf_rd[CONF_ID_QUEUE_WIDTH + QTD_WIDTH:CONF_ID_QUEUE_WIDTH + QTD_WIDTH + ADDR_WIDTH]),
                conf_ready(Int(1, 1, 2))
            )
        )
    )
    if isDsmOutput:
        m.Always(Posedge(clk))(
            If(dsm_rst_internal)(
                dsm_addr_write_next(make_const(0, ADDR_WIDTH)),
                dsm_count_req_cl(make_const(0, QTD_WIDTH)),
                dsm_flag_addr_init(Int(1, 1, 2))
            ).Else(
                If(AndList(dsm_conf_ready,dsm_flag_addr_init|dsm_end_req_wr_data))(
                    dsm_addr_write_next(dsm_addr_base),
                    dsm_flag_addr_init(Int(0, 1, 2)),
                    dsm_count_req_cl(make_const(0, QTD_WIDTH)),
                ).Elif(dsm_fifo_dout_valid)(
                    dsm_addr_write_next(dsm_addr_write_next + make_const(1, ADDR_WIDTH)),
                    dsm_count_req_cl(dsm_count_req_cl + make_const(1, QTD_WIDTH))
                )
            )
        )

    m.Always(Posedge(clk))(
        If(rst_internal)(
            addr_write_next(make_const(0, ADDR_WIDTH)),
            count_req_cl(make_const(0, QTD_WIDTH)),
            flag_addr_init(Int(1, 1, 2))
        ).Else(
            If(conf_ready & flag_addr_init)(
                addr_write_next(addr_base),
                flag_addr_init(Int(0, 1, 2))
            ).Elif(fifo_dout_valid)(
                addr_write_next(addr_write_next + make_const(1, ADDR_WIDTH)),
                count_req_cl(count_req_cl + make_const(1, QTD_WIDTH))
            )
        )
    )

    if isDsmOutput:
        m.Always(Posedge(clk))(
            If(dsm_rst_internal)(
                dsm_count_cl(make_const(0, QTD_WIDTH)),
            ).Else(
                If(dsm_write_data_valid_queue)(
                    dsm_count_cl(dsm_count_cl + make_const(0, QTD_WIDTH))
                )
            )
        )

    m.Always(Posedge(clk))(
        If(rst_internal)(
            count_cl(make_const(0, QTD_WIDTH)),
        ).Else(
            If(write_data_valid_queue)(
                count_cl(count_cl + make_const(1, QTD_WIDTH))
            )
        )
    )
    if isDsmOutput:
        m.Always(Posedge(clk))(
            If(dsm_rst_internal)(
                dsm_fifo_re(Int(0, 1, 2))
            ).Else(
                dsm_fifo_re(Int(0, 1, 2)),
                If(dsm_issue_req_data & ~issue_req_data)(
                    dsm_fifo_re(Int(1, 1, 2)),
                )
            )
        )

    m.Always(Posedge(clk))(
        If(rst_internal)(
            fifo_re(Int(0, 1, 2))
        ).Else(
            fifo_re(Int(0, 1, 2)),
            If(issue_req_data)(
                fifo_re(Int(1, 1, 2)),
            )
        )
    )

    if isDsmOutput:
        m.Always(Posedge(clk))(
            If(dsm_rst_internal)(
                dsm_write_peding(make_const(0, QTD_WIDTH))
            ).Else(
                Case(Cat(dsm_write_data_valid_queue, dsm_fifo_dout_valid))(
                    When(Int(0, 2, 10))(
                        dsm_write_peding(dsm_write_peding)
                    ),
                    When(Int(1, 2, 10))(
                        dsm_write_peding(dsm_write_peding + make_const(1, QTD_WIDTH))
                    ),
                    When(Int(2, 2, 10))(
                        dsm_write_peding(dsm_write_peding - make_const(1, QTD_WIDTH))
                    ),
                    When(Int(3, 2, 10))(
                        dsm_write_peding(dsm_write_peding)
                    )
                )
            )
        )

    m.Always(Posedge(clk))(
        If(rst_internal)(
            write_peding(make_const(0, QTD_WIDTH))
        ).Else(
            Case(Cat(write_data_valid_queue, fifo_dout_valid))(
                When(Int(0, 2, 10))(
                    write_peding(write_peding)
                ),
                When(Int(1, 2, 10))(
                    write_peding(write_peding + make_const(1, QTD_WIDTH))
                ),
                When(Int(2, 2, 10))(
                    write_peding(write_peding - make_const(1, QTD_WIDTH))
                ),
                When(Int(3, 2, 10))(
                    write_peding(write_peding)
                )
            )
        )
    )
    if isDsmOutput:
        m.Always(Posedge(clk))(
            If(dsm_rst_internal)(
                request_write(Int(0, 1, 2)),
                write_data(make_const(0, DATA_WIDTH + ADDR_WIDTH + TAG_WIDTH))
            ).Else(
                request_write(Int(0, 1, 2)),
                If(fifo_dout_valid)(
                    request_write(Int(1, 1, 2)),
                    write_data(Cat(fifo_dout, addr_write_next, ID_QUEUE[0:TAG_WIDTH]))
                ).Elif(dsm_fifo_dout_valid)(
                    request_write(Int(1, 1, 2)),
                    write_data(Cat(dsm_fifo_dout, dsm_addr_write_next, Cat(Int(1, 1, 2), AFU_ID[0:TAG_WIDTH - 1])))
                )
            )
        )
    else:
        m.Always(Posedge(clk))(
            If(rst_internal)(
                request_write(Int(0, 1, 2)),
                write_data(make_const(0, DATA_WIDTH + ADDR_WIDTH + TAG_WIDTH))
            ).Else(
                request_write(Int(0, 1, 2)),
                If(fifo_dout_valid)(
                    request_write(Int(1, 1, 2)),
                    write_data(Cat(fifo_dout, addr_write_next, ID_QUEUE[0:TAG_WIDTH]))
                )
            )
        )

    return m

# make_output_queue_controller().to_verilog('output_queue_controller_test')
