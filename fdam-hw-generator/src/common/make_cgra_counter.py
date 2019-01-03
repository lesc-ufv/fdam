from math import ceil, log

from veriloggen import *


def make_ignore_counter():
    m = Module('ignore_counter')
    WIDTH = m.Parameter('WIDTH', 8)
    clk = m.Input('clk')
    rst = m.Input('rst')
    en = m.Input('en')
    limit = m.Input('limit', WIDTH)
    end_counter = m.OutputReg('end_counter')
    count = m.Reg('count', WIDTH + 1)

    m.Initial(
        count(0),
        end_counter(0),
    )
    m.Always(Posedge(clk))(
        If(rst)(
            count(0),
            end_counter(1)
        ).Else(
            end_counter(0),
            If(en & ~(count[count.width - 1]))(
                count.inc()
            ),
            If(count >= limit)(
                end_counter(1),
            )
        )
    )

    return m


def make_thread_counter(num_thread):
    m = Module('thread_counter_%d' % num_thread)
    clk = m.Input('clk')
    rst = m.Input('rst')
    en = m.Input('en')
    out = m.OutputReg('out', int(ceil(log(num_thread, 2))))

    m.Initial(
        out(0)
    )

    m.Always(Posedge(clk))(
        If(rst)(
            out(0)
        ).Else(
            If(en)(
                out.inc(),
                If(out >= num_thread - 1)(
                    out(0)
                )
            )
        )
    )

    return m
