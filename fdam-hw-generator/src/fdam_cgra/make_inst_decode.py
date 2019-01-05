from veriloggen import *

"""
op, dst, src1, src2

op, fifo_out, fifo_in, rf_out
op, fifo_out, fifo_in, in_b
op, fifo_out, in_a, rf_out
op, fifo_out, in_a, in_b

op, rf_in, fifo_in, rf_out
op, rf_in, fifo_in, in_b
op, rf_in, in_a, rf_out
op, rf_in, in_a, in_b

op, out_a, fifo_in, rf_out
op, out_a, fifo_in, in_b
op, out_a, in_a, rf_out
op, out_a, in_a, in_b

op, out_b, fifo_in, rf_out
op, out_b, fifo_in, in_b
op, out_b, in_a, rf_out
op, out_b, in_a, in_b
"""


def make_inst_decode(cgra_id):
    name = 'cgra%d_inst_decode' % cgra_id
    m = Module(name)

    instruction = m.Input('instruction', 16)
    alu_op = m.Output('alu_op', 4)
    mux_a = m.Output('mux_a', 1)
    mux_b = m.Output('mux_b', 1)
    rf_re = m.Output('rf_re', 1)
    rf_we = m.Output('rf_we', 1)
    fifo_re = m.Output('fifo_re', 1)
    fifo_we = m.Output('fifo_we', 1)
    out_a_en = m.Output('out_a_en', 1)
    out_b_en = m.Output('out_b_en', 1)
    rf_raddr = m.Output('rf_raddr', 4)
    rf_waddr = m.Output('rf_waddr', 4)
    # branch_in_en = m.Output('branch_in_en', 1)
    # branch_out_en = m.Output('branch_out_en', 1)

    control = m.Wire('control', 4)
    A = 3
    B = 2
    C = 1
    D = 0
    control.assign(instruction[4:8])

    alu_op.assign(instruction[0:4]),
    rf_raddr.assign(instruction[8:12]),
    rf_waddr.assign(instruction[12:16]),

    mux_a.assign(control[C]),
    fifo_re.assign(control[C]),

    mux_b.assign((~control[C] & control[D]) | (control[A] & control[C] & ~control[D])),
    rf_re.assign((~control[C] & control[D]) | (control[A] & control[C] & ~control[D])),

    out_a_en.assign(~control[A] & ~control[B]),
    out_b_en.assign(~control[A] & control[B]),

    rf_we.assign(control[A] & ~control[B]),
    fifo_we.assign(control[A] & control[B]),

    # branch_in_en.assign(alu_op[A] & alu_op[B] & ~alu_op[C]),
    # branch_out_en.assign(alu_op[A] & alu_op[B] & alu_op[C] & alu_op[D])

    return m
