from math import ceil, log

from veriloggen import *


def make_conf_reader_pe(cgra_id, num_thread, data_width, conf_width, conf_depth):
    m = Module('cgra%d_conf_reader_pe' % cgra_id)
    PE_ID = m.Parameter('PE_ID', 0)

    clk = m.Input('clk')
    rst = m.Input('rst')

    rf_depth = 4

    conf_bus_in = m.Input('conf_bus_in', 64)

    instruction_we = m.OutputReg('instruction_we')
    instruction_addr = m.OutputReg('instruction_addr', conf_depth)
    instruction_data = m.OutputReg('instruction_data', conf_width)

    const_we = m.OutputReg('const_we')
    const_waddr = m.OutputReg('const_waddr', rf_depth)
    const_data = m.OutputReg('const_data', data_width)

    pc_max = m.OutputReg('pc_max', conf_depth)
    pc_max_we = m.OutputReg('pc_max_we')
    pc_loop = m.OutputReg('pc_loop', conf_depth)
    pc_loop_we = m.OutputReg('pc_loop_we')
    ignore_data = m.OutputReg('ignore_data', conf_depth)
    ignore_we = m.OutputReg('ignore_we')
    thread_id = m.OutputReg('thread_id', int(ceil(log(num_thread, 2))))

    CGRA_NOT_CONF = m.Localparam('CGRA_NOT_CONF', 0)
    CGRA_CONF_SET_PE_INSTRUCTION = m.Localparam('CGRA_CONF_SET_PE_INSTRUCTION', 1)
    CGRA_CONF_SET_PE_CONST = m.Localparam('CGRA_CONF_SET_PE_CONST', 2)
    CGRA_CONF_SET_PE_PC_MAX = m.Localparam('CGRA_CONF_SET_PE_PC_MAX', 3)
    CGRA_CONF_SET_PE_PC_LOOP = m.Localparam('CGRA_CONF_SET_PE_PC_LOOP', 4)
    CGRA_CONF_SET_PE_STORE_IGNORE = m.Localparam('CGRA_CONF_SET_PE_STORE_IGNORE', 5)
    CGRA_CONF_SET_NET_PC_MAX = m.Localparam('CGRA_CONF_SET_NET_PC_MAX', 6)
    CGRA_CONF_SET_NET_PC_LOOP = m.Localparam('CGRA_CONF_SET_NET_PC_LOOP', 7)
    CGRA_CONF_NET_SWITCH = m.Localparam('CGRA_CONF_NET_SWITCH', 8)

    pe_conf_type = m.Reg('pe_conf_type', 8)
    pe_id = m.Reg('pe_id', 16)
    pe_thread_id = m.Reg('pe_thread_id', 4)
    pe_inst_addr = m.Reg('pe_inst_addr', 12)
    pe_instruction = m.Reg('pe_instruction', 16)
    pe_const_addr = m.Reg('pe_const_addr', 4)
    pe_pc_max = m.Reg('pe_pc_max', 32)
    pe_pc_loop = m.Reg('pe_pc_loop', 32)
    pe_ignore = m.Reg('pe_ignore', 32)

    if data_width == 64:
        pe_constant = m.Reg('pe_constant', 64)
        rst_pe_const = pe_constant[32:64](0)
    else:
        pe_constant = m.Reg('pe_constant', 32)
        rst_pe_const = m.EmbeddedCode('')

    m.Initial(
        thread_id(0),
        instruction_we(0),
        instruction_addr(0),
        instruction_data(0),
        const_we(0),
        const_waddr(0),
        const_data(0),
        pc_max(0),
        pc_max_we(0),
        pc_loop(0),
        pc_loop_we(0),
        ignore_data(0),
        ignore_we(0),
        pe_conf_type(0),
        pe_id(0),
        pe_thread_id(0),
        pe_inst_addr(0),
        pe_instruction(0),
        pe_const_addr(0),
        pe_constant(0),
        pe_pc_max(0),
        pe_pc_loop(0),
        pe_ignore(0),
    )

    m.Always(Posedge(clk))(
        If(rst)(
            pe_id(0),
            rst_pe_const
        ).Else(
            pe_conf_type(conf_bus_in[0:8]),
            pe_id(conf_bus_in[8:24]),
            pe_thread_id(conf_bus_in[24:28]),

            pe_inst_addr(conf_bus_in[28:40]),
            pe_instruction(conf_bus_in[40:56]),

            pe_const_addr(conf_bus_in[28:32]),
            pe_constant[0:32](conf_bus_in[32:64]),

            pe_pc_max(conf_bus_in[32:64]),
            pe_pc_loop(conf_bus_in[32:64]),
            pe_ignore(conf_bus_in[32:64])
        )
    )

    m.Always(Posedge(clk))(
        If(rst)(
            instruction_we(Int(0, 1, 2)),
            const_we(Int(0, 1, 2)),
            pc_loop_we(Int(0, 1, 2)),
            pc_max_we(Int(0, 1, 2)),
            ignore_we(Int(0, 1, 2))
        ).Else(
            instruction_we(Int(0, 1, 2)),
            const_we(Int(0, 1, 2)),
            pc_loop_we(Int(0, 1, 2)),
            pc_max_we(Int(0, 1, 2)),
            ignore_we(Int(0, 1, 2)),

            If(pe_id == PE_ID)(
                Case(pe_conf_type)(
                    When(CGRA_NOT_CONF)(),
                    When(CGRA_CONF_SET_PE_INSTRUCTION)(
                        instruction_we(Int(1, 1, 2))
                    ),
                    When(CGRA_CONF_SET_PE_CONST)(
                        const_we(Int(1, 1, 2)),
                    ),
                    When(CGRA_CONF_SET_PE_PC_MAX)(
                        pc_max_we(Int(1, 1, 2)),

                    ),
                    When(CGRA_CONF_SET_PE_PC_LOOP)(
                        pc_loop_we(Int(1, 1, 2)),

                    ),
                    When(CGRA_CONF_SET_PE_STORE_IGNORE)(
                        ignore_we(Int(1, 1, 2)),
                    ),
                    When(CGRA_CONF_SET_NET_PC_MAX)(),
                    When(CGRA_CONF_SET_NET_PC_LOOP)(),
                    When(CGRA_CONF_NET_SWITCH)(),
                    When()(),
                )
            )
        )
    )
    m.Always(Posedge(clk))(
        thread_id(pe_thread_id[0:pe_thread_id.width]),
        instruction_addr(pe_inst_addr[0:instruction_addr.width]),
        instruction_data(pe_instruction[0:instruction_data.width]),
        thread_id(pe_thread_id[0:pe_thread_id.width]),
        const_waddr(pe_const_addr[0:const_waddr.width]),
        const_data(pe_constant[0:const_data.width]),
        pc_max(pe_pc_max[0:pc_max.width]),
        pc_loop(pe_pc_loop[0:pc_loop.width]),
        ignore_data(pe_ignore[0:ignore_data.width])
    )

    return m
