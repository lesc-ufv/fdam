from veriloggen import *

from make_fifo import make_fifo


def make_input_queue_controller(conf_receiver):
    m = Module('input_queue_controller')
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

    available_read = m.Input('available_read')
    has_rd_peding = m.OutputReg('has_rd_peding')
    request_read = m.OutputReg('request_read')
    request_data = m.OutputReg('request_data', ADDR_WIDTH + TAG_WIDTH)

    read_data_valid = m.Input('read_data_valid')
    read_queue_id = m.Input('read_queue_id', TAG_WIDTH)
    read_data = m.Input('read_data', DATA_WIDTH)

    acc_user_available_read = m.Output('acc_user_available_read')
    acc_user_request_read = m.Input('acc_user_request_read')
    acc_user_read_data = m.OutputReg('acc_user_read_data', DATA_WIDTH)
    acc_user_read_data_valid = m.OutputReg('acc_user_read_data_valid')
    done = m.OutputReg('done')

    FIFO_DEPTH_BITS = m.Localparam('FIFO_DEPTH_BITS', 10)
    FIFO_FULL = m.Localparam('FIFO_FULL', EmbeddedCode('2 ** FIFO_DEPTH_BITS'))
    CONF_TYPE_IN = m.Localparam('CONF_TYPE_IN', 1)

    conf_ready = m.Reg('conf_ready')
    addr_base = m.Reg('addr_base', ADDR_WIDTH)
    addr_read_next = m.Reg('addr_read_next', ADDR_WIDTH)
    qtd_data_cl = m.Reg('qtd_data_cl', QTD_WIDTH)
    count_req_cl = m.Reg('count_req_cl', QTD_WIDTH)
    count_cl = m.Reg('count_cl', QTD_WIDTH)
    read_peding = m.Reg('read_peding', FIFO_DEPTH_BITS + 1)
    read_peding_1 = m.Reg('read_peding_1', FIFO_DEPTH_BITS + 1)
    flag_addr_init = m.Reg('flag_addr_init')
    fifo_we = m.Reg('fifo_we')
    din = m.Reg('din', DATA_WIDTH)
    fifo_fit = m.Wire('fifo_fit')
    issue_req_data = m.Reg('issue_req_data')
    issue_req_data_next = m.Reg('issue_req_data_next')
    fifo_empty = m.Wire('fifo_empty')
    fifo_count = m.Wire('fifo_count', FIFO_DEPTH_BITS + 1)
    read_data_valid_queue = m.Wire('read_data_valid_queue')
    fifo_full = m.Wire('fifo_full')
    fifo_almostfull = m.Wire('fifo_almostfull')
    fifo_almostempty = m.Wire('fifo_almostempty')
    end_req_rd_data = m.Wire('end_req_rd_data')
    end_req_rd_data_next = m.Wire('end_req_rd_data_next')
    conf_rd_valid = m.Wire('conf_rd_valid')
    conf_rd = m.Wire('conf_rd', EmbeddedCode(ADDR_WIDTH + QTD_WIDTH + CONF_ID_QUEUE_WIDTH))
    rst_internal = m.Wire('rst_internal')
    acc_user_read_data_out = m.Wire('acc_user_read_data_out', DATA_WIDTH)
    acc_user_read_data_valid_out = m.Wire('acc_user_read_data_valid_out')

    params = [('CONF_TYPE', CONF_TYPE_IN), ('CONF_ID', ID_QUEUE), ('CONF_ID_WIDTH', CONF_ID_QUEUE_WIDTH),
              ('CONF_WIDTH', EmbeddedCode('ADDR_WIDTH + QTD_WIDTH + CONF_ID_QUEUE_WIDTH'))]
    con = [('clk', clk), ('rst', rst), ('conf_in_valid', conf_valid), ('conf_in_data', conf),
           ('conf_out_valid', conf_rd_valid),
           ('conf_out_data', conf_rd), ('conf_reset_out', rst_internal)]
    m.Instance(conf_receiver, 'conf_receiver', params, con)
    fifo = make_fifo()
    params = [('FIFO_WIDTH', DATA_WIDTH), ('FIFO_DEPTH_BITS', FIFO_DEPTH_BITS),
              ('FIFO_ALMOSTFULL_THRESHOLD', FIFO_FULL - 4), ('FIFO_ALMOSTEMPTY_THRESHOLD', 2)]
    con = [('clk', clk), ('rst', rst_internal), ('we', fifo_we), ('din', din), ('re', acc_user_request_read),
           ('valid', acc_user_read_data_valid_out), ('dout', acc_user_read_data_out), ('count', fifo_count),
           ('empty', fifo_empty), ('full', fifo_full), ('almostfull', fifo_almostfull),
           ('almostempty', fifo_almostempty)]
    m.Instance(fifo, 'fifo', params, con)

    end_req_rd_data.assign((count_req_cl < qtd_data_cl))
    end_req_rd_data_next.assign(((count_req_cl + 12) < qtd_data_cl))
    acc_user_available_read.assign(Mux(fifo_almostempty, ~fifo_empty & ~acc_user_request_read, Int(1, 1, 2))),
    fifo_fit.assign(read_peding < FIFO_FULL - 12)
    read_data_valid_queue.assign(AndList(read_data_valid, read_queue_id == ID_QUEUE))

    m.Always(Posedge(clk))(
        If(rst_internal)(
            done(Int(0, 1, 2)),
            acc_user_read_data(0),
            acc_user_read_data_valid(Int(0, 1, 2)),
        ).Else(
            done(AndList(count_cl >= qtd_data_cl, start)),
            acc_user_read_data(acc_user_read_data_out),
            acc_user_read_data_valid(acc_user_read_data_valid_out)
        )
    )

    m.Always(Posedge(clk))(
        If(rst_internal)(
            has_rd_peding(Int(0, 1, 2))
        ).Else(
            has_rd_peding(Mux(read_peding_1 > 0, Int(1, 1, 2), Int(0, 1, 2)))
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
            addr_read_next(0),
            flag_addr_init(Int(1, 1, 2))
        ).Else(
            If(conf_ready & flag_addr_init)(
                addr_read_next(addr_base),
                flag_addr_init(Int(0, 1, 2))
            ).Elif(issue_req_data)(
                addr_read_next(addr_read_next + 4),
            )
        )
    )
    m.Always(Posedge(clk))(
        If(rst_internal)(
            request_read(Int(0, 1, 2)),
            request_data(0),
            count_req_cl(0),
            issue_req_data(Int(0, 1, 2)),
            issue_req_data_next(Int(0, 1, 2))
        ).Else(
            request_read(Int(0, 1, 2)),
            issue_req_data(start & conf_ready & end_req_rd_data & available_read & fifo_fit),
            issue_req_data_next(start & conf_ready & end_req_rd_data_next & available_read & fifo_fit),
            If(issue_req_data)(
                request_read(Int(1, 1, 2)),
                request_data(Cat(addr_read_next, ID_QUEUE[0:TAG_WIDTH])),
                count_req_cl(count_req_cl + 4),
                issue_req_data(issue_req_data_next)
            )
        )
    )
    m.Always(Posedge(clk))(
        If(rst_internal)(
            read_peding(0),
            read_peding_1(0)
        ).Else(
            Case(Cat(acc_user_request_read, request_read))(
                When(Int(0, 2, 10))(
                    read_peding(read_peding)
                ),
                When(Int(1, 2, 10))(
                    read_peding(read_peding + 4)
                ),
                When(Int(2, 2, 10))(
                    read_peding(read_peding - 1)
                ),
                When(Int(3, 2, 10))(
                    read_peding(read_peding + 3)
                )
            ),
            Case(Cat(read_data_valid_queue, request_read))(
                When(Int(0, 2, 10))(
                    read_peding_1(read_peding_1)
                ),
                When(Int(1, 2, 10))(
                    read_peding_1(read_peding_1 + 4)
                ),
                When(Int(2, 2, 10))(
                    read_peding_1(read_peding_1 - 1)
                ),
                When(Int(3, 2, 10))(
                    read_peding_1(read_peding_1 + 3)
                )
            )
        )
    )
    m.Always(Posedge(clk))(
        If(rst_internal)(
            fifo_we(Int(0, 1, 2)),
            din(0),
            count_cl(0)
        ).Else(
            fifo_we(Int(0, 1, 2)),
            If(read_data_valid_queue)(
                fifo_we(Int(1, 1, 2)),
                din(read_data),
                count_cl(count_cl + 1)
            )
        )
    )

    return m

# make_input_queue_controller(make_conf_receiver()).to_verilog('../input_queue_controller_test')
