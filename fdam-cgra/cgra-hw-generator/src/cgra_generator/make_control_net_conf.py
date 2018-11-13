import math

from veriloggen import *

from omega_generator.make_omega import make_omega


# conf_net packt = {conf_net}

def make_control_net_conf(cgra_id, num_pe, net_radix, conf_depth):
    m = Module('cgra%d_net_conf_control'%cgra_id)

    net = make_omega(cgra_id,num_pe * 2, 0, net_radix)
    conf_net_width = net.get_ports().get('conf').width

    num_cl_conf_net = int(math.ceil(conf_net_width / 512))
    num_conf_net_cl = int(math.floor(512 / conf_net_width))

    clk = m.Input('clk')
    rst = m.Input('rst')
    start = m.Input('start')
    qtd_net_conf = m.Input('qtd_net_conf', 32)
    available_read = m.Input('available_read')
    req_rd_data = m.Output('req_rd_data')
    rd_data = m.Input('rd_data', 512)
    rd_data_valid = m.Input('rd_data_valid')

    conf_net_we = m.OutputReg('conf_net_we')
    conf_net_addr = m.OutputReg('conf_net_addr', conf_depth)
    conf_net_data_out = m.OutputReg('conf_net_data_out', conf_net_width)

    done = m.OutputReg('done')

    FSM_CONF_NET_CTRL_IDLE = m.Localparam('FSM_CONF_NET_CTRL_IDLE', 0)
    FSM_SEND_CONF_NET = m.Localparam('FSM_SEND_CONF_NET', 1)
    FSM_CONF_NET_CTRL_WAIT_DATA = m.Localparam('FSM_CONF_NET_CTRL_WAIT_DATA', 2)
    FSM_CONF_NET_CTRL_REQ_DATA = m.Localparam('FSM_CONF_NET_CTRL_REQ_DATA', 3)
    FSM_CONF_NET_DONE = m.Localparam('FSM_CONF_NET_DONE', 4)
    m.EmbeddedCode('')
    fsm_conf_net_ctrl = m.Reg('fsm_conf_net_ctrl', 3)
    fsm_conf_net_ctrl_next = m.Reg('fsm_conf_net_ctrl_next', 3)
    conf_net_req_data = m.Reg('conf_net_req_data')
    conf_net_cl = m.Reg('conf_net_cl', 512)
    conf_net = m.Reg('conf_net', conf_net_width)
    send_conf_net = m.Reg('send_conf_net')
    conf_net_counter = m.Reg('conf_net_counter', 32)
    conf_net_counter_cl = m.Reg('conf_net_counter_cl', 32)
    conf_net_addr_next = m.Reg('conf_net_addr_next', conf_depth)
    m.EmbeddedCode('')
    req_rd_data.assign(conf_net_req_data)

    conf_net_counter_cl_initial = 0
    if num_cl_conf_net == 1 and num_conf_net_cl == 1:
        when_send_conf_net = When(FSM_SEND_CONF_NET)(
            If(conf_net_counter >= qtd_net_conf)(
                fsm_conf_net_ctrl(FSM_CONF_NET_DONE),
            ).Else(
                conf_net(conf_net_cl[0:conf_net_width]),
                send_conf_net(1),
                conf_net_counter.inc(),
                fsm_conf_net_ctrl(FSM_CONF_NET_CTRL_REQ_DATA),
                fsm_conf_net_ctrl_next(FSM_SEND_CONF_NET)
            )
        )
    elif num_cl_conf_net > num_conf_net_cl:
        conf_net_counter_cl_initial = num_cl_conf_net
        when_send_conf_net = When(FSM_SEND_CONF_NET)(
            If(conf_net_counter >= qtd_net_conf)(
                fsm_conf_net_ctrl(FSM_CONF_NET_DONE),
            ).Else(
                If(conf_net_counter_cl < num_cl_conf_net)(
                    conf_net(Or(conf_net << 512, conf_net_cl)),
                    fsm_conf_net_ctrl(FSM_CONF_NET_CTRL_REQ_DATA),
                    fsm_conf_net_ctrl_next(FSM_SEND_CONF_NET),
                    conf_net_counter_cl.inc()
                ).Else(
                    send_conf_net(1),
                    conf_net_counter_cl(0),
                    fsm_conf_net_ctrl(FSM_CONF_NET_CTRL_REQ_DATA),
                    fsm_conf_net_ctrl_next(FSM_SEND_CONF_NET),
                )
            )
        )
    else:
        conf_net_counter_cl_initial = Int(int(math.floor(512 / (conf_net_width))), int(
            math.ceil(math.log(conf_net_width, 2))) + 1, 10)
        when_send_conf_net = When(FSM_SEND_CONF_NET)(
            If(conf_net_counter >= qtd_net_conf)(
                fsm_conf_net_ctrl(FSM_CONF_NET_DONE),
            ).Else(
                If(conf_net_counter_cl < Int(int(math.floor(512 / (conf_net_width))), int(
                    math.ceil(math.log(conf_net_width, 2))) + 1, 10))(
                    conf_net(conf_net_cl[0:conf_net_width]),
                    conf_net_cl(conf_net_cl[conf_net_width:]),
                    send_conf_net(1),
                    conf_net_counter.inc(),
                    conf_net_counter_cl.inc()
                ).Else(
                    conf_net_counter_cl(0),
                    fsm_conf_net_ctrl(FSM_CONF_NET_CTRL_REQ_DATA),
                    fsm_conf_net_ctrl_next(FSM_SEND_CONF_NET)
                )
            )
        )
    m.Always(Posedge(clk))(
        If(rst)(
            fsm_conf_net_ctrl(FSM_CONF_NET_CTRL_IDLE),
            fsm_conf_net_ctrl_next(FSM_CONF_NET_CTRL_IDLE),
            conf_net_req_data(0),
            send_conf_net(0),
            conf_net_counter(0),
            conf_net_counter_cl(conf_net_counter_cl_initial),
            done(0),
        ).Else(
            conf_net_req_data(0),
            send_conf_net(0),
            Case(fsm_conf_net_ctrl)(
                When(FSM_CONF_NET_CTRL_IDLE)(
                    If(start)(
                        fsm_conf_net_ctrl(FSM_SEND_CONF_NET),
                    )
                ),
                when_send_conf_net,
                When(FSM_CONF_NET_CTRL_REQ_DATA)(
                    If(available_read)(
                        conf_net_req_data(1),
                        fsm_conf_net_ctrl(FSM_CONF_NET_CTRL_WAIT_DATA)
                    )
                ),
                When(FSM_CONF_NET_CTRL_WAIT_DATA)(
                    If(rd_data_valid)(
                        conf_net_cl(rd_data),
                        fsm_conf_net_ctrl(fsm_conf_net_ctrl_next),
                    )
                ),
                When(FSM_CONF_NET_DONE)(
                    done(1)
                ),
            )
        )
    )

    m.Always(Posedge(clk))(
        If(rst)(
            conf_net_addr(0),
            conf_net_addr_next(0),
            conf_net_we(0),
        ).Else(
            conf_net_we(0),
            If(send_conf_net)(
                conf_net_addr(conf_net_addr_next),
                conf_net_addr_next.inc(),
                conf_net_we(1),
            )
        ),
        conf_net_data_out(conf_net)
    )

    return m
