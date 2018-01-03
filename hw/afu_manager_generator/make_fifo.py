from veriloggen import *


def make_fifo():
    m = Module('fifo')
    FIFO_WIDTH = m.Parameter('FIFO_WIDTH', 32)
    FIFO_DEPTH_BITS = m.Parameter('FIFO_DEPTH_BITS', 8)
    FIFO_ALMOSTFULL_THRESHOLD = m.Parameter('FIFO_ALMOSTFULL_THRESHOLD', EmbeddedNumeric('2**FIFO_DEPTH_BITS - 4'))
    FIFO_ALMOSTEMPTY_THRESHOLD = m.Parameter('FIFO_ALMOSTEMPTY_THRESHOLD', 2)

    clk = m.Input('clk')
    rst = m.Input('rst')
    we = m.Input('we')
    din = m.Input('din', FIFO_WIDTH)
    re = m.Input('re')
    dout = m.OutputReg('dout', FIFO_WIDTH)
    empty = m.OutputReg('empty')
    almostempty = m.OutputReg('almostempty')
    full = m.OutputReg('full')
    almostfull = m.OutputReg('almostfull')
    count = m.OutputReg('count', FIFO_DEPTH_BITS)

    rp = m.Reg('rp', FIFO_DEPTH_BITS)
    wp = m.Reg('wp', FIFO_DEPTH_BITS)
    mem = m.Reg('mem', FIFO_WIDTH, EmbeddedNumeric('2**FIFO_DEPTH_BITS'))

    m.Always(Posedge(clk))(
        If(rst)(
            empty(Int(1, 1, 2)),
            almostempty(Int(1, 1, 2)),
            full(Int(0, 1, 2)),
            almostfull(Int(0, 1, 2)),
            rp(0),
            wp(0),
            count(0)
        ).Else(
            Case(Cat(we, re))(
                When(Int(3, 2, 2))(
                    rp.inc(),
                    wp.inc(),
                ),
                When(Int(2, 2, 2))(
                    If(~full)(
                        wp.inc(),
                        count.inc(),
                        empty(Int(0, 1, 2)),
                        If(count == (FIFO_ALMOSTEMPTY_THRESHOLD - 1))(
                            almostempty(Int(0, 1, 2))
                        ),
                        If(count == EmbeddedNumeric('2**FIFO_DEPTH_BITS-1'))(
                            full(Int(1, 1, 2))
                        ),
                        If(count == (FIFO_ALMOSTFULL_THRESHOLD - 1))(
                            almostfull(Int(1, 1, 2))
                        )
                    )

                ),
                When(Int(1, 2, 2))(
                    If(~empty)(
                        rp.inc(),
                        count(count - 1),
                        full(Int(0, 1, 2)),
                        If(count == FIFO_ALMOSTFULL_THRESHOLD)(
                            almostfull(Int(0, 1, 2))
                        ),
                        If(count == 1)(
                            empty(Int(1, 1, 2))
                        ),
                        If(count == FIFO_ALMOSTEMPTY_THRESHOLD)(
                            almostempty(Int(1, 1, 2))
                        )

                    )
                ),
                When()(

                )

            )
        )

    )
    m.Always(Posedge(clk))(
        If(rst)(
            dout(0),
        ).Else(
            If(we == Int(1, 1, 2))(
                mem[wp](din)
            ),
            # If(re == Int(1, 1, 2))(
            dout(mem[rp])
            # )
        )
    )

    return m
