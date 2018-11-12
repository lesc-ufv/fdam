from veriloggen import *


def make_conf_reader_pc_net(conf_depth):
    m = Module('cgra_conf_reader_pc_net')

    PE_ID = m.Parameter('PE_ID', 0)

    clk = m.Input('clk')
    rst = m.Input('rst')
    conf_bus_in = m.Input('conf_bus_in', 60)

    pc_max = m.OutputReg('pc_max', conf_depth)
    pc_loop = m.OutputReg('pc_loop', conf_depth)

    SET_PC_MAX = m.Localparam('SET_PC_MAX', 2)
    SET_LOOP = m.Localparam('SET_LOOP', 3)

    op_conf = m.Reg('op_conf', 4)
    id_conf = m.Reg('id_conf', 16)
    conf_data = m.Reg('conf_data', 40)

    m.Initial(
        pc_max(0),
        pc_loop(0),

        op_conf(0),
        id_conf(0),
        conf_data(0),
    )

    m.Always(Posedge(clk))(
        If(rst)(
            op_conf(0),
            id_conf(0),
            conf_data(0),
        ).Else(
            op_conf(conf_bus_in[0:4]),
            id_conf(conf_bus_in[8:24]),
            conf_data(conf_bus_in[24:])
        )
    )

    m.Always(Posedge(clk))(
        If(rst)(
            pc_max(0),
            pc_loop(0),
        ).Else(
            If(PE_ID == id_conf)(
                Case(op_conf)(
                    When(SET_PC_MAX)(
                        pc_max(conf_data[0:conf_depth + 1]),
                    ),
                    When(SET_LOOP)(
                        pc_loop(conf_data[0:conf_depth + 1])
                    ),
                    When()(),
                ),
            )
        )
    )

    return m
