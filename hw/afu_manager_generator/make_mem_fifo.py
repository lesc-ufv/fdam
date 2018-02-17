from veriloggen import *

def make_mem_fifo():
    m = Module('fifo_mem')
    DATA_WIDTH = m.Parameter('DATA_WIDTH',32)
    ADDR_WIDTH = m.Parameter('ADDR_WIDTH',8)
    clk = m.Input('clk')
    we  = m.Input('we')
    re = m.Input('re')
    raddr = m.Input('raddr',ADDR_WIDTH)
    waddr = m.Input('waddr',ADDR_WIDTH)
    din = m.Input('din',DATA_WIDTH)
    dout = m.OutputReg('dout',DATA_WIDTH)

    code = '(* ramstyle = "no_rw_check" *)'
    m.EmbeddedCode(code)
    mem = m.Reg('mem',DATA_WIDTH,EmbeddedCode('2**ADDR_WIDTH'))
    m.Always(Posedge(clk))(
        If(we)(
          mem[waddr](din)
        ),
        If(re)(
          dout(mem[raddr])
        )
    )
    return m