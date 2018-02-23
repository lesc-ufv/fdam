from veriloggen import *


def make_conf_receiver():
    m = Module('conf_receiver')
    CONF_TYPE = m.Parameter('CONF_TYPE', 1)
    CONF_ID = m.Parameter('CONF_ID', 0)
    CONF_ID_WIDTH = m.Parameter('CONF_ID_WIDTH', 16)
    CONF_WIDTH = m.Parameter('CONF_WIDTH', 128)
    clk = m.Input('clk')
    rst = m.Input('rst')
    conf_in_valid = m.Input('conf_in_valid', 2)
    conf_in_data = m.Input('conf_in_data', CONF_WIDTH)
    conf_out_valid = m.OutputReg('conf_out_valid')
    conf_out_data = m.OutputReg('conf_out_data', CONF_WIDTH)

    m.Always(Posedge(clk))(
        If(rst)(
            conf_out_valid(0),
            conf_out_data(0)
        ).Else(
            conf_out_valid(Int(0, 1, 2)),
            If(AndList(conf_in_valid == CONF_TYPE, conf_in_data[0:CONF_ID_WIDTH] == CONF_ID))(
                conf_out_data(conf_in_data),
                conf_out_valid(Int(1, 1, 2))
            )
        )
    )

    return m

#make_conf_receiver().to_verilog('make_conf_receiver_test')