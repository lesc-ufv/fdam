from veriloggen import *

from common.make_reg_tree_pipeline import make_reg_tree_pipeline


def make_alu(cgra_id):
    m = Module('cgra%d_alu' % cgra_id)
    width = m.Parameter('DATA_WIDTH', 16)

    clk = m.Input('clk')
    op = m.Input('op', 4)

    # branch_in = m.Input('branch_in')
    # branch_out = m.OutputReg('branch_out')

    in_a = m.Input('ina', width)
    in_b = m.Input('inb', width)

    out = m.Output('out', width)

    PASS_A = m.Localparam('PASS_A', 0)
    PASS_B = m.Localparam('PASS_B', 1)

    ADD = m.Localparam('ADD', 2)
    SUB = m.Localparam('SUB', 3)
    MULT = m.Localparam('MULT', 4)
    DIV = m.Localparam('DIV', 5)
    MOD = m.Localparam('MOD', 6)

    AND = m.Localparam('AND', 7)
    OR = m.Localparam('OR', 8)
    NOT = m.Localparam('NOT', 9)

    SHL = m.Localparam('SHL', 10)
    SHR = m.Localparam('SHR', 11)
    BEQ = m.Localparam('BEQ', 12)
    BNE = m.Localparam('BNE', 13)
    SLT = m.Localparam('SLT', 14)

    MERGE = m.Localparam('MERGE', 15)
    m.EmbeddedCode('')

    in_a_rep = m.Wire('in_a_rep', width, 15)
    in_b_rep = m.Wire('in_b_rep', width, 14)

    ina_regs = make_reg_tree_pipeline(4, 15, 1)
    inb_regs = make_reg_tree_pipeline(4, 14, 1)

    param = [('DATA_WIDTH', width)]
    con = [('clk', clk), ('in', in_a)]
    con1 = [('out_%d' % i, in_a_rep[i]) for i in range(in_a_rep.length)]
    m.Instance(ina_regs, 'ina_regs', param, con + con1)

    param = [('DATA_WIDTH', width)]
    con = [('clk', clk), ('in', in_b)]
    con1 = [('out_%d' % i, in_b_rep[i]) for i in range(in_b_rep.length)]
    m.Instance(inb_regs, 'inb_regs', param, con + con1)

    result = m.Reg('result', width, Power(2, op.width))
    m.EmbeddedCode('')
    out.assign(result[op])

    m.Always(Posedge(clk))(

        result[PASS_A](in_a_rep[0]),
        result[PASS_B](in_b_rep[0]),
        result[ADD](in_a_rep[1] + in_b_rep[1]),
        result[SUB](in_a_rep[2] - in_b_rep[2]),
        result[MULT](in_a_rep[3] * in_b_rep[3]),

        result[AND](in_a_rep[4] & in_b_rep[4]),
        result[OR](in_a_rep[5] | in_b_rep[5]),
        result[NOT](~in_a_rep[6]),

        result[SHL](in_a_rep[7] << in_b_rep[7]),
        result[SHR](in_a_rep[8] >> in_b_rep[8]),
        result[SLT](Mux(in_a_rep[9] < in_b_rep[9], 1, 0)),
        result[DIV](0),
        result[MOD](0),
        result[BEQ](0),
        result[BNE](0),
        result[MERGE](0),
    )
    i = m.Integer('i')
    m.Initial(
        For(i(0), i < Power(2, op.width), i.inc())(
            result[i](0)
        )
    )

    return m
