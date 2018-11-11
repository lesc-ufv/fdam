from veriloggen import *


def make_initial_conf(init_conf_id_width, init_conf_width):
    m = Module('cgra_initial_conf')
    PE_ID = m.Parameter('PE_ID', 0)
    clk = m.Input('clk')
    rst = m.Input('rst')

    initial_conf_in = m.Input('initial_conf_in', init_conf_id_width + init_conf_width + 16)
    initial_conf_out = m.OutputReg('initial_conf_out', init_conf_id_width + init_conf_width + 16)

    init_conf_we = m.OutputReg('init_conf_we')
    init_const = m.OutputReg('init_const', 16)
    pc_max = m.OutputReg('pc_max', init_conf_width // 3)
    pc_loop = m.OutputReg('pc_loop', init_conf_width // 3)
    ignore_until = m.OutputReg('ignore_until', init_conf_width // 3)

    m.Initial(
        init_const(0),
        pc_max(0),
        pc_loop(0),
        ignore_until(0),
        init_conf_we(0),

    )

    m.Always(Posedge(clk))(
        If(rst)(
            initial_conf_out(0),
            init_conf_we(0)
        ).Else(
            initial_conf_out(initial_conf_in),
            init_conf_we(0),
            If(initial_conf_out[0:init_conf_id_width] == PE_ID)(
                init_const(initial_conf_out[init_conf_id_width:init_conf_id_width + 16]),
                pc_max(initial_conf_out[init_conf_id_width + 16:init_conf_id_width + 16 + (init_conf_width // 3)]),
                pc_loop(initial_conf_out[
                        init_conf_id_width + 16 + (init_conf_width // 3):init_conf_id_width + 16 + ((
                                init_conf_width // 3) * 2)]),
                ignore_until(initial_conf_out[init_conf_id_width + 16 + ((init_conf_width // 3) * 2):]),
                init_conf_we(1)
            )
        )
    )

    return m
