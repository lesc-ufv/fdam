from math import ceil

from veriloggen import *
from veriloggen.types.util import log2


# pe_conf  packt = {op_conf,id_pe,conf_data}

def make_control_conf(cgra_id, num_pe_io_in, num_pe_io_out, num_cicle_wait_conf_finish):
    m = Module('cgra%d_control_conf' % cgra_id)

    clk = m.Input('clk')
    rst = m.Input('rst')
    start = m.Input('start')

    available_read = m.Input('available_read')
    req_rd_data = m.Output('req_rd_data')
    rd_data = m.Input('rd_data', 512)
    rd_data_valid = m.Input('rd_data_valid')

    conf_out_bus = m.OutputReg('conf_out_bus', 64)

    read_fifo_mask = m.OutputReg('read_fifo_mask', num_pe_io_in)
    write_fifo_mask = m.OutputReg('write_fifo_mask', num_pe_io_out)

    done = m.OutputReg('done')

    FSM_INIT_CTRL_IDLE = m.Localparam('FSM_INIT_CTRL_IDLE', 0)
    FSM_INIT_CTRL_INIT = m.Localparam('FSM_INIT_CTRL_INIT', 1)
    FSM_SEND_INIT_CONF_PE = m.Localparam('FSM_SEND_INIT_CONF_PE', 2)
    FSM_INIT_CTRL_WAIT_DATA = m.Localparam('FSM_INIT_CTRL_WAIT_DATA', 3)
    FSM_INIT_CTRL_REQ_DATA = m.Localparam('FSM_INIT_CTRL_REQ_DATA', 4)
    FSM_INIT_CONF_DONE = m.Localparam('FSM_INIT_CONF_DONE', 5)
    FSM_WAIT_ALL_CONF_FINISH = m.Localparam('FSM_WAIT_ALL_CONF_FINISH', 6)

    m.EmbeddedCode('')
    fsm_conf_ctrl = m.Reg('fsm_conf_ctrl', 3)
    fsm_conf_ctrl_next = m.Reg('fsm_conf_ctrl_next', 3)
    conf_req_data = m.Reg('conf_req_data')
    conf_cl = m.Reg('conf_cl', 512)
    qtd_conf = m.Reg('qtd_conf', 32)
    conf_data = m.Reg('conf_data', 64)
    send_conf = m.Reg('send_conf')
    conf_counter = m.Reg('conf_counter', 32)
    conf_counter_cl = m.Reg('conf_counter_cl', 4)
    wait_counter = m.Reg('wait_counter', int(ceil(log2(num_cicle_wait_conf_finish))) + 1)

    m.EmbeddedCode('')

    m.Initial(
        fsm_conf_ctrl(FSM_INIT_CTRL_IDLE),
        fsm_conf_ctrl_next(FSM_INIT_CTRL_IDLE),
        conf_req_data(0),
        send_conf(0),
        conf_counter(0),
        conf_counter_cl(Int(8, conf_counter_cl.width, 10)),
        done(0),
        read_fifo_mask(0),
        write_fifo_mask(0),
        wait_counter(0),
        conf_out_bus(0)

    )

    req_rd_data.assign(conf_req_data)

    m.Always(Posedge(clk))(
        If(rst)(
            fsm_conf_ctrl(FSM_INIT_CTRL_IDLE),
            fsm_conf_ctrl_next(FSM_INIT_CTRL_IDLE),
            conf_req_data(0),
            send_conf(0),
            conf_counter(0),
            conf_counter_cl(Int(8, conf_counter_cl.width, 10)),
            done(0),
            read_fifo_mask(0),
            write_fifo_mask(0),
            wait_counter(0)
        ).Else(
            conf_req_data(0),
            send_conf(0),
            Case(fsm_conf_ctrl)(
                When(FSM_INIT_CTRL_IDLE)(
                    If(start)(
                        fsm_conf_ctrl(FSM_INIT_CTRL_REQ_DATA),
                        fsm_conf_ctrl_next(FSM_INIT_CTRL_INIT)
                    )
                ),
                When(FSM_INIT_CTRL_INIT)(
                    qtd_conf(conf_cl[0:32]),
                    read_fifo_mask(conf_cl[32:32 + num_pe_io_in]),
                    write_fifo_mask(conf_cl[64:64 + num_pe_io_out]),
                    fsm_conf_ctrl(FSM_SEND_INIT_CONF_PE),
                ),
                When(FSM_SEND_INIT_CONF_PE)(
                    If(conf_counter >= qtd_conf)(
                        fsm_conf_ctrl(FSM_WAIT_ALL_CONF_FINISH)
                    ).Else(
                        If(conf_counter_cl < Int(8, conf_counter_cl.width, 10))(
                            conf_data(conf_cl[0:64]),
                            conf_cl(conf_cl[64:]),
                            send_conf(1),
                            conf_counter.inc(),
                            conf_counter_cl.inc(),
                        ).Else(
                            conf_counter_cl(Int(0, conf_counter_cl.width, 10)),
                            fsm_conf_ctrl(FSM_INIT_CTRL_REQ_DATA),
                            fsm_conf_ctrl_next(FSM_SEND_INIT_CONF_PE)
                        )
                    )
                ),
                When(FSM_INIT_CTRL_REQ_DATA)(
                    If(available_read)(
                        conf_req_data(1),
                        fsm_conf_ctrl(FSM_INIT_CTRL_WAIT_DATA)
                    )
                ),
                When(FSM_INIT_CTRL_WAIT_DATA)(
                    If(rd_data_valid)(
                        conf_cl(rd_data),
                        fsm_conf_ctrl(fsm_conf_ctrl_next),
                    )
                ),
                When(FSM_WAIT_ALL_CONF_FINISH)(
                    wait_counter.inc(),
                    If(wait_counter > num_cicle_wait_conf_finish)(
                        fsm_conf_ctrl(FSM_INIT_CONF_DONE)
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
            conf_out_bus(0),
        ).Else(
            If(send_conf)(
                conf_out_bus(conf_data),
            ).Else(
                conf_out_bus(0)
            ),
        )
    )

    return m
