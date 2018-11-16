from veriloggen import *


def make_dispath_data():
    m = Module('dispath_data')
    INPUT_DATA_WIDTH = m.Parameter('INPUT_DATA_WIDTH', 16)
    OUTPUT_DATA_WIDTH = m.Parameter('OUTPUT_DATA_WIDTH', 512)

    clk = m.Input('clk')
    en = m.Input('en')
    rst = m.Input('rst')

    available_write = m.Input('available_write')
    request_write = m.OutputReg('request_write')
    write_data = m.OutputReg('write_data', OUTPUT_DATA_WIDTH)
    push_data = m.Input('push_data')
    available_push = m.OutputReg('available_push')
    data_in = m.Input('data_in', INPUT_DATA_WIDTH)

    NUM = m.Localparam('NUM', OUTPUT_DATA_WIDTH / INPUT_DATA_WIDTH)
    m.EmbeddedCode('')
    fsm_control = m.Reg('fsm_control', 2)
    buffer1 = m.Reg('buffer1', OUTPUT_DATA_WIDTH)
    buffer2 = m.Reg('buffer2', OUTPUT_DATA_WIDTH)
    count1 = m.Reg('count1', NUM)
    count2 = m.Reg('count2', NUM)
    request_write1 = m.Reg('request_write1')
    request_write2 = m.Reg('request_write2')
    request_write11 = m.Reg('request_write11')
    request_write22 = m.Reg('request_write22')

    m.Always(Posedge(clk))(
        If(rst)(
            request_write(0),
            request_write11(0),
            request_write22(0)
        ).Else(
            request_write11(request_write1),
            request_write22(request_write2),
            request_write(request_write11 | request_write22),
            If(en)(
                If(request_write11)(
                    write_data(buffer1)
                ).Elif(request_write22)(
                    write_data(buffer2)
                )
            )
        )
    )
    m.Always(Posedge(clk))(
        If(rst)(
            available_push(1),
            fsm_control(0),
            count1(1),
            count2(1),
            request_write1(0),
            request_write2(0),
        ).Else(
            request_write1(0),
            request_write2(0),
            If(en)(
                Case(fsm_control)(
                    When(0)(
                        If(push_data)(
                            buffer1(Or(Cat(data_in, Repeat(Int(0, 1, 2), OUTPUT_DATA_WIDTH - INPUT_DATA_WIDTH)),
                                       buffer1 >> INPUT_DATA_WIDTH)),
                            count1(count1 << 1)
                        ),
                        If(count1[NUM - 1] & push_data)(
                            fsm_control(1)
                        )
                    ),
                    When(1)(
                        If(available_write)(
                            count1(1),
                            request_write1(1),
                            available_push(1)
                        ),
                        If(available_write & available_push)(
                            fsm_control(2)
                        ),
                        If(available_write & ~available_push)(
                            fsm_control(3)
                        ),
                        If(push_data)(
                            buffer2(Or(Cat(data_in, Repeat(Int(0, 1, 2), OUTPUT_DATA_WIDTH - INPUT_DATA_WIDTH)),
                                       buffer2 >> INPUT_DATA_WIDTH)),
                            count2(count2 << 1)
                        ),
                        If(count2[NUM - 2] & push_data & ~available_write)(
                            available_push(0)
                        )
                    ),
                    When(2)(
                        If(push_data)(
                            buffer2(Or(Cat(data_in, Repeat(Int(0, 1, 2), OUTPUT_DATA_WIDTH - INPUT_DATA_WIDTH)),
                                       buffer2 >> INPUT_DATA_WIDTH)),
                            count2(count2 << 1)
                        ),
                        If(count2[NUM - 1] & push_data)(
                            fsm_control(3)
                        )
                    ),
                    When(3)(
                        If(available_write)(
                            request_write2(1),
                            available_push(1),
                            count2(1)
                        ),
                        If(available_write & available_push)(
                            fsm_control(0)
                        ),
                        If(available_write & ~available_push)(
                            fsm_control(1)
                        ),
                        If(push_data)(
                            buffer1(Or(Cat(data_in, Repeat(Int(0, 1, 2), OUTPUT_DATA_WIDTH - INPUT_DATA_WIDTH)),
                                       buffer1 >> INPUT_DATA_WIDTH)),
                            count1(count1 << 1)
                        ),
                        If(count1[NUM - 2] & push_data & ~available_write)(
                            available_push(0)
                        )
                    )
                )
            )
        )
    )

    return m
