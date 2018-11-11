import math

from veriloggen import *


# conf_pe packt = {id,conf_pe}

def make_control_pe_conf(num_pe):
    m = Module('cgra_pe_conf_control')

    conf_id_width = int(math.ceil(math.log(num_pe, 2)) + 1)
    conf_width = 16

    clk = m.Input('clk')
    rst = m.Input('rst')
    start = m.Input('start')
    qtd_conf = m.Input('qtd_conf', 32)

    available_read = m.Input('available_read')
    req_rd_data = m.Output('req_rd_data')
    rd_data = m.Input('rd_data', 512)
    rd_data_valid = m.Input('rd_data_valid')

    conf_pe_out_bus = m.OutputReg('conf_pe_out_bus', conf_id_width + conf_width)
    done = m.OutputReg('done')

    FSM_CONF_PE_CTRL_IDLE = m.Localparam('FSM_CONF_PE_CTRL_IDLE', 0)
    FSM_SEND_CONF_PE = m.Localparam('FSM_SEND_CONF_PE', 1)
    FSM_CONF_PE_CTRL_WAIT_DATA = m.Localparam('FSM_CONF_PE_CTRL_WAIT_DATA', 2)
    FSM_CONF_PE_CTRL_REQ_DATA = m.Localparam('FSM_CONF_PE_CTRL_REQ_DATA', 3)
    FSM_CONF_PE_DONE = m.Localparam('FSM_CONF_PE_DONE', 4)
    m.EmbeddedCode('')
    fsm_conf_ctrl = m.Reg('fsm_conf_ctrl', 3)
    fsm_conf_ctrl_next = m.Reg('fsm_conf_ctrl_next', 3)
    conf_req_data = m.Reg('conf_req_data')
    conf_cl = m.Reg('conf_cl', 512)
    conf_id = m.Reg('conf_id', conf_id_width)
    conf_pe = m.Reg('conf_pe', conf_width)
    send_conf = m.Reg('send_conf')
    conf_counter = m.Reg('conf_counter', 32)
    conf_counter_cl = m.Reg('conf_counter_cl', 5)
    m.EmbeddedCode('')
    req_rd_data.assign(conf_req_data)

    m.Always(Posedge(clk))(
        If(rst)(
            fsm_conf_ctrl(FSM_CONF_PE_CTRL_IDLE),
            fsm_conf_ctrl_next(FSM_CONF_PE_CTRL_IDLE),
            conf_req_data(0),
            send_conf(0),
            conf_counter(0),
            conf_counter_cl(Int(16, conf_counter_cl.width, 10)),
            done(0),
        ).Else(
            conf_req_data(0),
            send_conf(0),
            Case(fsm_conf_ctrl)(
                When(FSM_CONF_PE_CTRL_IDLE)(
                    If(start)(
                        fsm_conf_ctrl(FSM_SEND_CONF_PE),
                    )
                ),
                When(FSM_SEND_CONF_PE)(
                    If(conf_counter >= qtd_conf)(
                        fsm_conf_ctrl(FSM_CONF_PE_DONE),
                    ).Else(
                        If(conf_counter_cl < Int(16, conf_counter_cl.width, 10))(
                            conf_id(conf_cl[0:conf_id_width]),
                            conf_pe(conf_cl[16:16 + conf_width]),
                            conf_cl(conf_cl[16 + conf_width:]),
                            send_conf(1),
                            conf_counter.inc(),
                            conf_counter_cl.inc()
                        ).Else(
                            conf_counter_cl(Int(0, conf_counter_cl.width, 10)),
                            fsm_conf_ctrl(FSM_CONF_PE_CTRL_REQ_DATA),
                            fsm_conf_ctrl_next(FSM_SEND_CONF_PE)
                        )
                    )
                ),
                When(FSM_CONF_PE_CTRL_REQ_DATA)(
                    If(available_read)(
                        conf_req_data(1),
                        fsm_conf_ctrl(FSM_CONF_PE_CTRL_WAIT_DATA)
                    )
                ),
                When(FSM_CONF_PE_CTRL_WAIT_DATA)(
                    If(rd_data_valid)(
                        conf_cl(rd_data),
                        fsm_conf_ctrl(fsm_conf_ctrl_next),
                    )
                ),
                When(FSM_CONF_PE_DONE)(
                    done(1)
                ),
            )
        )
    )

    m.Always(Posedge(clk))(
        If(rst)(
            conf_pe_out_bus(0),
        ).Else(
            If(send_conf)(
                conf_pe_out_bus(Cat(conf_pe, conf_id))
            ).Else(
                conf_pe_out_bus(0)
            )
        )
    )

    return m
