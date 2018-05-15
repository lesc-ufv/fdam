from veriloggen import *

from src.make_arbiter import make_arbiter
from src.make_fifo import make_fifo
from src.make_select import make_select_tree
from src.utils import numBits


def make_arbiter_controller_rd_req_tree(radix, num_input):
    array = []
    array = make_arbiter_controller_tree_array(radix, num_input, array)
    m = Module('arbiter_controller_rd_req_tree_%d' % num_input)
    DATA_WIDTH = m.Parameter('DATA_WIDTH', 32)
    m.Parameter('INPUT_FIFO_DEPTH_BITS', 4)
    m.Parameter('OUTPUT_FIFO_DEPTH_BITS', 4)

    clk = m.Input('clk')
    rst = m.Input('rst')

    req_wr_en_in = m.Input('req_wr_en_in', num_input)
    req_wr_data_in = m.Input('req_wr_data_in', EmbeddedCode('DATA_WIDTH * %d' % num_input))
    req_wr_available_in = m.Output('req_wr_available_in', num_input)

    req_wr_available_out = m.Input('req_wr_available_out')
    req_wr_en_out = m.Output('req_wr_en_out')
    req_wr_data_out = m.Output('req_wr_data_out', DATA_WIDTH)

    mods = {}
    if len(array) == 1:
        con = [('clk', clk), ('rst', rst), ('req_wr_en_in', req_wr_en_in), ('req_wr_data_in', req_wr_data_in),
               ('req_wr_available_in', req_wr_available_in), ('req_wr_available_out', req_wr_available_out),
               ('req_wr_en_out', req_wr_en_out), ('req_wr_data_out', req_wr_data_out)]
        if not num_input in mods.keys():
            mods[num_input] = make_arbiter_controller_rd_req(num_input)
        m.Instance(mods[num_input], 'arbiter_controller_rd_req_%d_%d' % (0, 0), m.get_params(), con)
    else:
        map_req_wr_available_out = {}
        map_req_wr_en_out = {}
        map_req_wr_data_out = {}
        for i in range(len(array) - 1):
            c = 0
            p = i + 1
            for j in range(len(array[p])):
                c = c + array[p][j]
            map_req_wr_available_out[i] = m.Wire('req_wr_available_out_%d' % i, c)
            map_req_wr_en_out[i] = m.Wire('req_wr_en_out_%d' % i, c)
            map_req_wr_data_out[i] = m.Wire('req_wr_data_out_%d' % i, EmbeddedCode('DATA_WIDTH*%d' % c))
        for i in range(len(array) - 1):
            end_in = 0
            id_out = 0
            for j in range(len(array[i])):
                begin_in = end_in
                end_in = end_in + array[i][j]
                if i == 0:
                    data_range_in = 'DATA_WIDTH*%d - 1:DATA_WIDTH*%d' % (end_in, begin_in)
                    data_range_out = 'DATA_WIDTH*%d - 1:DATA_WIDTH*%d' % (id_out + 1, id_out)
                    con = [('clk', clk),
                           ('rst', rst),
                           ('req_wr_en_in', req_wr_en_in[begin_in:end_in]),
                           ('req_wr_data_in', req_wr_data_in[EmbeddedCode(data_range_in)]),
                           ('req_wr_available_in', req_wr_available_in[begin_in:end_in]),
                           ('req_wr_available_out', map_req_wr_available_out[i][id_out]),
                           ('req_wr_en_out', map_req_wr_en_out[i][id_out]),
                           ('req_wr_data_out', map_req_wr_data_out[i][EmbeddedCode(data_range_out)])
                           ]
                else:
                    data_range_in = 'DATA_WIDTH*%d - 1:DATA_WIDTH*%d' % (end_in, begin_in)
                    data_range_out = 'DATA_WIDTH*%d - 1:DATA_WIDTH*%d' % (id_out + 1, id_out)
                    con = [('clk', clk),
                           ('rst', rst),
                           ('req_wr_en_in', map_req_wr_en_out[i - 1][begin_in:end_in]),
                           ('req_wr_data_in', map_req_wr_data_out[i - 1][EmbeddedCode(data_range_in)]),
                           ('req_wr_available_in', map_req_wr_available_out[i - 1][begin_in:end_in]),
                           ('req_wr_available_out', map_req_wr_available_out[i][id_out]),
                           ('req_wr_en_out', map_req_wr_en_out[i][id_out]),
                           ('req_wr_data_out', map_req_wr_data_out[i][EmbeddedCode(data_range_out)])
                           ]
                if not array[i][j] in mods.keys():
                    mods[array[i][j]] = make_arbiter_controller_rd_req(array[i][j])
                m.Instance(mods[array[i][j]], 'arbiter_controller_rd_req_%d_%d' % (i, j), m.get_params(), con)
                id_out = id_out + 1

        con = [('clk', clk),
               ('rst', rst),
               ('req_wr_en_in', map_req_wr_en_out[len(array) - 2]),
               ('req_wr_data_in', map_req_wr_data_out[len(array) - 2]),
               ('req_wr_available_in', map_req_wr_available_out[len(array) - 2]),
               ('req_wr_available_out', req_wr_available_out),
               ('req_wr_en_out', req_wr_en_out), ('req_wr_data_out', req_wr_data_out)]
        if not array[len(array) - 1][0] in mods.keys():
            mods[array[len(array) - 1][0]] = make_arbiter_controller_rd_req(array[len(array) - 1][0])
        m.Instance(mods[array[len(array) - 1][0]], 'arbiter_controller_rd_req_%d_%d' % (len(array) - 1, 0),
                   m.get_params(),
                   con)
    return m


