from veriloggen import *

from utils import make_tree_array


def make_reg_tree_pipeline(radix, num_output, extra_pipeline):
    array = list(reversed(make_tree_array(radix, num_output, [])))
    for i in range(extra_pipeline):
        array.append([1 for j in range(num_output)])

    r = 1
    rr = 0
    code = 'r0 <= in;\n'

    for a in array:
        for b in a:
            for c in range(b):
                code = code + 'r%d <= r%d;\n' % (r, rr)
                #print('r%d -> r%d' % (rr, r))
                r = r + 1
            rr = rr + 1

    m = Module('fdam_reg_tree_%d_%d_%d' % (radix, num_output,extra_pipeline))
    DATA_WIDTH = m.Parameter('DATA_WIDTH', 16)
    clk = m.Input('clk')
    in_ = m.Input('in', DATA_WIDTH)
    outputs = []
    for i in range(num_output):
        name = 'out_%d' % i
        outputs.append(m.Output(name, DATA_WIDTH))

    regs = []
    code_initial = ''
    for i in range(r):
        regs.append(m.Reg('r%d' % i, DATA_WIDTH))
        code_initial = code_initial + 'r%d = 0;\n'%i

    m.Initial(
       EmbeddedCode(code_initial)
    )
    m.EmbeddedCode('')
    m.Always(Posedge(clk))(
        EmbeddedCode(code)
    )
    for i in range(r - num_output, r):
        outputs[i - r].assign(regs[i])
    return m
