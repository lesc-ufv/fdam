from math import log, ceil

from veriloggen import *


def make_mux(n):
    m = Module('cgra_mux%dx1' % n)
    WIDTH = m.Parameter('WIDTH', 16)
    s_bits = int(ceil(log(n, 2)))
    s = m.Input('sel', s_bits)
    ports_in = [m.Input('in%d' % i, WIDTH) for i in range(n)]
    out_in = m.Output('out', WIDTH)
    wr = m.Wire('ins', WIDTH, n)
    for i in range(len(ports_in)):
        m.Assign(wr[i](ports_in[i]))

    m.Assign(out_in(wr[s]))

    return m
