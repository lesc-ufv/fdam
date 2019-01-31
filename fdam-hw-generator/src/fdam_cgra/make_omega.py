from math import log, ceil

from veriloggen import *

from common.make_reg_pipe import make_reg_pipe
from common.utils import bit_rotate_rigth, bit_rotate_left
from make_swicth_conf_control import make_swicth_conf_control
from make_switch_box import make_swicth_box


# net conf pack = {id }

def make_omega(num_thread, size, num_extra_stages, radix, conf_net_depth, is_net_branch, print_status=False):

    swicth_conf_width = int(ceil(log(radix, 2)) * radix)
    num_stages = int(ceil(log(size, radix)) + num_extra_stages)
    num_swicth_stages = int(ceil(size / radix))
    num_swicth_total = num_stages * num_swicth_stages
    bits_conf = int(num_stages * num_swicth_stages * swicth_conf_width)
    max_bits = int(ceil(log(size, 2)))

    name = 'omega%dx%d_%d_%d_%d' % (size, size, radix, num_extra_stages, conf_net_depth)
    if(is_net_branch):
        name = 'omega_branch%dx%d_%d_%d_%d' % (size, size, radix, num_extra_stages, conf_net_depth)

    m = Module(name)

    WIDTH = m.Parameter('WIDTH', 16)
    PIPE_EXTRA = m.Parameter('PIPE_EXTRA', 0)

    clk = m.Input('clk')
    rst = m.Input('rst')

    en_pc_net = m.Input('en_pc_net', num_swicth_total)

    en = m.Input('en', (num_stages + 1) * size)

    net_conf_bus_in = m.Input('net_conf_bus_in', 64)

    inputs = [m.Input('in%d' % i, WIDTH) for i in range(size)]
    outputs = [m.Output('out%d' % i, WIDTH) for i in range(size)]

    switch_box = make_swicth_box(radix, radix)
    reg_pipe = make_reg_pipe()
    swicth_conf_control = make_swicth_conf_control(num_thread, swicth_conf_width, conf_net_depth, is_net_branch)

    conf_out = m.Wire('conf_out', bits_conf)
    in_reg_wire = [m.Wire('in_reg_wire_%d' % i, WIDTH, size) for i in range(num_stages * 2)]

    net_conf_bus = m.Wire('net_conf_bus', net_conf_bus_in.width, num_swicth_total)

    r = 1
    stage_count = 0
    en_count = 0
    for i in range(num_swicth_total):
        if stage_count == num_swicth_stages:
            r = r + 1
            stage_count = 0
        if i == 0:
            pin1 = net_conf_bus_in
        else:
            pin1 = net_conf_bus[i - 1]
        param = [('NUM_STAGES', 1), ('DATA_WIDTH', net_conf_bus.width)]
        con = [('clk', clk), ('rst', Int(0, 1, 2)), ('en', Int(1, 1, 2)), ('in', pin1), ('out', net_conf_bus[i])]
        m.Instance(reg_pipe, 'net_conf_bus_reg%d' % i, param, con)
        param = [('SWICTH_NUMBER', i + 1), ('STAGE', PIPE_EXTRA + r)]
        con = [('clk', clk), ('rst', rst), ('en_pc_net', en_pc_net[en_count]), ('conf_bus_in', net_conf_bus[i]),
               ('swicth_conf_out', conf_out[i * swicth_conf_width:(i + 1) * swicth_conf_width])
               ]
        m.Instance(swicth_conf_control, 'swicth_conf_control_%d' % i, param, con)
        stage_count = stage_count + 1
        en_count = en_count + 1

    c = 0
    en_count = 0
    for i in range(num_stages + 1):
        if i > 1:
            c = c + 1
        for j in range(size):
            if i == 0:
                param = [('NUM_STAGES', PIPE_EXTRA + 1), ('DATA_WIDTH', WIDTH)]
                con = [('clk', clk), ('rst', Int(0, 1, 2)), ('en', en[en_count]), ('in', inputs[j]),
                       ('out', in_reg_wire[i + c][j])]
                m.Instance(reg_pipe, 'reg_in_%d_%d' % (i, j), param, con)
            elif i == (num_stages + 1) - 1:
                param = [('NUM_STAGES', PIPE_EXTRA + 1), ('DATA_WIDTH', WIDTH)]
                con = [('clk', clk), ('rst', Int(0, 1, 2)), ('en', en[en_count]), ('in', in_reg_wire[i + c][j]),
                       ('out', outputs[bit_rotate_left(j, int(ceil(log(radix, 2))), max_bits)])]
                m.Instance(reg_pipe, 'reg_in_%d_%d' % (i, j), param, con)
            else:
                param = [('NUM_STAGES', PIPE_EXTRA + 1), ('DATA_WIDTH', WIDTH)]
                con = [('clk', clk), ('rst', Int(0, 1, 2)), ('en', en[en_count]), ('in', in_reg_wire[i + c][j]),
                       ('out', in_reg_wire[i + c + 1][j])]
                m.Instance(reg_pipe, 'reg_in_%d_%d' % (i, j), param, con)
            en_count = en_count + 1

    conf_idx = 0
    for i in range(0, num_stages * 2, 2):
        for j in range(num_swicth_stages):
            param = [('WIDTH', WIDTH)]
            con0 = [('sel', conf_out[conf_idx * swicth_conf_width:(conf_idx + 1) * swicth_conf_width])]
            con1 = [('in%d' % p, in_reg_wire[i][bit_rotate_rigth(j * radix + p, int(ceil(log(radix, 2))), max_bits)]) for p in range(radix)]
            con2 = [('out%d' % p, in_reg_wire[i + 1][j * radix + p]) for p in range(radix)]
            m.Instance(switch_box, 'sw_%d_%d' % (i / 2, j), param, con0 + con1 + con2)
            conf_idx = conf_idx + 1

    if print_status:
        print('Omega Network:')
        print('Radix: %d' % (radix))
        print('Number of inputs: %d' % (size))
        print('Number of outputs: %d' % (size))
        print('Number of bits conf: %d' % (bits_conf))
        print('Number of stages: %d' % (num_stages))
        print('Number of switch per stage: %d' % (num_swicth_stages))
        print('Number total of switch: %d' % num_swicth_total)
        print('Number of mux%dx1: %d' % (radix, num_stages * num_swicth_stages * (radix)))

    return m
