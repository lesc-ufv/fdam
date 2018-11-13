from veriloggen import *


def make_memory(cgra_id):
    m = Module('cgra%d_memory'%cgra_id)
    data_width = m.Parameter('DATA_WIDTH', 32)
    addr_width = m.Parameter('ADDR_WIDTH', 8)
    clk = m.Input('clk')
    we = m.Input('we')
    re = m.Input('re')
    raddr = m.Input('raddr', addr_width)
    waddr = m.Input('waddr', addr_width)
    din = m.Input('din', data_width)
    dout = m.OutputReg('dout', data_width)

    mem = m.Reg('mem', data_width, Power(2, addr_width))

    i = m.Integer('i')
    m.Initial(
        dout(0),
        For(i(0), i < Power(2, addr_width), i.inc())(
            mem[i](0)
        )
    )

    m.Always(Posedge(clk))(
        If(we)(
            mem[waddr](din)
        ),
        If(re)(
            dout(mem[raddr])
        )
    )

    return m
