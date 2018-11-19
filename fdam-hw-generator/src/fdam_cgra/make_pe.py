from veriloggen import *

from common.make_cgra_counter import make_cgra_counter
from common.make_mux import make_mux
from common.make_reg_pipe import make_reg_pipe
from make_alu import make_alu
from make_alu_out_decode import make_alu_out_decode
from make_conf_reader_pe import make_conf_reader_pe
from make_inst_decode import make_inst_decode


def make_pe(cgra_id, is_io, pc, memory, data_width, conf_depth):
    name = 'cgra%d_pe' % cgra_id
    if is_io:
        name = name + '_io'
    m = Module(name)

    PE_ID = m.Parameter('PE_ID', 0)

    clk = m.Input('clk')
    rst = m.Input('rst')
    en = m.Input('en')
    conf_bus_width = 64
    conf_bus_in = m.Input('conf_bus_in', conf_bus_width)

    ina = m.Input('ina', data_width)
    inb = m.Input('inb', data_width)

    outa = m.Output('outa', data_width)
    outb = m.Output('outb', data_width)

    alu_out = m.Wire('alu_out', data_width)

    alu_out_addr = m.Wire('alu_out_addr', 2)
    alu_out_addr_reg = m.Wire('alu_out_addr_reg', 2)
    alu_ina = m.Wire('alu_ina', data_width)
    alu_ina_reg = m.Wire('alu_ina_reg', data_width)
    alu_ina_addr = m.Wire('alu_ina_addr', 2)
    alu_ina_addr_reg = m.Wire('alu_ina_addr_reg', 2)
    alu_inb = m.Wire('alu_inb', data_width)
    alu_inb_reg = m.Wire('alu_inb_reg', data_width)
    alu_inb_addr = m.Wire('alu_inb_addr', 2)
    alu_inb_addr_reg = m.Wire('alu_inb_addr_reg', 2)
    alu_op = m.Wire('alu_op', 4)
    alu_op_reg = m.Wire('alu_op_reg', 4)
    rf_raddr = m.Wire('rf_raddr', 3)
    rf_raddr_reg = m.Wire('rf_raddr_reg', 3)
    rf_waddr = m.Wire('rf_waddr', 3)
    rf_waddr_reg = m.Wire('rf_waddr_reg', 3)
    rf_data_in = m.Wire('rf_data_in', data_width)
    rf_data_out = m.Wire('rf_data_out', data_width)
    rf_data_out_reg = m.Wire('rf_data_out_reg', data_width)
    mux_rf_const_out = m.Wire('mux_rf_const_out', data_width)
    mux_rf_const_waddr_out = m.Wire('mux_rf_const_waddr_out', 3)

    m.EmbeddedCode('')
    conf_wr_addr = m.Wire('conf_wr_addr', conf_depth)
    conf_wr_data = m.Wire('conf_wr_data', 16)
    conf_wr_en = m.Wire('conf_wr_en')
    m.EmbeddedCode('')
    init_const_we = m.Wire('init_const_we')
    init_conf_const_waddr = m.Wire('init_conf_const_waddr', 3)
    init_conf_const = m.Wire('init_conf_const', data_width)
    pc_max = m.Wire('pc_max', conf_depth)
    pc_loop = m.Wire('pc_loop', conf_depth)
    pc_out = m.Wire('pc_out', conf_depth)

    ignore_until = m.Wire('ignore_until', conf_depth)

    m.EmbeddedCode('')
    ina_reg = m.Wire('ina_reg', data_width)
    inb_reg = m.Wire('inb_reg', data_width)
    inst_mem_out = m.Wire('inst_mem_out', 16)
    n = 3
    if is_io:
        n = 4
    alu_out_decode_out = m.Wire('alu_out_decode_out', n)
    rf_we = m.Wire('rf_we')
    rf_rd_en = m.Wire('rf_rd_en')
    reg = make_reg_pipe()
    counter = make_cgra_counter()

    if is_io:
        fifo_in_data = m.Input('fifo_in_data', data_width)
        fifo_in_data_reg = m.Wire('fifo_in_data_reg', data_width)
        fifo_in_re = m.Output('fifo_in_re')
        fifo_out_data = m.Output('fifo_out_data', data_width)
        fifo_out_we = m.Output('fifo_out_we')
        ignore_until_en = m.Wire('ignore_until_en')
        ignore_counter_en = m.Wire('ignore_counter_en')
        ignore_counter_out = m.Wire('ignore_counter_out', conf_depth)
        m.EmbeddedCode('')
        fifo_in_re.assign(AndList(OrList(alu_ina_addr_reg == Int(3, 2, 10), alu_inb_addr_reg == Int(3, 2, 10)), en))
        fifo_out_we.assign(AndList(alu_out_addr_reg == Int(3, 2, 10), en, ignore_until_en))
        ignore_counter_en.assign(AndList(alu_out_addr_reg == Int(3, 2, 10), en))
        rf_rd_en.assign(AndList(OrList(alu_ina_addr_reg == Int(2, 2, 10), alu_inb_addr_reg == Int(2, 2, 10)), en))

        param = [('NUM_STAGES', 1), ('DATA_WIDTH', data_width)]
        con = [('clk', clk), ('rst', Int(0, 1, 2)), ('en', fifo_in_re), ('in', fifo_in_data), ('out', fifo_in_data_reg)]
        m.Instance(reg, 'fifo_in_data_reg_inst', param, con)

        param = [('NUM_STAGES', 1), ('DATA_WIDTH', data_width)]
        con = [('clk', clk), ('rst', Int(0, 1, 2)), ('en', en & alu_out_decode_out[3]), ('in', alu_out),
               ('out', fifo_out_data)]
        m.Instance(reg, 'fifo_out_data_reg_inst', param, con)

        param = [('WIDTH', conf_depth)]
        con = [('clk', clk), ('rst', rst), ('en', ignore_counter_en), ('limit', ignore_until),
               ('end_counter', ignore_until_en), ('out', ignore_counter_out)]
        m.Instance(counter, 'ignore_counter', param, con)

        mux4 = make_mux(4)
        param = [('WIDTH', data_width)]
        con = [('sel', alu_ina_addr_reg), ('in0', ina_reg), ('in1', inb_reg), ('in2', rf_data_out_reg),
               ('in3', fifo_in_data_reg),
               ('out', alu_ina)]
        m.Instance(mux4, 'mux_alu_ina', param, con)

        con = [('sel', alu_inb_addr_reg), ('in0', ina_reg), ('in1', inb_reg), ('in2', rf_data_out_reg),
               ('in3', fifo_in_data_reg),
               ('out', alu_inb)]
        m.Instance(mux4, 'mux_alu_inb', param, con)

        alu_out_decode = make_alu_out_decode(cgra_id, True)
        param = []
        con = [('sel', alu_out_addr_reg), ('out', alu_out_decode_out)]
        m.Instance(alu_out_decode, 'alu_out_decode', param, con)
    else:
        rf_rd_en.assign(AndList(OrList(alu_ina_addr_reg == Int(2, 2, 10), alu_inb_addr_reg == Int(2, 2, 10)), en))
        mux3 = make_mux(3)
        param = [('WIDTH', data_width)]
        con = [('sel', alu_ina_addr_reg), ('in0', ina_reg), ('in1', inb_reg), ('in2', rf_data_out_reg),
               ('out', alu_ina)]
        m.Instance(mux3, 'mux_alu_ina', param, con)

        con = [('sel', alu_inb_addr_reg), ('in0', ina_reg), ('in1', inb_reg), ('in2', rf_data_out_reg),
               ('out', alu_inb)]
        m.Instance(mux3, 'mux_alu_inb', param, con)
        alu_out_decode = make_alu_out_decode(cgra_id, False)
        param = []
        con = [('sel', alu_out_addr_reg), ('out', alu_out_decode_out)]
        m.Instance(alu_out_decode, 'alu_out_decode', param, con)

    param = [('NUM_STAGES', 1), ('DATA_WIDTH', data_width)]
    con = [('clk', clk), ('rst', Int(0, 1, 2)), ('en', en), ('in', ina), ('out', ina_reg)]
    m.Instance(reg, 'ina_reg_inst', param, con)

    param = [('NUM_STAGES', 1), ('DATA_WIDTH', data_width)]
    con = [('clk', clk), ('rst', Int(0, 1, 2)), ('en', en), ('in', inb), ('out', inb_reg)]
    m.Instance(reg, 'inb_reg_inst', param, con)

    param = [('NUM_STAGES', 1), ('DATA_WIDTH', data_width)]
    con = [('clk', clk), ('rst', Int(0, 1, 2)), ('en', en & alu_out_decode_out[0]), ('in', alu_out), ('out', outa)]
    m.Instance(reg, 'outa_reg_inst', param, con)

    param = [('NUM_STAGES', 1), ('DATA_WIDTH', data_width)]
    con = [('clk', clk), ('rst', Int(0, 1, 2)), ('en', en & alu_out_decode_out[1]), ('in', alu_out), ('out', outb)]
    m.Instance(reg, 'outb_reg_inst', param, con)

    param = [('NUM_STAGES', 1), ('DATA_WIDTH', data_width)]
    con = [('clk', clk), ('rst', Int(0, 1, 2)), ('en', en), ('in', alu_ina), ('out', alu_ina_reg)]
    m.Instance(reg, 'alu_ina_reg_inst', param, con)

    param = [('NUM_STAGES', 1), ('DATA_WIDTH', alu_ina_addr_reg.width)]
    con = [('clk', clk), ('rst', rst), ('en', en), ('in', alu_ina_addr), ('out', alu_ina_addr_reg)]
    m.Instance(reg, 'alu_ina_addr_reg_inst', param, con)

    param = [('NUM_STAGES', 1), ('DATA_WIDTH', data_width)]
    con = [('clk', clk), ('rst', Int(0, 1, 2)), ('en', en), ('in', alu_inb), ('out', alu_inb_reg)]
    m.Instance(reg, 'alu_inb_reg_inst', param, con)

    param = [('NUM_STAGES', 1), ('DATA_WIDTH', alu_inb_addr_reg.width)]
    con = [('clk', clk), ('rst', rst), ('en', en), ('in', alu_inb_addr), ('out', alu_inb_addr_reg)]
    m.Instance(reg, 'alu_inb_addr_reg_inst', param, con)

    param = [('NUM_STAGES', 8), ('DATA_WIDTH', alu_out_addr_reg.width)]
    con = [('clk', clk), ('rst', rst), ('en', en), ('in', alu_out_addr), ('out', alu_out_addr_reg)]
    m.Instance(reg, 'alu_out_addr_reg_inst', param, con)

    param = [('NUM_STAGES', 1), ('DATA_WIDTH', data_width)]
    con = [('clk', clk), ('rst', Int(0, 1, 2)), ('en', en & alu_out_decode_out[2]), ('in', alu_out),
           ('out', rf_data_in)]
    m.Instance(reg, 'rf_data_in_reg_inst', param, con)

    param = [('NUM_STAGES', 1), ('DATA_WIDTH', 1)]
    con = [('clk', clk), ('rst', rst), ('en', en), ('in', alu_out_decode_out[2]), ('out', rf_we)]
    m.Instance(reg, 'rf_we_reg_inst', param, con)

    param = [('NUM_STAGES', 1), ('DATA_WIDTH', data_width)]
    con = [('clk', clk), ('rst', Int(0, 1, 2)), ('en', en), ('in', rf_data_out), ('out', rf_data_out_reg)]
    m.Instance(reg, 'rf_data_out_reg_inst', param, con)

    param = [('NUM_STAGES', 6), ('DATA_WIDTH', alu_op_reg.width)]
    con = [('clk', clk), ('rst', rst), ('en', en), ('in', alu_op), ('out', alu_op_reg)]
    m.Instance(reg, 'alu_op_reg_inst', param, con)

    param = [('NUM_STAGES', 1), ('DATA_WIDTH', rf_raddr.width)]
    con = [('clk', clk), ('rst', rst), ('en', en), ('in', rf_raddr), ('out', rf_raddr_reg)]
    m.Instance(reg, 'rf_raddr_reg_inst', param, con)

    param = [('NUM_STAGES', 8), ('DATA_WIDTH', rf_waddr.width)]
    con = [('clk', clk), ('rst', rst), ('en', en), ('in', rf_waddr), ('out', rf_waddr_reg)]
    m.Instance(reg, 'rf_waddr_reg_inst', param, con)

    mux2 = make_mux(2)
    param = [('WIDTH', data_width)]
    con = [('sel', init_const_we), ('in0', rf_data_in), ('in1', init_conf_const), ('out', mux_rf_const_out)]
    m.Instance(mux2, 'mux_rf_const', param, con)

    param = [('WIDTH', 3)]
    con = [('sel', init_const_we), ('in0', rf_waddr_reg), ('in1', init_conf_const_waddr),
           ('out', mux_rf_const_waddr_out)]
    m.Instance(mux2, 'mux_rf_const_waddr', param, con)

    param = [('DATA_WIDTH', data_width), ('ADDR_WIDTH', 3)]
    con = [('clk', clk), ('we', rf_we | init_const_we), ('re', rf_rd_en), ('raddr', rf_raddr_reg),
           ('waddr', mux_rf_const_waddr_out), ('din', mux_rf_const_out), ('dout', rf_data_out)]
    m.Instance(memory, 'rf', param, con)

    alu = make_alu(cgra_id)
    param = [('DATA_WIDTH', data_width)]
    con = [('clk', clk), ('op', alu_op_reg), ('ina', alu_ina_reg), ('inb', alu_inb_reg), ('out', alu_out)]
    m.Instance(alu, 'alu', param, con)

    param = [('DATA_WIDTH', 16), ('ADDR_WIDTH', conf_depth)]
    con = [('clk', clk), ('we', conf_wr_en), ('re', Int(1, 1, 2)), ('raddr', pc_out), ('waddr', conf_wr_addr),
           ('din', conf_wr_data), ('dout', inst_mem_out)]
    m.Instance(memory, 'inst_mem', param, con)

    conf_reader_pe = make_conf_reader_pe(cgra_id, data_width, 16, conf_depth)
    param = [('PE_ID', PE_ID)]
    con = [('clk', clk), ('rst', rst), ('conf_bus_in', conf_bus_in), ('instruction_we', conf_wr_en),
           ('instruction_addr', conf_wr_addr), ('instruction_data', conf_wr_data), ('const_we', init_const_we),
           ('const_waddr', init_conf_const_waddr), ('const_data', init_conf_const), ('pc_max', pc_max),
           ('pc_loop', pc_loop), ('store_ignore', ignore_until)
           ]

    m.Instance(conf_reader_pe, 'conf_reader_pe', param, con)

    param = []
    con = [('clk', clk), ('rst', rst), ('en', en), ('max', pc_max), ('loop', pc_loop), ('pc', pc_out)]
    m.Instance(pc, 'pc', param, con)

    inst_decode = make_inst_decode(cgra_id)
    param = []
    con_inst_decode = [('instruction', inst_mem_out), ('alu_op', alu_op), ('alu_a', alu_ina_addr),
                       ('alu_b', alu_inb_addr), ('alu_out', alu_out_addr), ('rf_raddr', rf_raddr),
                       ('rf_waddr', rf_waddr)]
    m.Instance(inst_decode, 'inst_decode', param, con_inst_decode)

    return m
