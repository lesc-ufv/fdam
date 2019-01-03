from math import ceil, log

from veriloggen import *


def make_branch_control(stack_depth, num_threads):
    m = Module('branch_control')

    clk = m.Input('clk')
    rst = m.Input('rst')

    thread_id = m.Input('thread_id', int(ceil(log(num_threads, 2))))

    branch_in = m.Input('branch_in')
    branch_in_en = m.Input('branch_in_en')

    branch_out = m.Output('branch_out')
    branch_out_en = m.Input('branch_out_en')

    thread_stack = m.Wire('stack', num_threads)

    thread_en = m.Wire('thread_en', num_threads)

    thread_en.assign(Sll(1, thread_id))

    branch_out.assign(thread_stack[thread_id])

    bs = make_branch_stack(stack_depth)
    genv = m.Genvar('genv')
    param = []
    con = [('clk', clk), ('rst', rst), ('en', thread_en[genv]), ('push', branch_in_en), ('din', branch_in),
           ('pop', branch_out_en), ('dout', thread_stack[genv])]

    genFor = m.GenerateFor(genv(0), genv < num_threads, genv.inc(), 'inst_branch_stack')
    genFor.Instance(bs, 'branch_stack', param, con)

    return m


def make_branch_stack(stack_depth):
    m = Module('branch_stack')

    clk = m.Input('clk')
    rst = m.Input('rst')
    en = m.Input('en')
    push = m.Input('push')
    din = m.Input('din')

    pop = m.Input('pop')
    dout = m.Output('dout')

    stack = m.Reg('stack', pow(2, stack_depth))

    dout.assign(stack[0])

    m.Initial(
        stack(0)
    )

    m.Always(Posedge(clk))(
        If(rst)(
            stack(Int(0, stack.width, 10))
        ).Elif(en)(
            Case(Cat(pop, push))(
                When(1)(
                    stack(Cat(stack[0:stack.width - 1], din))
                ),
                When(2)(
                    stack(stack[1:])
                ),
                When(3)(
                    stack[0](din)
                ),
                When()()
            )
        )
    )
    return m
