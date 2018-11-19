from veriloggen import *


def make_conf_reader_pe(cgra_id, data_width, conf_width, conf_depth):
    m = Module('cgra%d_conf_reader_pe' % cgra_id)
    PE_ID = m.Parameter('PE_ID', 0)

    clk = m.Input('clk')
    rst = m.Input('rst')

    conf_bus_in = m.Input('conf_bus_in', 64)

    instruction_we = m.OutputReg('instruction_we')
    instruction_addr = m.OutputReg('instruction_addr', conf_depth)
    instruction_data = m.OutputReg('instruction_data', conf_width)

    const_we = m.OutputReg('const_we')
    const_waddr = m.OutputReg('const_waddr', 3)
    const_data = m.OutputReg('const_data', data_width)

    pc_max = m.OutputReg('pc_max', conf_depth)
    pc_loop = m.OutputReg('pc_loop', conf_depth)
    store_ignore = m.OutputReg('store_ignore', conf_depth)

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
    pe_inst_addr = m.Reg('pe_inst_addr', 16)
    pe_instruction = m.Reg('pe_instruction', 16)
    pe_const_addr = m.Reg('pe_const_addr', 8)
    if data_width == 64:
        pe_constant = m.Reg('pe_constant', 64)
        rst_pe_const = pe_constant[32:64](0)
    else:
        pe_constant = m.Reg('pe_constant', 32)
        rst_pe_const = m.EmbeddedCode('')

    pe_pc_max = m.Reg('pe_pc_max', 32)
    pe_pc_loop = m.Reg('pe_pc_loop', 32)
    pe_store_ignore = m.Reg('pe_store_ignore', 32)

    instruction_we_p = m.Reg('instruction_we_p')
    instruction_addr_p = m.Reg('instruction_addr_p', conf_depth)
    instruction_data_p = m.Reg('instruction_data_p', conf_width)

    const_we_p = m.Reg('const_we_p')
    const_waddr_p = m.Reg('const_waddr_p', 3)
    const_data_p = m.Reg('const_data_p', data_width)

    m.Initial(
        instruction_we(0),
        instruction_addr(0),
        instruction_data(0),
        const_we(0),
        const_waddr(0),
        const_data(0),
        pc_max(0),
        pc_loop(0),
        store_ignore(0),
        pe_conf_type(0),
        pe_id(0),
        pe_inst_addr(0),
        pe_instruction(0),
        pe_const_addr(0),
        pe_constant(0),
        pe_pc_max(0),
        pe_pc_loop(0),
        pe_store_ignore(0),
        instruction_we_p(0),
        instruction_addr_p(0),
        instruction_data_p(0),
        const_we_p(0),
        const_waddr_p(0),
        const_data_p(0)
    )

    m.Always(Posedge(clk))(
        If(rst)(
            pe_id(0),
            rst_pe_const
        ).Else(
            pe_conf_type(conf_bus_in[0:8]),
            pe_id(conf_bus_in[8:24]),
            pe_inst_addr(conf_bus_in[24:40]),
            pe_instruction(conf_bus_in[40:56]),
            pe_const_addr(conf_bus_in[24:32]),
            pe_constant[0:32](conf_bus_in[32:64]),
            pe_pc_max(conf_bus_in[24:56]),
            pe_pc_loop(conf_bus_in[24:56]),
            pe_store_ignore(conf_bus_in[24:56])
        )
    )

    m.Always(Posedge(clk))(
        If(rst)(
            instruction_we_p(0),
            const_we_p(0),
            pc_max(0),
            pc_loop(0),
            store_ignore(0)
        ).Else(
            instruction_we_p(0),
            const_we_p(0),
            If(pe_id == PE_ID)(
                Case(pe_conf_type)(
                    When(CGRA_NOT_CONF)(),
                    When(CGRA_CONF_SET_PE_INSTRUCTION)(
                        instruction_we_p(Int(1, 1, 2))
                    ),
                    When(CGRA_CONF_SET_PE_CONST)(
                        const_we_p(Int(1, 1, 2))
                    ),
                    When(CGRA_CONF_SET_PE_PC_MAX)(
                        pc_max(pe_pc_max[0:pc_max.width]),
                    ),
                    When(CGRA_CONF_SET_PE_PC_LOOP)(
                        pc_loop(pe_pc_loop[0:pc_loop.width]),
                    ),
                    When(CGRA_CONF_SET_PE_STORE_IGNORE)(
                        store_ignore(pe_store_ignore[0:store_ignore.width]),
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
        instruction_addr_p(pe_inst_addr[0:instruction_addr_p.width]),
        instruction_data_p(pe_instruction[0:instruction_data_p.width]),
        const_waddr_p(pe_const_addr[0:const_waddr_p.width]),
        const_data_p(pe_constant[0:const_data_p.width]),
        instruction_we(instruction_we_p),
        instruction_addr(instruction_addr_p),
        instruction_data(instruction_data_p),
        const_we(const_we_p),
        const_waddr(const_waddr_p),
        const_data(const_data_p)
    )

    return m
