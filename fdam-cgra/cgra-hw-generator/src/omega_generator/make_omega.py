from math import log, ceil

from veriloggen import *

from common.cgra_utils import bit_rotate_rigth
from common.make_reg_pipe import make_reg_pipe
from omega_generator.make_switch_box import make_swicth_box


def make_omega(size, num_extra_stagies, radix, print_status=False):
    
    bits_conf_cross = int(ceil(log(radix, 2)) * radix)
    num_stagies = int(ceil(log(size, radix)) + num_extra_stagies)
    num_cross_stagies = int(ceil(size / radix))
    bits_conf = int(num_stagies * num_cross_stagies * bits_conf_cross)
    max_bits = int(ceil(log(size, 2)))

    m = Module('cgra_omega%dx%d_%d' % (size, size, radix))
    WIDTH = m.Parameter('WIDTH', 16)
    PIPE_EXTRA = m.Parameter('PIPE_EXTRA', 0)
    clk = m.Input('clk')
    en = m.Input('en', (num_stagies + 1) * size + num_stagies)
    conf_in = m.Input('conf', bits_conf)

    inputs = [m.Input('in%d' % i, WIDTH) for i in range(size)]
    outputs = [m.Output('out%d' % i, WIDTH) for i in range(size)]

    switch_box = make_swicth_box(radix, radix)
    reg_pipe = make_reg_pipe()

    conf_stage_width = num_cross_stagies * bits_conf_cross
    conf = [m.Wire('conf_reg_%d' % e, conf_stage_width) for e in range(num_stagies)]

    in_reg_wire = [m.Wire('in_reg_wire_%d' % i, WIDTH, size) for i in range(num_stagies * 2)]
    en_count = 0
    for r in range(num_stagies):
        param = [('NUM_STAGES', PIPE_EXTRA + (r + 1)), ('DATA_WIDTH', conf_stage_width)]
        con = [('clk', clk), ('en', en[en_count]), ('in', conf_in[r * conf_stage_width:(r + 1) * conf_stage_width]),
               ('out', conf[r])]
        m.Instance(reg_pipe, 'reg_conf_%d' % (r), param, con)
        en_count = en_count + 1

    c = 0
    for i in range(num_stagies + 1):
        if i > 1:
            c = c + 1
        for j in range(size):
            if i == 0:
                param = [('NUM_STAGES', PIPE_EXTRA + 1), ('DATA_WIDTH', WIDTH)]

                con = [('clk', clk), ('en', en[en_count]), ('in', inputs[j]), ('out', in_reg_wire[i + c][j])]
                m.Instance(reg_pipe, 'reg_in_%d_%d' % (i, j), param, con)
            elif i == (num_stagies + 1) - 1:
                param = [('NUM_STAGES', PIPE_EXTRA + 1), ('DATA_WIDTH', WIDTH)]
                con = [('clk', clk), ('en', en[en_count]), ('in', in_reg_wire[i + c][j]), ('out', outputs[j])]
                m.Instance(reg_pipe, 'reg_in_%d_%d' % (i, j), param, con)
            else:
                param = [('NUM_STAGES', PIPE_EXTRA + 1), ('DATA_WIDTH', WIDTH)]
                con = [('clk', clk), ('en', en[en_count]), ('in', in_reg_wire[i + c][j]),
                       ('out', in_reg_wire[i + c + 1][j])]
                m.Instance(reg_pipe, 'reg_in_%d_%d' % (i, j), param, con)
            en_count = en_count + 1

    conf_idx = 0
    for i in range(0, num_stagies * 2, 2):
        for j in range(num_cross_stagies):
            param = [('WIDTH', WIDTH)]
            con0 = [('sel', conf[conf_idx][j * bits_conf_cross:(j + 1) * bits_conf_cross])]
            con1 = [('in%d' % p, in_reg_wire[i][bit_rotate_rigth(j * radix + p, int(ceil(log(radix, 2))), max_bits)]) for
                    p in
                    range(radix)]
            con2 = [('out%d' % p, in_reg_wire[i + 1][j * radix + p]) for p in range(radix)]
            m.Instance(switch_box, 'sw_%d_%d' % (i / 2, j), param, con0 + con1 + con2)
        conf_idx = conf_idx + 1

    if print_status:
        print('Omega Network:')
        print('Radix: %d' % (radix))
        print('Number of inputs: %d' % (size))
        print('Number of outputs: %d' % (size))
        print('Number of bits conf: %d' % (bits_conf))
        print('Number of stages: %d' % (num_stagies))
        print('Number of switch per stage: %d' % (num_cross_stagies))
        print('Number total of switch: %d' % (num_stagies * num_cross_stagies))
        print('Number of mux%dx1: %d' % (radix, num_stagies * num_cross_stagies * (radix)))

    return m
