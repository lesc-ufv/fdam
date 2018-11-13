from math import ceil, log

from veriloggen import *


def make_demux(cgra_id,n):
    m = Module('cgra%d_demux1x%d' % (cgra_id,n))
    WIDTH = m.Parameter('WIDTH', 16)
    clk = m.Input('clk')
    s_bits = int(ceil(log(n, 2)))
    s = m.Input('sel', s_bits)
    port_in = m.Input('in', WIDTH)
    ports_out = [m.OutputReg('out%d' % i, WIDTH) for i in range(n)]

    case = Case(s)
    for i in range(n):
        case.add(When(i)(ports_out[i](port_in, False)))

    m.Always(Posedge(clk))(
        case
    )
    return m
