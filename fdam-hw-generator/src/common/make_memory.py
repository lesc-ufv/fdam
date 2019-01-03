from veriloggen import *


def make_memory():
    name = 'memory'
    m = Module(name)
    data_width = m.Parameter('DATA_WIDTH', 32)
    addr_width = m.Parameter('ADDR_WIDTH', 8)
    clk = m.Input('clk')
    we = m.Input('we')
    re = m.Input('re')

    raddr = m.Input('raddr', addr_width)
    waddr = m.Input('waddr', addr_width)
    din = m.Input('din', data_width)
    dout = m.OutputReg('dout', data_width)

    m.EmbeddedCode('(* ramstyle = "AUTO, no_rw_check" *) reg  [DATA_WIDTH-1:0] mem[0:2**ADDR_WIDTH-1];')
    m.EmbeddedCode('/*')
    mem = m.Reg('mem', data_width, Power(2, addr_width))
    m.EmbeddedCode('*/')

    m.Always(Posedge(clk))(
        If(we)(
            mem[waddr](din)
        ),
        If(re)(
            dout(mem[raddr])
        )

    )
    m.EmbeddedCode('//synthesis translate_off')

    i = m.Integer('i')
    m.Initial(
        dout(0),
        For(i(0), i < Power(2, addr_width), i.inc())(
            mem[i](0)
        )
    )

    m.EmbeddedCode('//synthesis translate_on')
    return m
