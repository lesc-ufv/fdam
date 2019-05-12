from math import ceil, log

from veriloggen import *

from common.make_cgra_counter import make_thread_counter, make_ignore_counter
from common.make_mux import make_mux
from fdam_cgra.make_alu import make_alu
from fdam_cgra.make_conf_reader_pe import make_conf_reader_pe
from fdam_cgra.make_inst_decode import make_inst_decode


def make_pe(cgra_id, type, num_thread, pc, memory, reg_pipe, data_width, conf_depth):
    rf_depth = 4
    op_width = 4
    mux2 = make_mux(2)
    mux_ignore = make_mux(num_thread)
    mux_pc = mux_ignore

    if type == 1:
        name = 'cgra%d_pe_in' % cgra_id
    elif type == 2:
        name = 'cgra%d_pe_out' % cgra_id
    else:
        name = 'cgra%d_pe' % cgra_id

    m = Module(name)

    PE_ID = m.Parameter('PE_ID', 0)
    genv = m.Genvar('genv')

    clk = m.Input('clk')
    rst = m.Input('rst')
    en = m.Input('en')
    branch_in = m.Input('branch_in')
    branch_out = m.Output('branch_out')
    conf_bus_in = m.Input('conf_bus_in', 64)

    ina = m.Input('ina', data_width)
    inb = m.Input('inb', data_width)
    outa = m.Output('outa', data_width)
    outb = m.Output('outb', data_width)
    en_reg = m.Wire('en_reg')
    alu_op_inst_dec = m.Wire('alu_op_inst_dec', op_width)
    alu_op = m.Wire('alu_op', op_width)
    mux_b_in = m.Wire('mux_b_in', data_width)
    mux_a_sel_inst_dec = m.Wire('mux_a_sel_inst_dec')
    mux_b_sel_inst_dec = m.Wire('mux_b_sel_inst_dec')
    mux_b_sel = m.Wire('mux_b_sel')
    rf_we_inst_dec = m.Wire('rf_we_inst_dec')
    rf_we = m.Wire('rf_we')
    rf_we_p = m.Wire('rf_we_p')
    rf_waddr_inst_dec = m.Wire('rf_waddr_inst_dec', rf_depth)
    rf_waddr = m.Wire('rf_waddr', rf_depth)
    rf_data_out = m.Wire('rf_data_out', data_width)
    rf_re_inst_dec = m.Wire('rf_re_inst_dec')
    rf_raddr_inst_dec = m.Wire('rf_raddr_inst_dec', rf_depth)
    rf_data_in = m.Wire('rf_data_in', data_width)
    fifo_we_inst_dec = m.Wire('fifo_we_inst_dec')
    fifo_re_inst_dec = m.Wire('fifo_re_inst_dec')
    out_a_en_inst_dec = m.Wire('out_a_en_inst_dec')
    out_a_en = m.Wire('out_a_en')
    out_b_en_inst_dec = m.Wire('out_b_en_inst_dec')
    out_b_en = m.Wire('out_b_en')
    pc_max_mem = m.Wire('pc_max_mem', conf_depth)
    pc_max_we = m.Wire('pc_max_we')
    pc_loop_mem = m.Wire('pc_loop_mem', conf_depth)
    pc_loop_we = m.Wire('pc_loop_we')
    ignore_mem = m.Wire('ignore_mem', 16)
    ignore_we = m.Wire('ignore_we')
    qtd_we_low = m.Wire('qtd_we_low')
    qtd_mem_low = m.Wire('qtd_mem_low', 32)
    qtd_we_high = m.Wire('qtd_we_high')
    qtd_mem_high = m.Wire('qtd_mem_high', 32)
    thread_id = m.Wire('thread_id', int(ceil(log(num_thread, 2))))
    thread_id_dec = m.Wire('thread_id_dec', num_thread)
    thread_idx = m.Wire('thread_idx', int(ceil(log(num_thread, 2))))
    thread_idx_p = m.Wire('thread_idx_p', int(ceil(log(num_thread, 2))))
    thread_idx_pp = m.Wire('thread_idx_pp', int(ceil(log(num_thread, 2))))
    thread_idx_pppp = m.Wire('thread_idx_pppp', int(ceil(log(num_thread, 2))))
    thread_idx_dec = m.Wire('thread_idx_dec', num_thread)
    conf_wr_addr = m.Wire('conf_wr_addr', conf_depth)
    conf_wr_data = m.Wire('conf_wr_data', 16)
    conf_wr_en = m.Wire('conf_wr_en')
    init_const_we = m.Wire('init_const_we')
    init_conf_const_waddr = m.Wire('init_conf_const_waddr', rf_depth)
    init_conf_const = m.Wire('init_conf_const', data_width)
    inst_mem_out = m.Wire('inst_mem_out', 16)
    branch_in_alu = m.Wire('branch_in_alu')
    branch_out_alu = m.Wire('branch_out_alu')
    pc_max = m.Wire('pc_max', conf_depth, num_thread)
    pc_loop = m.Wire('pc_loop', conf_depth, num_thread)
    ignore = m.Wire('ignore', 16, num_thread)
    pc_out = m.Wire('pc_out', conf_depth, num_thread)
    mux_pc_out = m.Wire('mux_pc_out', conf_depth)
    reg_alu_in_a_out = m.Wire('reg_alu_in_a_out', data_width)
    reg_alu_in_b_out = m.Wire('reg_alu_in_b_out', data_width)
    alu_out = m.Wire('alu_out', data_width)
    mux_rf_const_out = m.Wire('mux_rf_const_out', data_width)
    mux_rf_const_waddr_out = m.Wire('mux_rf_const_waddr_out', rf_depth + int(ceil(log(num_thread, 2))))

    if type == 1:
        fifo_data = m.Input('fifo_data', data_width)
        fifo_re = m.Output('fifo_re')
        fifo_event = fifo_re
        mux_a_out = m.Wire('mux_a_out', data_width)
        ignore_end_counter = m.Wire('ignore_end_counter')
        ignore_end_counters = m.Wire('ignore_end_counters', num_thread)
        thread_idx_dec_p = m.Wire('thread_idx_dec_p', num_thread)
        ignore_en = fifo_re_inst_dec
        thread_idx_dec_ignore_counter = thread_idx_dec_p
        thread_idx_ignore = thread_idx_p
        qtd_low = m.Wire('qtd_low', 32, num_thread)
        qtd_high = m.Wire('qtd_high', 32, num_thread)
        qtd_end_counters = m.Wire('qtd_end_counters', num_thread)

        m.EmbeddedCode('')
        fifo_re.assign(fifo_re_inst_dec & ignore_end_counter & en)
        param = [('WIDTH', data_width)]
        con = [('sel', mux_a_sel_inst_dec), ('in0', ina), ('in1', fifo_data), ('out', mux_a_out)]
        m.Instance(mux2, 'mux_a', param, con)

        param = [('NUM_STAGES', 1), ('DATA_WIDTH', thread_idx_dec.width)]
        con = [('clk', clk), ('rst', rst), ('en', en), ('in', thread_idx_dec), ('out', thread_idx_dec_p)]
        m.Instance(reg_pipe, 'reg_thread_idx_dec_ppp', param, con)

    elif type == 2:
        fifo_data = m.Output('fifo_data', data_width)
        fifo_we = m.Output('fifo_we')
        fifo_event = fifo_we
        mux_a_out = ina
        fifo_we_ignore_en = m.Wire('fifo_we_ignore_en')
        ignore_end_counter = m.Wire('ignore_end_counter')
        ignore_end_counters = m.Wire('ignore_end_counters', num_thread)
        we_trash = m.Wire("we_trash")
        thread_idx_dec_pppp = m.Wire('thread_idx_dec_pppp', num_thread)
        qtd_low = m.Wire('qtd_low', 32, num_thread)
        qtd_high = m.Wire('qtd_high', 32, num_thread)
        qtd_end_counters = m.Wire('qtd_end_counters', num_thread)
        ignore_en = fifo_we_ignore_en
        thread_idx_dec_ignore_counter = thread_idx_dec_pppp
        thread_idx_ignore = thread_idx_pppp
        m.EmbeddedCode('')
        fifo_we.assign(Or(AndList(fifo_we_ignore_en, ignore_end_counter, en), we_trash))
        we_trash.assign(And(Uand(qtd_end_counters), en))

        param = [('NUM_STAGES', 4), ('DATA_WIDTH', thread_idx_dec.width)]
        con = [('clk', clk), ('rst', rst), ('en', en), ('in', thread_idx_dec), ('out', thread_idx_dec_pppp)]
        m.Instance(reg_pipe, 'reg_thread_idx_dec_ppp', param, con)

        param = [('NUM_STAGES', 1), ('DATA_WIDTH', data_width)]
        con = [('clk', clk), ('rst', rst), ('en', en), ('in', alu_out), ('out', fifo_data)]
        m.Instance(reg_pipe, 'reg_fifo_data', param, con)

        param = [('NUM_STAGES', 3), ('DATA_WIDTH', 1)]
        con = [('clk', clk), ('rst', rst), ('en', en), ('in', fifo_we_inst_dec), ('out', fifo_we_ignore_en)]
        m.Instance(reg_pipe, 'reg_fifo_we_ignore_en', param, con)

    else:
        mux_a_out = ina
        ignore_end_counter = ''
        ignore_end_counters = ''
        ignore_en = ''
        thread_idx_dec_ignore_counter = ''
        thread_idx_ignore = ''
        fifo_event = ''
        qtd_low = ''
        qtd_high = ''
        qtd_end_counters = ''

    if type == 1 or type == 2:
        param = [('NUM_STAGES', 1), ('DATA_WIDTH', 16)]
        con = [('clk', clk), ('rst', rst), ('en', thread_id_dec[genv] & ignore_we), ('in', ignore_mem),
               ('out', ignore[genv])]
        genfor = m.GenerateFor(genv(0), genv < num_thread, genv.inc(), 'ignore_reg_inst')
        genfor.Instance(reg_pipe, 'ignore_reg', param, con)

        ignore_counter = make_ignore_counter()
        param = [('WIDTH', 16)]
        con = [('clk', clk), ('rst', rst), ('en', thread_idx_dec_ignore_counter[genv] & ignore_en & en),
               ('limit', ignore[genv]),
               ('end_counter', ignore_end_counters[genv])]

        genfor = m.GenerateFor(genv(0), genv < num_thread, genv.inc(), 'ignore_counter_inst')
        genfor.Instance(ignore_counter, 'ignore_counter', param, con)
        #####################################################################################################################

        param = [('NUM_STAGES', 1), ('DATA_WIDTH', 32)]
        con = [('clk', clk), ('rst', rst), ('en', thread_id_dec[genv] & qtd_we_low), ('in', qtd_mem_low),
               ('out', qtd_low[genv])]
        genfor = m.GenerateFor(genv(0), genv < num_thread, genv.inc(), 'qtd_low_reg_inst')
        genfor.Instance(reg_pipe, 'qtd_low_reg', param, con)

        param = [('NUM_STAGES', 1), ('DATA_WIDTH', 32)]
        con = [('clk', clk), ('rst', rst), ('en', thread_id_dec[genv] & qtd_we_high), ('in', qtd_mem_high),
               ('out', qtd_high[genv])]
        genfor = m.GenerateFor(genv(0), genv < num_thread, genv.inc(), 'qtd_high_reg_inst')
        genfor.Instance(reg_pipe, 'qtd_high_reg', param, con)

        qtd_counter = make_ignore_counter()
        param = [('WIDTH', 64)]
        con = [('clk', clk), ('rst', rst), ('en', thread_idx_dec_ignore_counter[genv] & fifo_event),
               ('limit', Cat(qtd_high[genv], qtd_low[genv])),
               ('end_counter', qtd_end_counters[genv])]

        genfor = m.GenerateFor(genv(0), genv < num_thread, genv.inc(), 'qtd_counter_inst')
        genfor.Instance(qtd_counter, 'qtd_counter', param, con)

        #####################################################################################################################
        param = [('WIDTH', 1)]
        con1 = [('in%d' % i, ignore_end_counters[i] & ~qtd_end_counters[i]) for i in range(num_thread)]
        con = [('sel', thread_idx_ignore), ('out', ignore_end_counter)]
        m.Instance(mux_ignore, 'mux_ignore_end_counter', param, con + con1)

    thread_idx_dec.assign(Sll(1, thread_idx))
    thread_id_dec.assign(Sll(1, thread_id))

    param = [('NUM_STAGES', 1), ('DATA_WIDTH', conf_depth)]
    con = [
        ('clk', clk), ('rst', rst), ('en', thread_id_dec[genv] & pc_max_we), ('in', pc_max_mem),
        ('out', pc_max[genv])]
    genfor = m.GenerateFor(genv(0), genv < num_thread, genv.inc(), 'pc_max_reg_inst')
    genfor.Instance(reg_pipe, 'pc_max_reg', param, con)

    param = [('NUM_STAGES', 1), ('DATA_WIDTH', conf_depth)]
    con = [('clk', clk), ('rst', rst), ('en', thread_id_dec[genv] & pc_loop_we), ('in', pc_loop_mem),
           ('out', pc_loop[genv])]
    genfor = m.GenerateFor(genv(0), genv < num_thread, genv.inc(), 'pc_loop_reg_inst')
    genfor.Instance(reg_pipe, 'pc_loop_reg', param, con)

    param = [('WIDTH', mux_pc_out.width)]
    con1 = [('in%d' % i, pc_out[i]) for i in range(num_thread)]
    con = [('sel', thread_idx), ('out', mux_pc_out)]
    m.Instance(mux_pc, 'mux_pc', param, con + con1)

    param = []
    con = [
        ('clk', clk), ('rst', rst), ('en', thread_idx_dec[genv] & en), ('max', pc_max[genv]),
        ('loop', pc_loop[genv]), ('pc', pc_out[genv])
    ]
    genfor = m.GenerateFor(genv(0), genv < num_thread, genv.inc(), 'pc_inst')
    genfor.Instance(pc, 'pc', param, con)

    param = [('DATA_WIDTH', 16), ('ADDR_WIDTH', conf_depth + int(ceil(log(num_thread, 2))))]
    con = [
        ('clk', clk), ('we', conf_wr_en), ('re', en), ('raddr', Cat(thread_idx, mux_pc_out)),
        ('waddr', Cat(thread_id, conf_wr_addr)), ('din', conf_wr_data), ('dout', inst_mem_out)
    ]
    m.Instance(memory, 'inst_mem', param, con)

    param = [('NUM_STAGES', 1), ('DATA_WIDTH', 1)]
    con = [('clk', clk), ('rst', rst), ('en', en), ('in', rf_we), ('out', rf_we_p)]
    m.Instance(reg_pipe, 'reg_rf_we_p', param, con)

    param = [('NUM_STAGES', 3), ('DATA_WIDTH', rf_waddr_inst_dec.width)]
    con = [('clk', clk), ('rst', rst), ('en', en), ('in', rf_waddr_inst_dec), ('out', rf_waddr)]
    m.Instance(reg_pipe, 'reg_rf_waddr', param, con)

    param = [('WIDTH', rf_depth + int(ceil(log(num_thread, 2))))]
    con = [
        ('sel', init_const_we), ('in0', Cat(thread_idx_pppp, rf_waddr)),
        ('in1', Cat(thread_id, init_conf_const_waddr)),
        ('out', mux_rf_const_waddr_out)
    ]
    m.Instance(mux2, 'mux_rf_const_waddr', param, con)

    param = [('DATA_WIDTH', data_width), ('ADDR_WIDTH', rf_depth + int(ceil(log(num_thread, 2))))]
    con = [
        ('clk', clk), ('we', (rf_we_p & en) | init_const_we), ('re', rf_re_inst_dec),
        ('raddr', Cat(thread_idx_p, rf_raddr_inst_dec)),
        ('waddr', mux_rf_const_waddr_out), ('din', mux_rf_const_out), ('dout', rf_data_out)
    ]
    m.Instance(memory, 'rf', param, con)

    param = [('WIDTH', data_width)]
    con = [('sel', init_const_we), ('in0', rf_data_in), ('in1', init_conf_const), ('out', mux_rf_const_out)]
    m.Instance(mux2, 'mux_rf_const', param, con)

    param = [('NUM_STAGES', 1), ('DATA_WIDTH', 1)]
    con = [('clk', clk), ('rst', rst), ('en', en), ('in', mux_b_sel_inst_dec), ('out', mux_b_sel)]
    m.Instance(reg_pipe, 'reg_mux_b_sel', param, con)

    param = [('WIDTH', data_width)]
    con = [('sel', mux_b_sel), ('in0', mux_b_in), ('in1', rf_data_out), ('out', reg_alu_in_b_out)]
    m.Instance(mux2, 'mux_b', param, con)

    param = [('NUM_STAGES', 1), ('DATA_WIDTH', data_width)]
    con = [('clk', clk), ('rst', rst), ('en', en), ('in', mux_a_out), ('out', reg_alu_in_a_out)]
    m.Instance(reg_pipe, 'reg_alu_in_a', param, con)

    param = [('NUM_STAGES', 1), ('DATA_WIDTH', data_width)]
    con = [('clk', clk), ('rst', rst), ('en', en), ('in', inb), ('out', mux_b_in)]
    m.Instance(reg_pipe, 'reg_alu_in_b', param, con)

    param = [('NUM_STAGES', 1), ('DATA_WIDTH', alu_op.width)]
    con = [('clk', clk), ('rst', rst), ('en', en), ('in', alu_op_inst_dec), ('out', alu_op)]
    m.Instance(reg_pipe, 'reg_alu_op', param, con)

    param = [('NUM_STAGES', 1), ('DATA_WIDTH', 1)]
    con = [('clk', clk), ('rst', rst), ('en', en), ('in', branch_in), ('out', branch_in_alu)]
    m.Instance(reg_pipe, 'reg_branch_in', param, con)

    param = [('NUM_STAGES', 1), ('DATA_WIDTH', 1)]
    con = [('clk', clk), ('rst', rst), ('en', en), ('in', branch_out_alu), ('out', branch_out)]
    m.Instance(reg_pipe, 'reg_branch_out', param, con)

    param = [('NUM_STAGES', 1), ('DATA_WIDTH', thread_idx.width)]
    con = [('clk', clk), ('rst', rst), ('en', en), ('in', thread_idx_p), ('out', thread_idx_pp)]
    m.Instance(reg_pipe, 'reg_thread_idx_pp', param, con)

    param = [('NUM_STAGES', 1), ('DATA_WIDTH', thread_idx.width)]
    con = [('clk', clk), ('rst', rst), ('en', en), ('in', thread_idx), ('out', thread_idx_p)]
    m.Instance(reg_pipe, 'reg_thread_idx_p', param, con)

    param = [('NUM_STAGES', 2), ('DATA_WIDTH', thread_idx.width)]
    con = [('clk', clk), ('rst', rst), ('en', en), ('in', thread_idx_pp), ('out', thread_idx_pppp)]
    m.Instance(reg_pipe, 'reg_thread_idx_pppp', param, con)

    alu = make_alu(cgra_id)
    param = [('DATA_WIDTH', data_width)]
    con = [
        ('clk', clk), ('op', alu_op), ('branch_in', branch_in_alu), ('branch_out', branch_out_alu),
        ('ina', reg_alu_in_a_out), ('inb', reg_alu_in_b_out), ('out', alu_out)
    ]
    m.Instance(alu, 'alu', param, con)

    param = [('NUM_STAGES', 2), ('DATA_WIDTH', 1)]
    con = [('clk', clk), ('rst', rst), ('en', en), ('in', out_a_en_inst_dec), ('out', out_a_en)]
    m.Instance(reg_pipe, 'reg_out_a_en', param, con)

    param = [('NUM_STAGES', 1), ('DATA_WIDTH', data_width)]
    con = [('clk', clk), ('rst', rst), ('en', out_a_en & en), ('in', alu_out), ('out', outa)]
    m.Instance(reg_pipe, 'reg_out_a', param, con)

    param = [('NUM_STAGES', 2), ('DATA_WIDTH', 1)]
    con = [('clk', clk), ('rst', rst), ('en', en), ('in', out_b_en_inst_dec), ('out', out_b_en)]
    m.Instance(reg_pipe, 'reg_out_b_en', param, con)

    param = [('NUM_STAGES', 1), ('DATA_WIDTH', data_width)]
    con = [('clk', clk), ('rst', rst), ('en', out_b_en & en), ('in', alu_out), ('out', outb)]
    m.Instance(reg_pipe, 'reg_out_b', param, con)

    param = [('NUM_STAGES', 2), ('DATA_WIDTH', 1)]
    con = [('clk', clk), ('rst', rst), ('en', en), ('in', rf_we_inst_dec), ('out', rf_we)]
    m.Instance(reg_pipe, 'reg_rf_we_en', param, con)

    param = [('NUM_STAGES', 1), ('DATA_WIDTH', data_width)]
    con = [('clk', clk), ('rst', rst), ('en', rf_we), ('in', alu_out), ('out', rf_data_in)]
    m.Instance(reg_pipe, 'reg_rf_data_in', param, con)

    conf_reader_pe = make_conf_reader_pe(cgra_id, num_thread, data_width, 16, conf_depth)
    param = [('PE_ID', PE_ID)]
    con = [
        ('clk', clk), ('rst', rst), ('conf_bus_in', conf_bus_in), ('instruction_we', conf_wr_en),
        ('instruction_addr', conf_wr_addr), ('instruction_data', conf_wr_data), ('const_we', init_const_we),
        ('const_waddr', init_conf_const_waddr), ('const_data', init_conf_const), ('pc_max', pc_max_mem),
        ('pc_max_we', pc_max_we), ('pc_loop', pc_loop_mem), ('pc_loop_we', pc_loop_we), ('ignore_data', ignore_mem),
        ('ignore_we', ignore_we), ('qtd_low', qtd_mem_low), ('qtd_we_low', qtd_we_low), ('qtd_high', qtd_mem_high),
        ('qtd_we_high', qtd_we_high),
        ('thread_id', thread_id)
    ]
    m.Instance(conf_reader_pe, 'conf_reader_pe', param, con)

    thread_counter = make_thread_counter(num_thread)
    param = []
    con = [('clk', clk), ('rst', rst), ('en', en), ('out', thread_idx)]
    m.Instance(thread_counter, 'thread_counter', param, con)

    param = [('NUM_STAGES', 1), ('DATA_WIDTH', 1)]
    con = [('clk', clk), ('rst', rst), ('en', en), ('in', en), ('out', en_reg)]
    m.Instance(reg_pipe, 'reg_en', param, con)

    inst_decode = make_inst_decode(cgra_id)
    param = []
    con_inst_decode = [
        ('instruction', Mux(en_reg, inst_mem_out, Int(0, 16, 10))), ('alu_op', alu_op_inst_dec),
        ('mux_a', mux_a_sel_inst_dec), ('mux_b', mux_b_sel_inst_dec), ('rf_re', rf_re_inst_dec),
        ('rf_we', rf_we_inst_dec), ('fifo_re', fifo_re_inst_dec), ('fifo_we', fifo_we_inst_dec),
        ('out_a_en', out_a_en_inst_dec), ('out_b_en', out_b_en_inst_dec), ('rf_raddr', rf_raddr_inst_dec),
        ('rf_waddr', rf_waddr_inst_dec)
    ]
    m.Instance(inst_decode, 'inst_decode', param, con_inst_decode)

    return m
