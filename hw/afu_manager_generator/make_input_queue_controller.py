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
    FIFO_DEPTH_BITS = m.Parameter('FIFO_DEPTH_BITS', 4)
    FIFO_FULL = m.Parameter('FIFO_FULL', EmbeddedCode('2 ** FIFO_DEPTH_BITS'))

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

    afu_user_available_read = m.Output('afu_user_available_read')
    afu_user_request_read = m.Input('afu_user_request_read')
    afu_user_read_data = m.Output('afu_user_read_data', DATA_WIDTH)
    afu_user_read_data_valid = m.Output('afu_user_read_data_valid')
    done = m.Output('done')

    CONF_TYPE_IN = m.Localparam('CONF_TYPE_IN',1)

    conf_ready = m.Reg('conf_ready')
    addr_base = m.Reg('addr_base', ADDR_WIDTH)
    addr_read_next = m.Reg('addr_read_next', ADDR_WIDTH)
    qtd_data_cl = m.Reg('qtd_data_cl', QTD_WIDTH)
    count_req_cl = m.Reg('count_req_cl', QTD_WIDTH)
    count_cl = m.Reg('count_cl', QTD_WIDTH)
    read_peding = m.Reg('read_peding', QTD_WIDTH)
    flag_addr_init = m.Reg('flag_addr_init')
    fifo_we = m.Reg('fifo_we')
    din = m.Reg('din', DATA_WIDTH)
    fifo_fit = m.Wire('fifo_fit')
    issue_req_data = m.Wire('issue_req_data')
    fifo_empty = m.Wire('fifo_empty')
    fifo_count = m.Wire('fifo_count', FIFO_DEPTH_BITS + 1)
    read_data_valid_queue = m.Wire('read_data_valid_queue')
    fifo_full = m.Wire('fifo_full')
    fifo_almostfull = m.Wire('fifo_almostfull')
    fifo_almostempty = m.Wire('fifo_almostempty')
    end_req_rd_data = m.Wire('end_req_rd_data')
    conf_rd_valid = m.Wire('conf_rd_valid')
    conf_rd = m.Wire('conf_rd', EmbeddedCode(ADDR_WIDTH + QTD_WIDTH + CONF_ID_QUEUE_WIDTH))

    fifo = make_fifo()
    params = [('FIFO_WIDTH', DATA_WIDTH), ('FIFO_DEPTH_BITS', FIFO_DEPTH_BITS),
              ('FIFO_ALMOSTFULL_THRESHOLD', FIFO_FULL - 4), ('FIFO_ALMOSTEMPTY_THRESHOLD', 2)]
    con = [('clk', clk), ('rst', conf_rd_valid), ('we', fifo_we), ('din', din), ('re', afu_user_request_read),
           ('valid', afu_user_read_data_valid), ('dout', afu_user_read_data), ('count', fifo_count),
           ('empty', fifo_empty), ('full', fifo_full), ('almostfull', fifo_almostfull),
           ('almostempty', fifo_almostempty)]
    m.Instance(fifo, 'fifo', params, con)

    params = [('CONF_TYPE',CONF_TYPE_IN), ('CONF_ID', ID_QUEUE), ('CONF_ID_WIDTH', CONF_ID_QUEUE_WIDTH),
              ('CONF_WIDTH', EmbeddedCode('ADDR_WIDTH + QTD_WIDTH + CONF_ID_QUEUE_WIDTH'))]
    con = [('clk', clk), ('rst', rst), ('conf_in_valid', conf_valid), ('conf_in_data', conf),
           ('conf_out_valid', conf_rd_valid),
           ('conf_out_data', conf_rd)]
    m.Instance(conf_receiver, 'conf_receiver', params, con)

    end_req_rd_data.assign((count_req_cl >= qtd_data_cl))
    done.assign(AndList(count_cl >= qtd_data_cl, (read_peding == 0), start))
    issue_req_data.assign(start & conf_ready & ~end_req_rd_data & available_read & fifo_fit & ~request_read)
    fifo_fit.assign(EmbeddedCode('(read_peding + fifo_count) < FIFO_FULL'))
    afu_user_available_read.assign(Mux(fifo_almostempty, ~fifo_empty & ~afu_user_request_read, Int(1, 1, 2)))
    read_data_valid_queue.assign(AndList(read_data_valid, read_queue_id == ID_QUEUE))

    m.Always(Posedge(clk))(
        If(conf_rd_valid)(
            has_rd_peding(0)
        ).Else(
            has_rd_peding(Mux(read_peding > 0, Int(1, 1, 2), Int(0, 1, 2)))
        )
    )
    m.Always(Posedge(clk))(
        If(rst)(
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
        If(conf_rd_valid)(
            addr_read_next(0),
            count_req_cl(0),
            flag_addr_init(Int(1, 1, 2))
        ).Else(
            If(conf_ready & flag_addr_init)(
                addr_read_next(addr_base),
                flag_addr_init(Int(0, 1, 2))
            ).Elif(request_read)(
                addr_read_next.inc(),
                count_req_cl.inc()
            )
        )
    )
    m.Always(Posedge(clk))(
        If(conf_rd_valid)(
            request_read(Int(0, 1, 2)),
            request_data(0)
        ).Else(
            request_read(Int(0, 1, 2)),
            If(issue_req_data)(
                request_read(Int(1, 1, 2)),
                request_data(Cat(addr_read_next, ID_QUEUE[0:TAG_WIDTH]))
            )
        )
    )
    m.Always(Posedge(clk))(
        If(conf_rd_valid)(
            read_peding(0)
        ).Else(
            Case(Cat(fifo_we, request_read))(
                When(Int(0, 2, 10))(
                    read_peding(read_peding)
                ),
                When(Int(1, 2, 10))(
                    read_peding.inc()
                ),
                When(Int(2, 2, 10))(
                    read_peding.dec()
                ),
                When(Int(3, 2, 10))(
                    read_peding(read_peding)
                )
            )
        )
    )
    m.Always(Posedge(clk))(
        If(conf_rd_valid)(
            fifo_we(Int(0, 1, 2)),
            din(0),
            count_cl(0)
        ).Else(
            fifo_we(Int(0, 1, 2)),
            If(read_data_valid_queue)(
                fifo_we(Int(1, 1, 2)),
                din(read_data),
                count_cl.inc()
            )
        )
    )

    return m

# make_input_queue_controller().to_verilog('input_queue_controller_test')
