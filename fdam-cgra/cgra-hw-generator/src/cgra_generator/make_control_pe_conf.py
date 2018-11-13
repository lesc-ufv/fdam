from veriloggen import *


# pe_conf  packt = {op_conf,id_pe,conf_data}

def make_control_pe_conf(cgra_id, num_pe_io):
    m = Module('cgra%d_pe_conf_control'%cgra_id)

    clk = m.Input('clk')
    rst = m.Input('rst')
    start = m.Input('start')

    available_read = m.Input('available_read')
    req_rd_data = m.Output('req_rd_data')
    rd_data = m.Input('rd_data', 512)
    rd_data_valid = m.Input('rd_data_valid')

    conf_pe_out_bus = m.OutputReg('conf_pe_out_bus', 60)

    qtd_net_conf = m.OutputReg('qtd_net_conf', 32)
    read_fifo_mask = m.OutputReg('read_fifo_mask', num_pe_io)
    write_fifo_mask = m.OutputReg('write_fifo_mask', num_pe_io)

    done = m.OutputReg('done')

    FSM_INIT_CTRL_IDLE = m.Localparam('FSM_INIT_CTRL_IDLE', 0)
    FSM_INIT_CTRL_INIT = m.Localparam('FSM_INIT_CTRL_INIT', 1)
    FSM_SEND_INIT_CONF_PE = m.Localparam('FSM_SEND_INIT_CONF_PE', 2)
    FSM_INIT_CTRL_WAIT_DATA = m.Localparam('FSM_INIT_CTRL_WAIT_DATA', 3)
    FSM_INIT_CTRL_REQ_DATA = m.Localparam('FSM_INIT_CTRL_REQ_DATA', 4)
    FSM_INIT_CONF_DONE = m.Localparam('FSM_INIT_CONF_DONE', 5)
    m.EmbeddedCode('')
    fsm_init_conf_ctrl = m.Reg('fsm_init_conf_ctrl', 3)
    fsm_init_conf_ctrl_next = m.Reg('fsm_init_conf_ctrl_next', 3)

    init_conf_req_data = m.Reg('init_conf_req_data')
    init_conf_cl = m.Reg('init_conf_cl', 512)
    qtd_pe_conf = m.Reg('qtd_pe_conf', 32)

    init_conf_op = m.Reg('init_conf_op', 4)
    init_conf_id = m.Reg('init_conf_id', 16)
    init_conf_data = m.Reg('init_conf_data', 40)

    send_init_conf = m.Reg('send_init_conf')
    conf_init_counter = m.Reg('conf_init_counter', 32)
    conf_init_counter_cl = m.Reg('conf_init_counter_cl', 4)

    m.EmbeddedCode('')
    req_rd_data.assign(init_conf_req_data)

    m.Always(Posedge(clk))(
        If(rst)(
            fsm_init_conf_ctrl(FSM_INIT_CTRL_IDLE),
            fsm_init_conf_ctrl_next(FSM_INIT_CTRL_IDLE),
            init_conf_req_data(0),
            init_conf_id(0),
            init_conf_op(0),
            init_conf_data(0),
            qtd_pe_conf(0),
            qtd_net_conf(0),
            send_init_conf(0),
            conf_init_counter(0),
            conf_init_counter_cl(Int(8, conf_init_counter_cl.width, 10)),
            done(0),
            read_fifo_mask(0),
            write_fifo_mask(0)
        ).Else(
            init_conf_req_data(0),
            send_init_conf(0),
            Case(fsm_init_conf_ctrl)(
                When(FSM_INIT_CTRL_IDLE)(
                    If(start)(
                        fsm_init_conf_ctrl(FSM_INIT_CTRL_REQ_DATA),
                        fsm_init_conf_ctrl_next(FSM_INIT_CTRL_INIT)
                    )
                ),
                When(FSM_INIT_CTRL_INIT)(
                    qtd_pe_conf(init_conf_cl[0:32]),
                    qtd_net_conf(init_conf_cl[32:64]),
                    read_fifo_mask(init_conf_cl[64:64 + num_pe_io]),
                    write_fifo_mask(init_conf_cl[96:96 + num_pe_io]),
                    fsm_init_conf_ctrl(FSM_SEND_INIT_CONF_PE),
                ),
                When(FSM_SEND_INIT_CONF_PE)(
                    If(conf_init_counter >= qtd_pe_conf)(
                        fsm_init_conf_ctrl(FSM_INIT_CONF_DONE)
                    ).Else(
                        If(conf_init_counter_cl < Int(8, conf_init_counter_cl.width, 10))(
                            init_conf_op(init_conf_cl[0:4]),
                            init_conf_id(init_conf_cl[16:32]),

                            init_conf_data(Cat(init_conf_cl[8:16],init_conf_cl[32:64])),

                            init_conf_cl(init_conf_cl[64:]),
                            send_init_conf(1),
                            conf_init_counter.inc(),
                            conf_init_counter_cl.inc(),
                        ).Else(
                            conf_init_counter_cl(Int(0, conf_init_counter_cl.width, 10)),
                            fsm_init_conf_ctrl(FSM_INIT_CTRL_REQ_DATA),
                            fsm_init_conf_ctrl_next(FSM_SEND_INIT_CONF_PE)
                        )
                    )
                ),
                When(FSM_INIT_CTRL_REQ_DATA)(
                    If(available_read)(
                        init_conf_req_data(1),
                        fsm_init_conf_ctrl(FSM_INIT_CTRL_WAIT_DATA)
                    )
                ),
                When(FSM_INIT_CTRL_WAIT_DATA)(
                    If(rd_data_valid)(
                        init_conf_cl(rd_data),
                        fsm_init_conf_ctrl(fsm_init_conf_ctrl_next),
                    )
                ),
                When(FSM_INIT_CONF_DONE)(
                    done(1)
                )
            )
        )
    )

    m.Always(Posedge(clk))(
        If(rst)(
            conf_pe_out_bus(0),
        ).Else(
            If(send_init_conf)(
                conf_pe_out_bus(Cat(init_conf_data, init_conf_id, init_conf_op)),
            ).Else(
                conf_pe_out_bus(0)
            ),
        )
    )

    return m
