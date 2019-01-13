from math import ceil, log

from veriloggen import *


def make_swicth_conf_reader(num_thread, swicth_conf_width, conf_net_depth, is_net_branch):
    name = 'swicth_conf_reader_%d_%d' % (swicth_conf_width, conf_net_depth)
    if(is_net_branch):
        name = 'swicth_conf_reader_branch_%d_%d' % (swicth_conf_width, conf_net_depth)

    m = Module(name)
    SWICTH_NUMBER = m.Parameter('SWICTH_NUMBER', 1)
    clk = m.Input('clk')
    rst = m.Input('rst')
    conf_bus_in = m.Input('conf_bus_in', 64)

    pc_max = m.OutputReg('pc_max', conf_net_depth)
    pc_max_we = m.OutputReg('pc_max_we')
    pc_loop = m.OutputReg('pc_loop', conf_net_depth)
    pc_loop_we = m.OutputReg('pc_loop_we')
    thread_id = m.OutputReg('thread_id', int(ceil(log(num_thread, 2))))

    net_mem_we = m.OutputReg('net_mem_we')
    net_mem_waddr = m.OutputReg('net_mem_waddr', conf_net_depth)
    net_mem_data = m.OutputReg('net_mem_data', swicth_conf_width)

    # CGRA_CONF_SET_PE_INSTRUCTION = 1,
    # CGRA_CONF_SET_PE_CONST = 2,
    # CGRA_CONF_SET_PE_PC_MAX= 3,
    # CGRA_CONF_SET_PE_PC_LOOP= 4,
    # CGRA_CONF_SET_PE_IGNORE= 5,
    # CGRA_CONF_SET_PE_QTD_LOW= 6,
    # CGRA_CONF_SET_PE_QTD_HIGH= 7,
    # CGRA_CONF_SET_NET_PC_MAX= 8,
    # CGRA_CONF_SET_NET_PC_LOOP= 9,
    # CGRA_CONF_NET_SWITCH=10,
    # CGRA_CONF_SET_NET_BRANCH_PC_MAX=11,
    # CGRA_CONF_SET_NET_BRANCH_PC_LOOP=12,
    # CGRA_CONF_NET_BRANCH_SWITCH=13

    if is_net_branch:
        CGRA_CONF_SET_NET_PC_MAX = m.Localparam('CGRA_CONF_SET_NET_BRANCH_PC_MAX', 11)
        CGRA_CONF_SET_NET_PC_LOOP = m.Localparam('CGRA_CONF_SET_NET_BRANCH_PC_LOOP', 12)
        CGRA_CONF_NET_SWITCH = m.Localparam('CGRA_CONF_NET_BRANCH_SWITCH', 13)
    else:
        CGRA_CONF_SET_NET_PC_MAX = m.Localparam('CGRA_CONF_SET_NET_PC_MAX', 8)
        CGRA_CONF_SET_NET_PC_LOOP = m.Localparam('CGRA_CONF_SET_NET_PC_LOOP', 9)
        CGRA_CONF_NET_SWITCH = m.Localparam('CGRA_CONF_NET_SWITCH', 10)

    net_conf_type = m.Reg('net_conf_type', 8)
    net_swicth_number = m.Reg('net_swicth_number', 16)
    net_inst_addr = m.Reg('inst_addr', 12)
    net_swicth_conf = m.Reg('net_swicth_conf', 24)
    net_pc_max = m.Reg('net_pc_max', 32)
    net_pc_loop = m.Reg('net_pc_loop', 32)
    net_thread_id = m.Reg('net_thread_id', 4)

    m.Initial(
        net_conf_type(0),
        net_swicth_number(0),
        net_inst_addr(0),
        net_swicth_conf(0),
        net_pc_max(0),
        net_pc_loop(0),
        net_thread_id(0),
        pc_max(0),
        pc_loop(0),
        thread_id(0),
        net_mem_we(0),
        net_mem_waddr(0),
        net_mem_data(0),
        net_thread_id(0),

    )

    m.Always(Posedge(clk))(
        If(rst)(
            net_swicth_number(0),
        ).Else(
            net_conf_type(conf_bus_in[0:8]),
            net_swicth_number(conf_bus_in[8:24]),
            net_thread_id(conf_bus_in[24:28]),
            net_inst_addr(conf_bus_in[28:40]),
            net_swicth_conf(conf_bus_in[40:64]),
            net_pc_max(conf_bus_in[32:64]),
            net_pc_loop(conf_bus_in[32:64]),
        )
    )

    m.Always(Posedge(clk))(
        If(rst)(
            pc_max_we(0),
            pc_loop_we(0),
            net_mem_we(0)
        ).Else(
            net_mem_we(0),
            pc_max_we(0),
            pc_loop_we(0),
            If(net_swicth_number == SWICTH_NUMBER)(
                Case(net_conf_type)(
                    When(CGRA_CONF_SET_NET_PC_MAX)(
                        pc_max_we(1),
                    ),
                    When(CGRA_CONF_SET_NET_PC_LOOP)(
                        pc_loop_we(1),
                    ),
                    When(CGRA_CONF_NET_SWITCH)(
                        net_mem_we(1),
                    ),
                    When()()
                )
            )
        )
    )

    m.Always(Posedge(clk))(
        thread_id(net_thread_id[0:thread_id.width]),
        net_mem_waddr(net_inst_addr[0:net_mem_waddr.width]),
        net_mem_data(net_swicth_conf[0:net_mem_data.width]),
        pc_loop(net_pc_loop[0:pc_loop.width]),
        pc_max(net_pc_max[0:pc_max.width])
    )

    return m
