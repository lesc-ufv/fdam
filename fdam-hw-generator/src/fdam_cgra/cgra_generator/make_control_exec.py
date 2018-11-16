from veriloggen import *

from fdam_cgra.omega_generator.make_omega import make_omega


def make_control_exec(cgra_id, num_pe, num_pe_io, net_radix):
    net = make_omega(num_pe * 2, 0, net_radix, 8)
    en_net_bits = net.get_ports().get('en').width
    en_pc_net_bits = net.get_ports().get('en_pc_net').width

    m = Module('cgra%d_control_exec' % cgra_id)
    clk = m.Input('clk')
    rst = m.Input('rst')

    start = m.Input('start')

    read_fifo_mask = m.Input('read_fifo_mask', num_pe_io)
    write_fifo_mask = m.Input('write_fifo_mask', num_pe_io)

    available_read = m.Input('available_read', num_pe_io)
    available_write = m.Input('available_write', num_pe_io)

    available_pop = m.Input('available_pop', num_pe_io)
    available_push = m.Input('available_push', num_pe_io)

    read_fifo_done = m.Input('read_fifo_done', num_pe_io)
    write_fifo_done = m.Input('write_fifo_done', num_pe_io)

    en_pe = m.OutputReg('en_pe', num_pe)
    en_net = m.OutputReg('en_net', en_net_bits)
    en_pc_net = m.OutputReg('en_pc_net', en_pc_net_bits)

    en_fecth_data = m.OutputReg('en_fecth_data', num_pe_io)

    done = m.OutputReg('done')

    FSM_IDLE = m.Localparam('FSM_IDLE', 0)
    FSM_PROCESS = m.Localparam('FSM_PROCESS', 1)
    FSM_DONE = m.Localparam('FSM_DONE', 3)
    FSM_WAIT_DATA = m.Localparam('FSM_WAIT_DATA', 2)
    m.EmbeddedCode('')

    fsm_state = m.Reg('fsm_state', 2)

    en_read = m.Reg('en_read', num_pe_io)
    en_write = m.Reg('en_write', num_pe_io)
    en_read1 = m.Reg('en_read1', num_pe_io)
    en_write1 = m.Reg('en_write1', num_pe_io)
    en_process = m.Reg('en_process', num_pe + en_net_bits + en_pc_net_bits)
    en_process1 = m.Reg('en_process1', num_pe + en_net_bits + en_pc_net_bits)
    available_pop_masked = m.Reg('available_pop_masked', num_pe_io)
    available_push_masked = m.Reg('available_push_masked', num_pe_io)
    available_queues = m.Reg('available_queues')

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
            en_pe(0),
            en_net(0),
            en_pc_net(0)
        ).Else(
            en_read(Or(available_read, read_fifo_done)),
            en_write(Or(available_write, write_fifo_done)),
            en_read1(And(en_read, read_fifo_mask)),
            en_write1(And(en_write, write_fifo_mask)),
            For(i(0), i < en_process.width, i.inc())(
                en_process[i](And(Uor(en_read1), Uor(en_write1))),
                en_process1[i](And(en_process[i], fsm_state[0]))
            ),
            For(i(0), i < en_pe.width, i.inc())(
                en_pe[i](en_process1[i])
            ),
            For(i(0), i < en_net.width, i.inc())(
                en_net[i](en_process1[i + en_pe.width])
            ),
            For(i(0), i < en_pc_net.width, i.inc())(
                en_pc_net[i](en_process1[i + en_pe.width + en_net.width])
            )
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
