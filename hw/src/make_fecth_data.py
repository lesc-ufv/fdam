from veriloggen import *


def make_fecth_data():
    m = Module('fecth_data')
    INPUT_DATA_WIDTH = m.Parameter('INPUT_DATA_WIDTH', 512)
    OUTPUT_DATA_WIDTH = m.Parameter('OUTPUT_DATA_WIDTH', 16)

    clk = m.Input('clk')
    en = m.Input('en')
    rst = m.Input('rst')

    available_read = m.Input('available_read')
    request_read = m.OutputReg('request_read')
    data_valid = m.Input('data_valid')
    read_data = m.Input('read_data', INPUT_DATA_WIDTH)

    pop_data = m.Input('pop_data')
    available_pop = m.OutputReg('available_pop')
    data_out = m.Output('data_out', OUTPUT_DATA_WIDTH)

    NUM = m.Localparam('NUM', INPUT_DATA_WIDTH / OUTPUT_DATA_WIDTH)

    fsm_read = m.Reg('fsm_read', 2)
    fsm_control = m.Reg('fsm_control', 2)
    data = m.Reg('data', INPUT_DATA_WIDTH)
    buffer = m.Reg('buffer', INPUT_DATA_WIDTH)
    count = m.Reg('count', NUM)
    has_buffer = m.Reg('has_buffer')
    buffer_read = m.Reg('buffer_read')
    m.EmbeddedCode('')
    data_out.assign(data[0:OUTPUT_DATA_WIDTH])

    m.Initial(
        request_read(0),
        available_pop(0),
        fsm_read(0),
        fsm_control(0),
        data(0),
        buffer(0),
        count(0),
        has_buffer(0),
        buffer_read(0)
    )

    m.Always(Posedge(clk))(
        If(rst)(
            fsm_read(0),
            request_read(0),
            has_buffer(0)
        ).Else(
            request_read(0),
            Case(fsm_read)(
                When(0)(
                    If(available_read & en)(
                        request_read(1),
                        fsm_read(1)
                    )
                ),
                When(1)(
                    If(data_valid)(
                        buffer(read_data),
                        has_buffer(1),
                        fsm_read(2)
                    )
                ),
                When(2)(
                    If(buffer_read)(
                        has_buffer(0),
                        fsm_read(0)
                    )
                )
            )
        )
    )

    m.Always(Posedge(clk))(
        If(rst)(
            fsm_control(0),
            available_pop(0),
            count(0),
            buffer_read(0)
        ).Else(
            buffer_read(0),
            Case(fsm_control)(
                When(0)(
                    If(has_buffer)(
                        data(buffer),
                        count(1),
                        buffer_read(1),
                        available_pop(1),
                        fsm_control(1)
                    )
                ),
                When(1)(
                    If(pop_data & ~count[NUM - 1])(
                        count(count << 1),
                        data(data[OUTPUT_DATA_WIDTH:512])
                    ),
                    If(pop_data & count[NUM - 1] & has_buffer)(
                        count(1),
                        data(buffer),
                        buffer_read(1)
                    ),
                    If(count[NUM - 1] & pop_data & ~has_buffer)(
                        count(count << 1),
                        data(data[OUTPUT_DATA_WIDTH:511]),
                        available_pop(0),
                        fsm_control(0)
                    )
                )
            )
        )
    )

    return m
