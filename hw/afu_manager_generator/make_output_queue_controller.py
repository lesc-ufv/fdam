from veriloggen import *

from make_fifo import make_fifo

def make_output_queue_controller():
    m = Module('output_queue_controller')
    ID_QUEUE = m.Parameter('ID_QUEUE',0)
    ADDR_WIDTH = m.Parameter('ADDR_WIDTH',64)
    QTD_WIDTH = m.Parameter('QTD_WIDTH',32)
    DATA_WIDTH = m.Parameter('DATA_WIDTH',512)
    CONF_ID_QUEUE_WIDTH = m.Parameter('CONF_ID_QUEUE_WIDTH',32)
    TAG_WIDTH = m.Parameter('TAG_WIDTH',16)
    FIFO_DEPTH_BITS = m.Parameter('FIFO_DEPTH_BITS',4)
    FIFO_FULL = m.Parameter('FIFO_FULL',EmbeddedCode('2 ** FIFO_DEPTH_BITS'))

    clk = m.Input('clk')
    rst = m.Input('rst')
    start = m.Input('start')

    conf_valid = m.Input('conf_valid',2)
    conf = m.Input('conf',EmbeddedCode('ADDR_WIDTH + QTD_WIDTH + CONF_ID_QUEUE_WIDTH'))

    available_write = m.Input('available_write')
    request_write = m.OutputReg('request_write')
    write_data = m.OutputReg('write_data',DATA_WIDTH+ADDR_WIDTH+TAG_WIDTH)

    write_data_valid = m.Input('write_data_valid')
    write_queue_id = m.Input('write_queue_id',TAG_WIDTH)

    afu_user_available_write = m.Output('afu_user_available_write')
    afu_user_request_write = m.Input('afu_user_request_write')
    afu_user_write_data = m.Input('afu_user_write_data',DATA_WIDTH)

    done = m.Output('done')

    conf_ready = m.Reg('conf_ready')
    conf_read = m.Reg('conf_read')
    conf_reg = m.Reg('conf_reg',EmbeddedCode(ADDR_WIDTH + QTD_WIDTH + CONF_ID_QUEUE_WIDTH))
    addr_base = m.Reg('addr_base',ADDR_WIDTH)
    addr_write_next = m.Reg('addr_write_next',ADDR_WIDTH)
    qtd_data_cl = m.Reg('qtd_data_cl',QTD_WIDTH)
    count_req_cl = m.Reg('count_req_cl',QTD_WIDTH)
    count_cl = m.Reg('count_cl',QTD_WIDTH)
    write_peding = m.Reg('write_peding',QTD_WIDTH)
    flag_addr_init = m.Reg('flag_addr_init')
    fifo_re = m.Reg('fifo_re')
    issue_req_data = m.Wire('issue_req_data')
    fifo_empty = m.Wire('fifo_empty')
    fifo_count = m.Wire('fifo_count',FIFO_DEPTH_BITS+1)
    fifo_full = m.Wire('fifo_full')
    fifo_almostfull = m.Wire('fifo_almostfull')
    fifo_almostempty = m.Wire('fifo_almostempty')
    write_data_valid_queue = m.Wire('write_data_valid_queue')
    fifo_dout_valid = m.Wire('fifo_dout_valid')
    fifo_dout = m.Wire('fifo_dout',DATA_WIDTH)
    end_req_wr_data = m.Wire('end_req_rd_data')
    fifo = make_fifo()
    params = [('FIFO_WIDTH',DATA_WIDTH), ('FIFO_DEPTH_BITS',FIFO_DEPTH_BITS),
              ('FIFO_ALMOSTFULL_THRESHOLD',FIFO_FULL-4), ('FIFO_ALMOSTEMPTY_THRESHOLD',2)]
    con = [('clk',clk), ('rst',rst),('we',afu_user_request_write),('din',afu_user_write_data), ('re',fifo_re),
           ('valid',fifo_dout_valid),('dout',fifo_dout), ('count',fifo_count),
           ('empty',fifo_empty), ('full', fifo_full),('almostfull',fifo_almostfull) , ('almostempty',fifo_almostempty)]
    m.Instance(fifo,'fifo',params,con)

    end_req_wr_data.assign((count_req_cl >= qtd_data_cl))
    done.assign(AndList((count_cl>=qtd_data_cl),(write_peding == 0),start))
    issue_req_data.assign(AndList(start & conf_ready & ~end_req_wr_data & available_write,Mux(fifo_almostempty,(~fifo_empty&~fifo_re),Int(1,1,2))))
    afu_user_available_write.assign(~fifo_almostfull)
    write_data_valid_queue.assign(AndList(write_data_valid,(write_queue_id == ID_QUEUE)))

    m.Always(Posedge(clk))(
        If(rst)(
            addr_base(0),
            qtd_data_cl(0),
            conf_reg(0),
            conf_read(Int(0,1,2)),
            conf_ready(Int(0,1,2)),
        ).Else(
            If(conf_valid == Int(2,2,10))(
                conf_reg(conf),
                conf_read(Int(1,1,2))
            ),
            If(AndList(conf_reg[0:CONF_ID_QUEUE_WIDTH] == ID_QUEUE ,conf_read))(
                qtd_data_cl(conf_reg[CONF_ID_QUEUE_WIDTH:CONF_ID_QUEUE_WIDTH + QTD_WIDTH]),
                addr_base(conf_reg[CONF_ID_QUEUE_WIDTH + QTD_WIDTH:CONF_ID_QUEUE_WIDTH + QTD_WIDTH + ADDR_WIDTH]),
                conf_read(Int(0, 1, 2)),
                conf_ready(Int(1, 1, 2))
            )
        )
    )

    m.Always(Posedge(clk))(
        If(rst)(
            addr_write_next(0),
            count_req_cl(0),
            flag_addr_init(Int(1,1,2))
        ).Else(
            If(conf_ready & flag_addr_init)(
                addr_write_next(addr_base),
                flag_addr_init(Int(0, 1, 2))
            ).Elif(fifo_dout_valid)(
                addr_write_next.inc(),
                count_req_cl.inc()
            )
        )
    )
   
    m.Always(Posedge(clk))(
        If(rst)(
            count_cl(0),
        ).Else(
             If(write_data_valid_queue)(
                count_cl.inc()
            )
        )
    )
   
    m.Always(Posedge(clk))(
        If(rst)(
          fifo_re(Int(0,1,2))
        ).Else(
          fifo_re(Int(0, 1, 2)),
          If(issue_req_data)(
              fifo_re(Int(1, 1, 2)),
          )
        )
    )
    m.Always(Posedge(clk))(
        If(rst)(
            write_peding(0)
        ).Else(
            Case(Cat(write_data_valid_queue, request_write))(
                When(Int(0, 2, 10))(
                    write_peding(write_peding)
                ),
                When(Int(1, 2, 10))(
                    write_peding.inc()
                ),
                When(Int(2, 2, 10))(
                    write_peding.dec()
                ),
                When(Int(3, 2, 10))(
                    write_peding(write_peding)
                )
            )
        )
    )

    m.Always(Posedge(clk))(
        If(rst)(
            request_write(Int(0, 1, 2)),
            write_data(0)
        ).Else(
            request_write(Int(0, 1, 2)),
            If(fifo_dout_valid)(
                request_write(Int(1, 1, 2)),
                write_data(Cat(fifo_dout,addr_write_next,ID_QUEUE[0:TAG_WIDTH]))
            )
        )
    )

    return m

#make_output_queue_controller().to_verilog('output_queue_controller_test')
