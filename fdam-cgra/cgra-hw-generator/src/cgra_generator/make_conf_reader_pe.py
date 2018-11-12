from veriloggen import *


def make_conf_reader_pe(data_width, conf_width, conf_depth):
    m = Module('cgra_conf_reader_pe')

    PE_ID = m.Parameter('PE_ID', 0)

    clk = m.Input('clk')
    rst = m.Input('rst')

    conf_bus_in = m.Input('conf_bus_in', 60)

    conf_wr_en = m.OutputReg('conf_wr_en')
    conf_wr_addr = m.OutputReg('conf_wr_addr', conf_depth)
    conf_wr_data = m.OutputReg('conf_wr_data', conf_width)

    init_const_we = m.OutputReg('init_const_we')
    init_const_waddr = m.OutputReg('init_const_waddr', 3)
    init_const = m.OutputReg('init_const', data_width)

    pc_max = m.OutputReg('pc_max', conf_depth)
    pc_loop = m.OutputReg('pc_loop', conf_depth)
    ignore_until = m.OutputReg('ignore_until', conf_depth)

    SET_INSTRUCTION = m.Localparam('SET_INSTRUCTION', 0)
    SET_CONST = m.Localparam('SET_CONST', 1)
    SET_PC_MAX = m.Localparam('SET_PC_MAX', 2)
    SET_LOOP = m.Localparam('SET_LOOP', 3)
    SET_IGNORE = m.Localparam('SET_IGNORE', 4)

    op_conf = m.Reg('op_conf', 4)
    id_conf = m.Reg('id_conf', 16)
    conf_data = m.Reg('conf_data', 40)

    m.Initial(
        init_const_we(0),
        init_const_waddr(0),
        init_const(0),
        pc_max(0),
        pc_loop(0),
        ignore_until(0),
        op_conf(0),
        id_conf(0),
        conf_data(0),
        conf_wr_en(0),
        conf_wr_addr(0),
        conf_wr_data(0)
    )

    m.Always(Posedge(clk))(
        If(rst)(
            op_conf(0),
            id_conf(0),
            conf_data(0)
        ).Else(
            op_conf(conf_bus_in[0:4]),
            id_conf(conf_bus_in[4:20]),
            conf_data(conf_bus_in[20:])
        )

    )

    m.Always(Posedge(clk))(
        If(rst)(
            conf_wr_en(0),
            init_const_we(0),
            pc_max(0),
            pc_loop(0),
            ignore_until(0),
        ).Else(
            conf_wr_en(0),
            init_const_we(0),
            If(id_conf == PE_ID)(
                Case(op_conf)(
                    When(SET_INSTRUCTION)(
                        conf_wr_en(1),
                        conf_wr_addr(conf_data[0:conf_depth]),
                        conf_wr_data(conf_data[16:])
                    ),
                    When(SET_CONST)(
                        init_const_we(1),
                        init_const_waddr(conf_data[32:32+init_const_waddr.width]),
                        init_const(conf_data[0:data_width]),
                    ),
                    When(SET_PC_MAX)(
                        pc_max(conf_data[0:conf_depth + 1]),
                    ),
                    When(SET_LOOP)(
                        pc_loop(conf_data[0:conf_depth + 1])
                    ),
                    When(SET_IGNORE)(
                        ignore_until(conf_data[0:conf_depth + 1])
                    ),
                    When()(),
                )
            )
        )
    )

    return m