from math import ceil, log

from veriloggen import Module

from common.make_mux import make_mux


def make_swicth_box(num_in, num_out):
    m = Module('cgra_switch_box%dx%d' % (num_in, num_out))
    WIDTH = m.Parameter('WIDTH', 16)

    ports_in = []
    ports_out = []

    bits = int(ceil(log(num_in, 2)) * num_out)
    step = int(ceil(log(num_in, 2)))

    m.Input('sel', bits)

    for i in range(num_in):
        ports_in.append(m.Input('in%d' % i, WIDTH))

    for i in range(num_out):
        ports_out.append(m.Output('out%d' % i, WIDTH))

    mux = make_mux(num_in)
    ports_cross = m.get_ports()
    for i in range(num_out):
        p = []
        for j in range(num_in):
            p.append(('in%d' % j, ports_cross['in%d' % j]))
        start = -(step * num_out) + (i * step)
        stop = -(step * num_out - 1) + (i * step) + step - 1
        if start != 0 and stop != 0:
            p.append(('sel', ports_cross['sel'][start:stop]))
        elif start == 0:
            p.append(('sel', ports_cross['sel'][:stop]))
        else:
            p.append(('sel', ports_cross['sel'][start:]))

        p.append(('out', ports_cross['out%d' % i]))
        m.Instance(mux, ('mux%d' % i), params=m.connect_params(mux), ports=p)

    return m