def make_arbiter_controller_tree_array(radix, num_input, array):
    if radix < 2:
        raise Exception("The radix parameter needs greater than 1, found: %d" % radix)
    m_array = []
    while num_input > radix:
        m_array.append(radix)
        num_input = num_input - radix
    else:
        m_array.append(num_input)

    array.append(m_array)
    if len(m_array) == 1:
        return array
    else:
        return make_arbiter_controller_tree_array(radix, len(m_array), array)


def make_arbiter_controller_rd_req(num_input):
    m = Module('arbiter_controller_rd_req_%d' % num_input)
    NUM_INPUT = num_input
    DATA_WIDTH = m.Parameter('DATA_WIDTH', 32)
    INPUT_FIFO_DEPTH_BITS = m.Parameter('INPUT_FIFO_DEPTH_BITS', 4)
    OUTPUT_FIFO_DEPTH_BITS = m.Parameter('OUTPUT_FIFO_DEPTH_BITS', 4)
    clk = m.Input('clk')
    rst = m.Input('rst')

    req_wr_en_in = m.Input('req_wr_en_in', NUM_INPUT)
    req_wr_data_in = m.Input('req_wr_data_in', EmbeddedCode('DATA_WIDTH * %d' % NUM_INPUT))
    req_wr_available_in = m.OutputReg('req_wr_available_in', NUM_INPUT)

    req_wr_available_out = m.Input('req_wr_available_out')
    req_wr_en_out = m.OutputReg('req_wr_en_out')
    req_wr_data_out = m.OutputReg('req_wr_data_out', DATA_WIDTH)

    IN_FIFO_ALMOSTFULL_THRESHOLD = m.Localparam('IN_FIFO_ALMOSTFULL_THRESHOLD',
                                                EmbeddedCode('2 ** INPUT_FIFO_DEPTH_BITS - 6'))
    IN_FIFO_ALMOSTEMPTY_THRESHOLD = m.Localparam('IN_FIFO_ALMOSTEMPTY_THRESHOLD', 4)
    m.EmbeddedCode('')
    OUT_FIFO_ALMOSTFULL_THRESHOLD = m.Localparam('OUT_FIFO_ALMOSTFULL_THRESHOLD',
                                                 EmbeddedCode('2 ** OUTPUT_FIFO_DEPTH_BITS - 6'))
    OUT_FIFO_ALMOSTEMPTY_THRESHOLD = m.Localparam('OUT_FIFO_ALMOSTEMPTY_THRESHOLD', 2)
    m.EmbeddedCode('')
    in_fifo_empty = m.Wire('in_fifo_empty', NUM_INPUT)
    in_fifo_count = m.Wire('in_fifo_count', EmbeddedCode('(INPUT_FIFO_DEPTH_BITS + 1) * %d' % NUM_INPUT))
    in_fifo_full = m.Wire('in_fifo_full', NUM_INPUT)
    in_fifo_almostfull = m.Wire('in_fifo_almostfull', NUM_INPUT)
    in_fifo_almostempty = m.Wire('in_fifo_almostempty', NUM_INPUT)
    m.EmbeddedCode('')
    rst_reg = m.Reg('rst_reg')
    m.EmbeddedCode('')
    in_fifo_we = m.Reg('in_fifo_we', NUM_INPUT)
    in_fifo_din = m.Reg('in_fifo_din', DATA_WIDTH * NUM_INPUT)
    in_fifo_re_flag = m.Reg('in_fifo_re_flag', 2)
    m.EmbeddedCode('')
    in_fifo_re = m.Reg('in_fifo_re', NUM_INPUT)
    in_fifo_dout_valid = m.Wire('in_fifo_dout_valid', NUM_INPUT)
    in_fifo_dout = m.Wire('in_fifo_dout', EmbeddedCode('DATA_WIDTH*%d' % NUM_INPUT))
    m.EmbeddedCode('')
    out_fifo_empty = m.Wire('out_fifo_empty')
    out_fifo_count = m.Wire('out_fifo_count', OUTPUT_FIFO_DEPTH_BITS + 1)
    out_fifo_full = m.Wire('out_fifo_full')
    out_fifo_almostfull = m.Wire('out_fifo_almostfull')
    out_fifo_almostempty = m.Wire('out_fifo_almostempty')
    m.EmbeddedCode('')
    out_fifo_we = m.Reg('out_fifo_we')
    out_fifo_din = m.Reg('out_fifo_din', DATA_WIDTH)
    m.EmbeddedCode('')
    out_fifo_re = m.Reg('out_fifo_re')
    out_fifo_dout_valid = m.Wire('out_fifo_dout_valid')
    out_fifo_dout = m.Wire('out_fifo_dout', DATA_WIDTH)
    m.EmbeddedCode('')
    arbiter_request = m.Wire('arbiter_request', NUM_INPUT)
    arbiter_grant = m.Wire('arbiter_grant', NUM_INPUT)
    arbiter_grant_valid = m.Wire('arbiter_grant_valid')
    arbiter_grant_encoded = m.Wire('arbiter_grant_encoded', numBits(NUM_INPUT))
    arbiter_request_tt = m.Wire('arbiter_request_tt', NUM_INPUT)
    arbiter_grant_tt = m.Wire('arbiter_grant_tt', NUM_INPUT)
    arbiter_grant_valid_tt = m.Wire('arbiter_grant_valid_tt')
    arbiter_grant_encoded_tt = m.Wire('arbiter_grant_encoded_tt', numBits(NUM_INPUT))
    m.EmbeddedCode('')
    select_dout_valid = m.Wire('select_dout_valid')
    select_dout = m.Wire('select_dout', DATA_WIDTH)
    m.EmbeddedCode('')
    it = m.Integer('it')
    idx = m.Genvar('idx')

    fifo = make_fifo()
    params = [('FIFO_WIDTH', DATA_WIDTH), ('FIFO_DEPTH_BITS', INPUT_FIFO_DEPTH_BITS),
              ('FIFO_ALMOSTFULL_THRESHOLD', IN_FIFO_ALMOSTFULL_THRESHOLD),
              ('FIFO_ALMOSTEMPTY_THRESHOLD', IN_FIFO_ALMOSTEMPTY_THRESHOLD)]
    con = [('clk', clk), ('rst', rst_reg), ('we', in_fifo_we[idx]),
           ('din', in_fifo_din[idx * (DATA_WIDTH):idx * (DATA_WIDTH) + (DATA_WIDTH)]),
           ('re', in_fifo_re[idx]), ('valid', in_fifo_dout_valid[idx]),
           ('dout', in_fifo_dout[idx * (DATA_WIDTH):idx * (DATA_WIDTH) + (DATA_WIDTH)]),
           ('count', in_fifo_count[idx * (INPUT_FIFO_DEPTH_BITS + 1):idx * (INPUT_FIFO_DEPTH_BITS + 1) + (
                   INPUT_FIFO_DEPTH_BITS + 1)]),
           ('empty', in_fifo_empty[idx]), ('full', in_fifo_full[idx]), ('almostfull', in_fifo_almostfull[idx]),
           ('almostempty', in_fifo_almostempty[idx])]
    genReqRdFifos = m.GenerateFor(idx(0), idx < NUM_INPUT, idx.inc(), 'gen_in_fifos')
    genReqRdFifos.Instance(fifo, 'in_fifo', params, con)

    select = make_select_tree(4, NUM_INPUT)
    params = [('DATA_WIDTH', DATA_WIDTH)]
    con = [('clk', clk), ('rst', rst_reg), ('data_in_valid', in_fifo_dout_valid)]
    for i in range(NUM_INPUT):
        ran = '(DATA_WIDTH+DATA_WIDTH*%d)-1:DATA_WIDTH*%d' % (i, i)
        con.append(('data_in_%d' % i, in_fifo_dout[EmbeddedCode(ran)]))
    con.append(('data_out_valid', select_dout_valid))
    con.append(('data_out', select_dout))
    m.Instance(select, 'select', params, con)

    params = [('FIFO_WIDTH', DATA_WIDTH), ('FIFO_DEPTH_BITS', OUTPUT_FIFO_DEPTH_BITS),
              ('FIFO_ALMOSTFULL_THRESHOLD', OUT_FIFO_ALMOSTFULL_THRESHOLD),
              ('FIFO_ALMOSTEMPTY_THRESHOLD', OUT_FIFO_ALMOSTEMPTY_THRESHOLD)]
    con = [('clk', clk), ('rst', rst_reg), ('we', out_fifo_we), ('din', out_fifo_din), ('re', out_fifo_re),
           ('valid', out_fifo_dout_valid),
           ('dout', out_fifo_dout), ('count', out_fifo_count), ('empty', out_fifo_empty), ('full', out_fifo_full),
           ('almostfull', out_fifo_almostfull),
           ('almostempty', out_fifo_almostempty)]
    m.Instance(fifo, 'out_fifo', params, con)

    arbiter = make_arbiter()
    params = [('PORTS', NUM_INPUT), ('TYPE', "ROUND_ROBIN"), ('BLOCK', "NONE"), ('LSB_PRIORITY', "LOW")]
    con = [('clk', clk), ('rst', rst_reg), ('request', arbiter_request),
           ('acknowledge', Repeat(Int(0, 1, 2), NUM_INPUT)),
           ('grant', arbiter_grant), ('grant_valid', arbiter_grant_valid),
           ('grant_encoded', arbiter_grant_encoded)]
    m.Instance(arbiter, 'arbiter', params, con)
    params = [('PORTS', NUM_INPUT), ('TYPE', "ROUND_ROBIN"), ('BLOCK', "NONE"), ('LSB_PRIORITY', "LOW")]
    con = [('clk', clk), ('rst', rst_reg), ('request', arbiter_request_tt),
           ('acknowledge', Repeat(Int(0, 1, 2), NUM_INPUT)),
           ('grant', arbiter_grant_tt), ('grant_valid', arbiter_grant_valid_tt),
           ('grant_encoded', arbiter_grant_encoded_tt)]
    m.Instance(arbiter, 'arbiter_tt', params, con)

    gen = m.GenerateFor(idx(0), idx < NUM_INPUT, idx.inc(), 'gen_req')
    gen.Assign(arbiter_request[idx](~in_fifo_empty[idx] & ~out_fifo_almostfull))
    gen.Assign(arbiter_request_tt[idx](~in_fifo_almostempty[idx] & ~out_fifo_almostfull))

    m.Always(Posedge(clk))(
        rst_reg(rst)
    )

    m.Always(Posedge(clk))(
        If(rst_reg)(
            req_wr_available_in(Int(0, NUM_INPUT, 10))
        ).Else(
            For(it(0), it < NUM_INPUT, it.inc())(
                req_wr_available_in[it](~in_fifo_almostfull[it])
            )
        )
    )
    m.Always(Posedge(clk))(
        If(rst_reg)(
            in_fifo_we(Int(0, NUM_INPUT, 10)),
            in_fifo_din(Repeat(Int(0, 1, 2), EmbeddedCode('DATA_WIDTH*%d' % NUM_INPUT))),
            in_fifo_re((Int(0, NUM_INPUT, 10))),
            out_fifo_we(Int(0, 1, 2)),
            out_fifo_din(Repeat(Int(0, 1, 2), DATA_WIDTH)),
            out_fifo_re(Int(0, 1, 2)),
            req_wr_en_out(Int(0, 1, 2)),
            req_wr_data_out(Repeat(Int(0, 1, 2), DATA_WIDTH)),
            in_fifo_re_flag(Int(0, 2, 10))
        ).Else(
            in_fifo_we(req_wr_en_in),
            in_fifo_din(req_wr_data_in),
            out_fifo_we(select_dout_valid),
            out_fifo_din(select_dout),
            out_fifo_re(
                Mux(out_fifo_almostempty, req_wr_available_out & ~out_fifo_empty & ~out_fifo_re, req_wr_available_out)),
            req_wr_en_out(out_fifo_dout_valid),
            req_wr_data_out(out_fifo_dout),
            in_fifo_re((Int(0, NUM_INPUT, 10))),
            If(arbiter_grant_valid_tt)(
                in_fifo_re(arbiter_grant_tt),
            ),
            If(AndList(~arbiter_grant_valid_tt, arbiter_grant_valid, in_fifo_re_flag == Int(0, 2, 10)))(
                in_fifo_re(arbiter_grant),
                in_fifo_re_flag(Int(2, 2, 10))
            ),
            If(in_fifo_re_flag > 0)(
                in_fifo_re_flag(in_fifo_re_flag - Int(1, 2, 10))
            )
        )
    )
    return m

# make_arbiter_controller_tree(4, 32).to_verilog('../arbiter_controller_test')
