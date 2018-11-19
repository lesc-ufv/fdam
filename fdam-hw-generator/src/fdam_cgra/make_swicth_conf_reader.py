from veriloggen import *


def make_swicth_conf_reader(swicth_conf_width, conf_net_depth):
    m = Module('swicth_conf_reader_%d_%d' %(swicth_conf_width,conf_net_depth) )
    SWICTH_NUMBER = m.Parameter('SWICTH_NUMBER', 1)

    clk = m.Input('clk')
    rst = m.Input('rst')
    conf_bus_in = m.Input('conf_bus_in', 64)

    pc_max = m.OutputReg('pc_max', conf_net_depth)
    pc_loop = m.OutputReg('pc_loop', conf_net_depth)

    net_mem_we = m.OutputReg('net_mem_we')
    net_mem_waddr = m.OutputReg('net_mem_waddr', conf_net_depth)
    net_mem_data = m.OutputReg('net_mem_data', swicth_conf_width)

    CGRA_NOT_CONF = m.Localparam('CGRA_NOT_CONF', 0)
    CGRA_CONF_SET_PE_INSTRUCTION = m.Localparam('CGRA_CONF_SET_PE_INSTRUCTION', 1)
    CGRA_CONF_SET_PE_CONST = m.Localparam('CGRA_CONF_SET_PE_CONST', 2)
    CGRA_CONF_SET_PE_PC_MAX = m.Localparam('CGRA_CONF_SET_PE_PC_MAX', 3)
    CGRA_CONF_SET_PE_PC_LOOP = m.Localparam('CGRA_CONF_SET_PE_PC_LOOP', 4)
    CGRA_CONF_SET_PE_STORE_IGNORE = m.Localparam('CGRA_CONF_SET_PE_STORE_IGNORE', 5)
    CGRA_CONF_SET_NET_PC_MAX = m.Localparam('CGRA_CONF_SET_NET_PC_MAX', 6)
    CGRA_CONF_SET_NET_PC_LOOP = m.Localparam('CGRA_CONF_SET_NET_PC_LOOP', 7)
    CGRA_CONF_NET_SWITCH = m.Localparam('CGRA_CONF_NET_SWITCH', 8)

    net_conf_type = m.Reg('net_conf_type', 8)
    net_swicth_number = m.Reg('net_swicth_number', 16)
    net_inst_addr = m.Reg('inst_addr', 16)
    net_swicth_conf = m.Reg('net_swicth_conf', 24)
    net_pc_max = m.Reg('net_pc_max', 32)
    net_pc_loop = m.Reg('net_pc_loop', 32)

    net_mem_we_p = m.Reg('net_mem_we_p')
    net_mem_waddr_p = m.Reg('net_mem_waddr_p', conf_net_depth)
    net_mem_data_p = m.Reg('net_mem_data_p', swicth_conf_width)

    m.Initial(
        net_conf_type(0),
        net_swicth_number(0),
        net_inst_addr(0),
        net_swicth_conf(0),
        net_pc_max(0),
        net_pc_loop(0),
        pc_max(0),
        pc_loop(0),
        net_mem_we(0),
        net_mem_waddr(0),
        net_mem_data(0),
        net_mem_we_p(0),
        net_mem_waddr_p(0),
        net_mem_data_p(0)
    )

    m.Always(Posedge(clk))(
        If(rst)(
            net_swicth_number(0),
        ).Else(
            net_conf_type(conf_bus_in[0:8]),
            net_swicth_number(conf_bus_in[8:24]),
            net_inst_addr(conf_bus_in[24:40]),
            net_swicth_conf(conf_bus_in[40:56]),
            net_pc_max(conf_bus_in[24:56]),
            net_pc_loop(conf_bus_in[24:56]),
        )
    )

    m.Always(Posedge(clk))(
        If(rst)(
            pc_max(0),
            pc_loop(0),
            net_mem_we_p(0)
        ).Else(
            net_mem_we_p(0),
            If(net_swicth_number == SWICTH_NUMBER)(
                Case(net_conf_type)(
                    When(CGRA_NOT_CONF)(),
                    When(CGRA_CONF_SET_PE_INSTRUCTION)(),
                    When(CGRA_CONF_SET_PE_CONST)(),
                    When(CGRA_CONF_SET_PE_PC_MAX)(),
                    When(CGRA_CONF_SET_PE_PC_LOOP)(),
                    When(CGRA_CONF_SET_PE_STORE_IGNORE)(),
                    When(CGRA_CONF_SET_NET_PC_MAX)(
                        pc_max(net_pc_max[0:pc_max.width])
                    ),
                    When(CGRA_CONF_SET_NET_PC_LOOP)(
                        pc_loop(net_pc_loop[0:pc_loop.width])
                    ),
                    When(CGRA_CONF_NET_SWITCH)(
                        net_mem_we_p(1)
                    ),
                    When()()
                )
            )
        )
    )

    m.Always(Posedge(clk))(
        net_mem_waddr_p(net_inst_addr[0:net_mem_waddr_p.width]),
        net_mem_data_p(net_swicth_conf[0:net_mem_data_p.width]),
        net_mem_we(net_mem_we_p),
        net_mem_waddr(net_mem_waddr_p),
        net_mem_data(net_mem_data_p)
    )

    return m
