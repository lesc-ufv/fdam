from veriloggen import *


def make_alu_out_decode(cgra_id,is_io):
    name = 'cgra%d_alu_out_decode'%(cgra_id)
    n = 3
    if is_io:
        name = name + '_io'
        n = 4
    m = Module(name)
    sel = m.Input('sel', 2)
    out = m.Output('out', n)

    out[0].assign(~sel[1] & ~sel[0])
    out[1].assign(~sel[1] & sel[0])
    out[2].assign(sel[1] & ~sel[0])
    if is_io:
        out[3].assign(sel[1] & sel[0])

    return m
