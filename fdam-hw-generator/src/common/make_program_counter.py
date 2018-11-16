from veriloggen import *


def make_program_counter(pc_depth):
    m = Module('program_counter_%d'%pc_depth)
    clk = m.Input('clk')
    rst = m.Input('rst')
    en = m.Input('en')
    max = m.Input('max', pc_depth)
    loop = m.Input('loop', pc_depth)
    pc = m.OutputReg('pc', pc_depth)
    pc_next = m.Reg('pc_next', pc_depth)

    m.Always(Posedge(clk))(
        If(rst)(
            pc_next(0),
            pc(0),
        ).Elif(en)(
            If(pc_next < max)(
                pc(pc_next),
                pc_next(pc_next + 1)
            ).Else(
                pc(pc_next),
                pc_next(loop)
            )
        )
    )

    return m
