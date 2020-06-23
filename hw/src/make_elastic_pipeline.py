from veriloggen import *


def make_elastic_pipeline():
    m = Module('elastic_pipeline')
    DATA_WIDTH = m.Parameter('DATA_WIDTH', 16)
    MAX_LATENCY_BITS = m.Parameter('MAX_LATENCY_BITS', 3)
    clk = m.Input('clk')
    latency = m.Input('latency', MAX_LATENCY_BITS)
    din = m.Input('din', DATA_WIDTH)
    dout = m.Output('dout', DATA_WIDTH)

    shift_reg = m.Reg('shift_reg', DATA_WIDTH, Power(2, MAX_LATENCY_BITS))

    i = m.Integer('i')

    m.Initial(
        For(i(0), i < Power(2, MAX_LATENCY_BITS), i.inc())(
            shift_reg[i](0)
        )
    )

    m.Always(Posedge(clk))(
        shift_reg[0](din),
        For(i(1), i < Power(2, MAX_LATENCY_BITS), i.inc())(
            shift_reg[i](shift_reg[i - 1])
        )
    )

    dout.assign(shift_reg[latency])

    return m
