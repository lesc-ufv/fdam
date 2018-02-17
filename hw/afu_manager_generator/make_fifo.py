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
    valid = m.OutputReg('valid')
    dout = m.OutputReg('dout', FIFO_WIDTH)
    empty = m.OutputReg('empty')
    almostempty = m.OutputReg('almostempty')
    full = m.OutputReg('full')
    almostfull = m.OutputReg('almostfull')
    count = m.OutputReg('count', FIFO_DEPTH_BITS+1)

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
            valid(Int(0,1,2))
        ).Else(
            valid(Int(0,1,2)),
            If(we == Int(1, 1, 2))(
                mem[wp](din)
            ),
            If(re == Int(1, 1, 2))(
               dout(mem[rp]),
               valid(Int(1,1,2))
            )
        )
    )

    return m




'''
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
    valid = m.OutputReg('valid')
    dout = m.OutputReg('dout', FIFO_WIDTH)
    count = m.OutputReg('count', FIFO_DEPTH_BITS+1)
    empty = m.OutputReg('empty')
    almostempty = m.OutputReg('almostempty')
    full = m.OutputReg('full')
    almostfull = m.OutputReg('almostfull')

    overflow = m.Reg('overflow')
    rp = m.Reg('rp', FIFO_DEPTH_BITS)
    wp = m.Reg('wp', FIFO_DEPTH_BITS)
    empty_d1 = m.Reg('empty_d1')
    mem_we = m.Wire('mem_we')
    mem_re = m.Reg('mem_re')
    mem_dout = m.Wire('mem_dout',FIFO_WIDTH)

    mem_fifo = make_mem_fifo()
    params = [('DATA_WIDTH',FIFO_WIDTH),('ADDR_WIDTH',FIFO_DEPTH_BITS)]
    con = [('clk',clk),('we',mem_we),('re', mem_re),('raddr',rp),('waddr',wp),('din',din),('dout',mem_dout)]
    m.Instance(mem_fifo,'mem_fifo',params,con)

    mem_we.assign(we & ((~re) | (~empty) | mem_re))

    m.Always(Posedge(clk))(
        If(rst)(
            empty(Int(1, 1, 2)),
            empty_d1(Int(1, 1, 2)),
            almostempty(Int(1, 1, 2)),
            full(Int(0, 1, 2)),
            almostfull(Int(0, 1, 2)),
            count(0),
            rp(0),
            wp(0),
            valid(Int(0,1,2)),
            mem_re(Int(0,1,2)),
            overflow(Int(0,1,2)),
            dout(0)
        ).Else(
           empty_d1(empty),
           valid(Int(0,1,2)),
           mem_re(Int(0,1,2)),
           If(we & re & empty & (~mem_re))(
               valid(Int(1,1,2)),
               dout(din)
           ),
           If(we & re & (~empty))(
               wp(wp + Int(1,32,10)),
               rp(rp + Int(1,32,10)),
               mem_re(Int(1,1,2))
           ),
           If(we & ((~re) | (re & empty & mem_re)))(
               wp(wp + Int(1, 32, 10)),
               count(count + Int(1,32,10)),
               empty(Int(0,1,2)),
               If(EmbeddedCode('count == (2**FIFO_DEPTH_BITS-1)'))(
                   full(Int(1,1,2))
               ),
               If(EmbeddedCode('count == (FIFO_ALMOSTFULL_THRESHOLD-1)'))(
                   almostfull(Int(1,1,2))
               ),
               If(EmbeddedCode('count == (FIFO_ALMOSTEMPTY_THRESHOLD-1)'))(
                   almostempty(Int(0, 1, 2))
               )
           ),
           If((~we) & re & (~empty))(
             rp(rp + Int(1,32,10)),
             count(count - Int(1, 32, 10)),
             full(Int(0,1,2)),
             mem_re(Int(1,1,2)),
             If(EmbeddedCode('count == FIFO_ALMOSTFULL_THRESHOLD'))(
                 almostfull(Int(0,1,2))
             ),
             If(EmbeddedCode('count == (FIFO_ALMOSTEMPTY_THRESHOLD)'))(
                   almostempty(Int(1, 1, 2))
             ),
             If(EmbeddedCode('count == 1\'b1'))(
                 empty(Int(1,1,2))
             ),
             If(mem_re)(
                 valid(Int(1,1,2)),
                 dout(mem_dout)
             ),
             EmbeddedCode('// synthesis translate_off'),
             If(EmbeddedCode('rst === 1\'b0'))(
                 If(Not((~(we & (~re) & full))))(
                    EmbeddedCode('$fatal("overflow");')
                 ),
                 If(Not(((we & re &(wp == rp) & (~empty)) == Int(1,1,2))))(
                    EmbeddedCode('$fatal("read-during-write at same time to location %x", wp);')
                 )
             ),
             EmbeddedCode('// synthesis translate_on')
           )
        )
    )

    return m
'''