from veriloggen import *


def make_program_counter(pc_depth):
    m = Module('cgra_program_counter')
    clk = m.Input('clk')
    rst = m.Input('rst')
    en = m.Input('en')
    max = m.Input('max', pc_depth)
    loop = m.Input('loop', pc_depth)
    pc = m.OutputReg('pc', pc_depth)
    pc_en = m.OutputReg('pc_en')

    pc_next = m.Reg('pc_next', pc_depth)
    pc_en_next = m.Reg('pc_en_next')

    m.Always(Posedge(clk))(
        If(rst)(
            pc_next(0),
            pc(0),
            pc_en(0),
            pc_en_next(0)
        ).Elif(en)(
            pc_en_next(1),
            pc_en(pc_en_next),
            If(pc_next < max)(
                pc(pc_next),
                pc_next(pc_next + 1)
            ).Else(
                pc(pc_next),
                pc_next(loop)
            )
        ).Else(
            pc_en(0)
        )
    )

    return m
