from veriloggen import *


def make_conf_reader(conf_id_width, conf_width, conf_depth):
    m = Module('cgra_conf_reader')

    PE_ID = m.Parameter('PE_ID', 0)

    clk = m.Input('clk')
    rst = m.Input('rst')

    conf_in = m.Input('conf_in', conf_width + conf_id_width)
    conf_out = m.OutputReg('conf_out', conf_width + conf_id_width)

    conf_wr_addr = m.OutputReg('conf_wr_addr', conf_depth)
    conf_wr_data = m.OutputReg('conf_wr_data', conf_width)
    conf_wr_en = m.OutputReg('conf_wr_en')

    conf_wr_addr_next = m.Reg('conf_wr_addr_next', conf_depth)

    m.Always(Posedge(clk))(
        If(rst)(
            conf_wr_en(0),
            conf_wr_addr_next(0),
            conf_wr_addr(0)
        ).Else(
            conf_out(conf_in),
            conf_wr_en(0),
            If(conf_out[0:conf_id_width] == PE_ID)(
                conf_wr_addr(conf_wr_addr_next),
                conf_wr_addr_next.inc(),
                conf_wr_en(1),
                conf_wr_data(conf_out[conf_id_width:])
            )
        )
    )

    m.Initial(
        conf_wr_en(0),
        conf_wr_addr_next(0),
        conf_wr_addr(0),
        conf_wr_data(0)
    )

    return m
