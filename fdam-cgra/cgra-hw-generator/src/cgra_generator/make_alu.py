from veriloggen import *


def make_alu(cgra_id):
    m = Module('cgra%d_alu'%cgra_id)
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
    result = m.Reg('result', width, Power(2, op.width))
    m.EmbeddedCode('')
    out.assign(result[op])

    m.Always(Posedge(clk))(
        result[PASS_A](in_a),
        result[PASS_B](in_b),

        result[ADD](in_a + in_b),
        result[SUB](in_a - in_b),
        result[MULT](in_a * in_b),
        result[DIV](0),
        result[MOD](0),

        result[AND](in_a & in_b),
        result[OR](in_a | in_b),
        result[NOT](~in_a),

        result[SHL](in_a << in_b),
        result[SHR](in_a >> in_b),
        result[SLT](Mux(in_a < in_b, 1, 0)),

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
