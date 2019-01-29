from veriloggen import *


def make_alu(cgra_id):
    m = Module('cgra%d_alu' % cgra_id)
    width = m.Parameter('DATA_WIDTH', 16)

    clk = m.Input('clk')
    op = m.Input('op', 4)

    branch_in = m.Input('branch_in')
    branch_out = m.Output('branch_out')

    in_a = m.Input('ina', width)
    in_b = m.Input('inb', width)

    out = m.Output('out', width)

    PASS_A = m.Localparam('PASS_A', 0)
    PASS_B = m.Localparam('PASS_B', 1)
    MIN = m.Localparam('MIN', 2)
    MAX = m.Localparam('MAX', 3)

    BEQ = m.Localparam('BEQ', 0)
    BNE = m.Localparam('BNE', 1)
    SLT = m.Localparam('SLT', 2)
    SGT = m.Localparam('SGT', 3)

    ADD = m.Localparam('ADD', 4)
    SUB = m.Localparam('SUB', 5)
    MULT = m.Localparam('MULT', 6)
    XOR = m.Localparam('XOR', 7)
    AND = m.Localparam('AND', 8)
    OR = m.Localparam('OR', 9)
    NOT = m.Localparam('NOT', 10)

    SHL = m.Localparam('SHL', 11)
    SHR = m.Localparam('SHR', 12)
    MUX = m.Localparam('MUX', 13)

    ABS = m.Localparam('ABS', 14)

    result = m.Reg('result', width, Power(2, op.width))
    result_branch = m.Reg('result_branch', 4)

    op_reg = m.Reg('op_reg', op.width)

    out.assign(result[op_reg])
    branch_out.assign(result_branch[op_reg[0:2]])

    m.Always(Posedge(clk))(
        op_reg(op),
        result[PASS_A](in_a),
        result[PASS_B](in_b),

        result[ADD](in_a + in_b),
        result[SUB](in_a - in_b),
        result[MULT](in_a * in_b),

        result[AND](in_a & in_b),
        result[OR](in_a | in_b),
        result[NOT](~in_a),
        result[XOR](in_a ^ in_b),

        result[SHL](in_a << in_b),
        result[SHR](in_a >> in_b),

        result[MIN](Mux(in_a < in_b, in_a, in_b)),
        result[MAX](Mux(in_a > in_b, in_a, in_b)),
        result[MUX](Mux(branch_in, in_a, in_b)),
        result[ABS](Mux(in_a[in_a.width-1], -in_a, in_a)),
    )

    m.Always(Posedge(clk))(
        result_branch[BEQ](in_a == in_b),
        result_branch[BNE](in_a != in_b),
        result_branch[SLT](Mux(in_a < in_b, Int(1, 1, 2), Int(0, 1, 2))),
        result_branch[SGT](Mux(in_a > in_b, Int(1, 1, 2), Int(0, 1, 2)))
    )
    return m
