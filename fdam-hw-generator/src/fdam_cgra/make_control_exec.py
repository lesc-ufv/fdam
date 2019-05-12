from veriloggen import *


def make_control_exec(cgra_id, num_pe_io_in, num_pe_io_out):
    m = Module('cgra%d_control_exec' % cgra_id)
    clk = m.Input('clk')
    rst = m.Input('rst')

    start = m.Input('start')

    read_fifo_mask = m.Input('read_fifo_mask', num_pe_io_in)
    write_fifo_mask = m.Input('write_fifo_mask', num_pe_io_out)

    available_read = m.Input('available_read', num_pe_io_in)
    available_write = m.Input('available_write', num_pe_io_out)

    available_pop = m.Input('available_pop', num_pe_io_in)
    available_push = m.Input('available_push', num_pe_io_out)

    read_fifo_done = m.Input('read_fifo_done', num_pe_io_in)
    write_fifo_done = m.Input('write_fifo_done', num_pe_io_out)

    en = m.OutputReg('en')

    en_fecth_data = m.OutputReg('en_fecth_data', num_pe_io_in)

    done = m.OutputReg('done')

    FSM_IDLE = m.Localparam('FSM_IDLE', 0)
    FSM_PROCESS = m.Localparam('FSM_PROCESS', 1)
    FSM_DONE = m.Localparam('FSM_DONE', 3)
    FSM_WAIT_DATA = m.Localparam('FSM_WAIT_DATA', 2)
    m.EmbeddedCode('')

    fsm_state = m.Reg('fsm_state', 2)

    en_read = m.Reg('en_read', num_pe_io_in)
    en_write = m.Reg('en_write', num_pe_io_out)
    en_read1 = m.Reg('en_read1', num_pe_io_in)
    en_write1 = m.Reg('en_write1', num_pe_io_out)
    en_process = m.Reg('en_process')
    en_process1 = m.Reg('en_process1')
    available_pop_masked = m.Reg('available_pop_masked', num_pe_io_in)
    available_push_masked = m.Reg('available_push_masked', num_pe_io_out)
    available_queues = m.Reg('available_queues')

    m.Initial(
        available_pop_masked(0),
        available_push_masked(0),
        available_queues(0),
        en_process(0),
        en_process1(0),
        en_read(0),
        en_write(0),
        en_read1(0),
        en_write1(0),
        en(0),
        fsm_state(FSM_IDLE),
        en_fecth_data(Int(0, en_fecth_data.width, 16)),
        done(0)
    )

    m.EmbeddedCode('')
    i = m.Integer('i')
    m.Always(Posedge(clk))(
        If(rst)(
            available_pop_masked(0),
            available_push_masked(0),
            available_queues(0)
        ).Else(
            available_pop_masked(available_pop | Unot(read_fifo_mask)),
            available_push_masked(available_push | Unot(write_fifo_mask)),
            available_queues(Uand(available_pop_masked & available_push_masked))
        )
    )
    m.Always(Posedge(clk))(
        If(rst)(
            en_process(0),
            en_process1(0),
            en_read(0),
            en_write(0),
            en_read1(0),
            en_write1(0),
            en(0)
        ).Else(
            en_read(Or(available_read, read_fifo_done)),
            en_write(Or(available_write, write_fifo_done)),
            en_read1(And(en_read, read_fifo_mask)),
            en_write1(And(en_write, write_fifo_mask)),
            en_process(And(Uor(en_read1), Uor(en_write1))),
            en_process1(And(en_process, fsm_state[0])),
            en(en_process1)
        )
    )

    m.Always(Posedge(clk))(
        If(rst)(
            fsm_state(FSM_IDLE),
            en_fecth_data(Int(0, en_fecth_data.width, 16)),
            done(0)
        ).Else(
            Case(fsm_state)(
                When(FSM_IDLE)(
                    If(start)(
                        fsm_state(FSM_WAIT_DATA),
                        en_fecth_data(Int((1 << en_fecth_data.width) - 1, en_fecth_data.width, 16)),
                    )
                ),
                When(FSM_WAIT_DATA)(
                    If(available_queues)(
                        fsm_state(FSM_PROCESS)
                    )
                ),
                When(FSM_PROCESS)(
                    If(Uand(write_fifo_done | ~write_fifo_mask))(
                        fsm_state(FSM_DONE)
                    )
                ),
                When(FSM_DONE)(
                    done(1)
                )
            )
        )
    )

    return m
