from veriloggen import *


def make_counter():
    m = Module('cgra_counter')
    WIDTH = m.Parameter('WIDTH', 32)
    clk = m.Input('clk')
    rst = m.Input('rst')
    en = m.Input('en')
    out = m.Output('out', WIDTH)
    limit = m.Input('limit', WIDTH)
    end_counter = m.OutputReg('end_counter')
    count = m.Reg('count', WIDTH)
    out.assign(count)
    m.Always(Posedge(clk))(
        If(rst)(
            count(0),
            end_counter(0)
        ).Elif(en)(
            count.inc(),
            If(count >= limit)(
                end_counter(Int(1, 1, 2))
            )
        )
    )

    return m
