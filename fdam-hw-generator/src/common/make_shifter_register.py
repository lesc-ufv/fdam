from veriloggen import *


def make_shifter_register():
    m = Module('shifter_register')
    DATA_WIDTH = m.Parameter('DATA_WIDTH', 8)
    NUMREGS = m.Parameter('NUMREGS', 8)
    clk = m.Input('clk')

    din = m.Input('din', DATA_WIDTH)
    dout = m.Output('dout', DATA_WIDTH)
    doutfull = m.Output('doutfull', DATA_WIDTH * NUMREGS)
    shift_reg = m.Reg('shift_reg', DATA_WIDTH, NUMREGS)
    g = m.Genvar('g')
    i = m.Integer('i')

    m.Initial(
        For(i(0), i < NUMREGS, i.inc())(
            shift_reg[i](0)
        )
    )

    m.Always(Posedge(clk))(
        shift_reg[NUMREGS - 1](din),
        For(i(NUMREGS - 2), i >= 0, i.dec())(
            shift_reg[i](shift_reg[i + 1])
        )
    )

    dout.assign(shift_reg[0])

    m.GenerateFor(g(0), g < NUMREGS, g.inc(), 'gen_outfull').Assign(
        doutfull[Mul(g, DATA_WIDTH):Mul((g + 1), DATA_WIDTH)](shift_reg[g])
    )

    return m
