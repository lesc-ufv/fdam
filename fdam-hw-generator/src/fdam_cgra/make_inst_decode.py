from veriloggen import *


def make_inst_decode(cgra_id):
    name = 'cgra%d_inst_decode' % cgra_id

    m = Module(name)

    instruction = m.Input('instruction', 16)

    alu_op = m.Output('alu_op', 4)
    alu_a = m.Output('alu_a', 2)
    alu_b = m.Output('alu_b', 2)
    alu_out = m.Output('alu_out', 2)
    rf_raddr = m.Output('rf_raddr', 3)
    rf_waddr = m.Output('rf_waddr', 3)

    alu_op.assign(instruction[0:4])
    alu_a.assign(instruction[4:6])
    alu_b.assign(instruction[6:8])
    alu_out.assign(instruction[8:10])
    rf_raddr.assign(instruction[10:13])
    rf_waddr.assign(instruction[13:16])

    return m
