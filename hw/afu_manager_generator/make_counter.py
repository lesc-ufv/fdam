from veriloggen import *

from util import make_const


def make_counter():
    m = Module('afu_counter')
    DEPTH_BITS = m.Parameter('DEPTH_BITS', 32)
    clk = m.Input('clk')
    rst = m.Input('rst')
    en = m.Input('en')
    dout = m.OutputReg('dout', DEPTH_BITS)
    m.Always(Posedge(clk))(
        If(rst)(
            dout(make_const(0, DEPTH_BITS))
        ).Elif(en)(
            dout(dout + make_const(1, DEPTH_BITS))
        )
    )
    return m
