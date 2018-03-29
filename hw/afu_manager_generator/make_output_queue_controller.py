from veriloggen import *

from make_fifo import make_fifo


def make_output_queue_controller(conf_receiver):
    m = Module('output_queue_controller')
    ID_QUEUE = m.Parameter('ID_QUEUE', 0)
    ADDR_WIDTH = m.Parameter('ADDR_WIDTH', 64)
    QTD_WIDTH = m.Parameter('QTD_WIDTH', 32)
    DATA_WIDTH = m.Parameter('DATA_WIDTH', 512)
    CONF_ID_QUEUE_WIDTH = m.Parameter('CONF_ID_QUEUE_WIDTH', 32)
    TAG_WIDTH = m.Parameter('TAG_WIDTH', 16)

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

    acc_user_available_write = m.Output('acc_user_available_write')
    acc_user_request_write = m.Input('acc_user_request_write')
    acc_user_write_data = m.Input('acc_user_write_data', DATA_WIDTH)

    done = m.OutputReg('done')

    FIFO_DEPTH_BITS = m.Localparam('FIFO_DEPTH_BITS', 4)
    FIFO_FULL = m.Localparam('FIFO_FULL', EmbeddedCode('2 ** FIFO_DEPTH_BITS'))
    CONF_TYPE_OUT_DATA = m.Localparam('CONF_TYPE_OUT_DATA', 2)

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

    rst_internal = m.Wire('rst_internal')

    params = [('CONF_TYPE', CONF_TYPE_OUT_DATA), ('CONF_ID', ID_QUEUE), ('CONF_ID_WIDTH', CONF_ID_QUEUE_WIDTH),
              ('CONF_WIDTH', EmbeddedCode('ADDR_WIDTH + QTD_WIDTH + CONF_ID_QUEUE_WIDTH'))]
    con = [('clk', clk), ('rst', rst), ('conf_in_valid', conf_valid), ('conf_in_data', conf),
           ('conf_out_valid', conf_rd_valid),
           ('conf_out_data', conf_rd), ('conf_reset_out', rst_internal)]
    m.Instance(conf_receiver, 'conf_receiver', params, con)

    fifo = make_fifo()
    params = [('FIFO_WIDTH', DATA_WIDTH), ('FIFO_DEPTH_BITS', FIFO_DEPTH_BITS),
              ('FIFO_ALMOSTFULL_THRESHOLD', FIFO_FULL - 4), ('FIFO_ALMOSTEMPTY_THRESHOLD', 2)]
    con = [('clk', clk), ('rst', rst_internal), ('we', acc_user_request_write), ('din', acc_user_write_data),
           ('re', fifo_re), ('valid', fifo_dout_valid), ('dout', fifo_dout), ('count', fifo_count),
           ('empty', fifo_empty), ('full', fifo_full), ('almostfull', fifo_almostfull),
           ('almostempty', fifo_almostempty)]
    m.Instance(fifo, 'fifo', params, con)

    end_req_wr_data.assign((count_req_cl >= qtd_data_cl))
    issue_req_data.assign(AndList(start & conf_ready & ~end_req_wr_data & available_write,
                                  Mux(fifo_almostempty, (~fifo_empty & ~fifo_re), Int(1, 1, 2))))
    acc_user_available_write.assign(~fifo_almostfull)
    write_data_valid_queue.assign(AndList(write_data_valid, (write_queue_id == ID_QUEUE)))

    m.Always(Posedge(clk))(
        If(rst_internal)(
            done(Int(0, 1, 2)),
            has_wr_peding(Int(0, 1, 2))
        ).Else(
            done(AndList((count_cl >= qtd_data_cl), start, fifo_empty)),
            has_wr_peding(Mux(write_peding > 0, Int(1, 1, 2), Int(0, 1, 2)))
        )
    )

    m.Always(Posedge(clk))(
        If(rst_internal)(
            addr_base(0),
            qtd_data_cl(0),
            conf_ready(Int(0, 1, 2)),
        ).Else(
            If(conf_rd_valid)(
                qtd_data_cl(conf_rd[CONF_ID_QUEUE_WIDTH:CONF_ID_QUEUE_WIDTH + QTD_WIDTH]),
                addr_base(conf_rd[CONF_ID_QUEUE_WIDTH + QTD_WIDTH:CONF_ID_QUEUE_WIDTH + QTD_WIDTH + ADDR_WIDTH]),
                conf_ready(Int(1, 1, 2))
            )
        )
    )

    m.Always(Posedge(clk))(
        If(rst_internal)(
            addr_write_next(0),
            count_req_cl(0),
            flag_addr_init(Int(1, 1, 2))
        ).Else(
            If(conf_ready & flag_addr_init)(
                addr_write_next(addr_base),
                flag_addr_init(Int(0, 1, 2))
            ).Elif(fifo_dout_valid)(
                addr_write_next(addr_write_next + 1),
                count_req_cl(count_req_cl + 1)
            )
        )
    )

    m.Always(Posedge(clk))(
        If(rst_internal)(
            count_cl(0),
        ).Else(
            If(write_data_valid_queue)(
                count_cl(count_cl + 4)
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

    m.Always(Posedge(clk))(
        If(rst_internal)(
            write_peding(0)
        ).Else(
            Case(Cat(write_data_valid_queue, fifo_dout_valid))(
                When(Int(0, 2, 10))(
                    write_peding(write_peding)
                ),
                When(Int(1, 2, 10))(
                    write_peding(write_peding + 1)
                ),
                When(Int(2, 2, 10))(
                    write_peding(write_peding - 4)
                ),
                When(Int(3, 2, 10))(
                    write_peding(write_peding - 3)
                )
            )
        )
    )

    m.Always(Posedge(clk))(
        If(rst_internal)(
            request_write(Int(0, 1, 2)),
            write_data(0)
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
