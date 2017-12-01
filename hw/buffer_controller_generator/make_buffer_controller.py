from src_buffer_control.make_arbiter import make_arbiter
from src_buffer_control.make_fifo import make_fifo
from src_buffer_control.util import numBits
from veriloggen import *

from hw.buffer_controller_generator.make_interface import make_interface


def make_buffer_controller(cache_data_width, fifo_depth, interfaces):
    # verificação da possibilidade de construção do circuito
    qtde_fifo_in = 0
    qtde_fifo_out = 0
    if (len(interfaces) < 1):
        print('No mínimo uma interface deve ser criada!')
        print('Processo abortado.')
        exit()
    for i in interfaces:
        if (i[0] == 0 or i[1] == 0):
            print('As quantidades de entradas ou de saídas das interfaces não podem ser 0!')
            print('Processo abortado.')
            exit()
        qtde_fifo_in = qtde_fifo_in + i[0]
        qtde_fifo_out = qtde_fifo_out + i[1]
    if (qtde_fifo_in > 8 or qtde_fifo_out > 8):
        print('As quantidades de entradas ou de saídas não podem ultrapassar 8!')
        print('Processo abortado.')
        exit()
    else:
        print('Circuito construído com sucesso!')

    addr_width = 64
    mdata_width = 16
    max_counter_read_bits = 64
    num_conf = 4
    num_pointers_in = qtde_fifo_in
    tam_vec_data_in = qtde_fifo_in * addr_width
    num_pointers_out = qtde_fifo_out
    tam_vec_data_out = qtde_fifo_out * addr_width
    params = []

    m = Module('buffer_controller')
    clk = m.Input('clk')
    rst = m.Input('rst')
    rst_parc = m.Input('rst_parc')
    start = m.Input('start')
    # Endereço da linha de cache com as configurações
    workspace_addr_base = m.Input('workspace_addr_base', addr_width)
    # ---------------------------------

    # Reading interface
    req_rd_en = m.OutputReg('req_rd_en')
    req_rd_addr = m.OutputReg('req_rd_addr', addr_width)
    req_rd_mdata = m.OutputReg('req_rd_mdata', mdata_width)
    req_rd_available = m.Input('req_rd_available')
    resp_rd_valid = m.Input('resp_rd_valid')
    resp_rd_data = m.Input('resp_rd_data', cache_data_width)
    resp_rd_mdata = m.Input('resp_rd_mdata', mdata_width)
    # ---------------------------------

    # Writing interface
    req_wr_available = m.Input('req_wr_available')
    req_wr_en = m.OutputReg('req_wr_en')
    req_wr_addr = m.OutputReg('req_wr_addr', addr_width)
    req_wr_mdata = m.OutputReg('req_wr_mdata', mdata_width)
    req_wr_data = m.OutputReg('req_wr_data', cache_data_width)
    resp_wr_valid = m.Input('resp_wr_valid')
    resp_wr_mdata = m.Input('resp_wr_mdata', mdata_width)
    # ---------------------------------

    # Registrador para pipeline do dado a ser enfileirado nas filas de entrada
    resp_rd_data_tmp = m.Reg('resp_rd_data_tmp', cache_data_width)

    # DONE ******ALTERADO... AGORA O DONE SERÁ ESCRITO NA DSM - SHOW!
    # done = m.Output('done')
    done = m.Wire('done')
    # ---------------------------------

    # Gerenciamento dos dones de leitura e escrita para o circuito
    # Apalavra é constituída por:
    # bit 0 - done total
    # proximos Nbits - término de leitura das fifos_in
    # próximos Mbits- término do processamento das interfaces
    dsm_word = m.Reg('dsm_word', cache_data_width)
    dsm_word_tmp = m.Reg('dsm_word_tmp', cache_data_width)

    # Gerenciamento dos Done signals
    done_uut_interface = m.Wire('done_uut_interface', len(interfaces))
    done_read = m.Wire('done_read', qtde_fifo_in)
    done_write_back = m.Wire('done_write_back', qtde_fifo_out)

    # Parâmetro da fifo_almost_full
    FIFO_IN_FULL = m.Localparam('FIFO_IN_FULL', pow(2, fifo_depth))
    # ---------------------------------

    # controle da fsm de leitura de dados e configurações
    FSM_RD_REQ_READ_CONF = m.Localparam('FSM_RD_REQ_READ_CONF', 0)
    FSM_RD_REQ_READ_DATA = m.Localparam('FSM_RD_REQ_READ_DATA', 1)
    fsm_rd = m.Reg('fsm_rd', 2)
    # ---------------------------------

    # controle da fsm de escrita de dados
    FSM_WR_REQ_IDLE = m.Localparam('FSM_WR_REQ_IDLE', 0)
    FSM_WR_REQ_WRITE_DATA = m.Localparam('FSM_WR_REQ_WRITE_DATA', 1)
    fsm_wr = m.Reg('fsm_wr', 2)
    # ---------------------------------

    # registradores onde ficarão armazenados os endereços,
    # as quantidades de dados para cada fila de entrada e para cada fila de saída
    # control for conf
    confs = m.Reg('confs', cache_data_width, num_conf)
    counter_received_conf = m.Reg('counter_received_conf', numBits(num_conf) + 1)
    addr_offset_conf = m.Reg('addr_offset_conf', numBits(num_conf) + 1)
    # ---------------------------------

    # control for data_in
    qtde_data_in_all = m.Wire('qtde_data_in_all', tam_vec_data_in)
    addr_in_all = m.Wire('addr_in_all', tam_vec_data_in)
    counter_received_data_in = m.Reg('counter_received_data_in', max_counter_read_bits, num_pointers_in)
    addr_offset_data_in = m.Reg('addr_offset_data_in', max_counter_read_bits, num_pointers_in)
    qtde_data_in = m.Wire('qtde_data_in', max_counter_read_bits, num_pointers_in)
    addr_in = m.Wire('addr_in', max_counter_read_bits, num_pointers_in)
    # ---------------------------------

    # control for data_out
    qtde_data_out_all = m.Wire('qtde_data_out_all', tam_vec_data_out)
    addr_out_all = m.Wire('addr_out_all', tam_vec_data_out)
    counter_sent_data_out = m.Reg('counter_sent_data_out', max_counter_read_bits, num_pointers_out)
    addr_offset_data_out = m.Reg('addr_offset_data_out', max_counter_read_bits, num_pointers_out)
    qtde_data_out = m.Wire('qtde_data_out', max_counter_read_bits, num_pointers_out)
    addr_out = m.Wire('addr_out', max_counter_read_bits, num_pointers_out)
    # ---------------------------------

    # criação das fifos, regs para as filas de entrada e saída
    fifos_in_read_request = m.Wire('fifos_in_read_request', qtde_fifo_in)
    fifos_out_read_request = m.Wire('fifos_out_read_request', qtde_fifo_in)
    we_fifo_in = m.Reg('we_fifo_in', qtde_fifo_in)
    re_fifo_out = m.Reg('re_fifo_out', qtde_fifo_out)
    dout_fifo_out = m.Wire('dout_fifo_out', cache_data_width, qtde_fifo_out)
    almostempty_fifo_out = m.Wire('almostempty_fifo_out', qtde_fifo_out)
    count_fifo_out = m.Wire('count_fifo_out', fifo_depth, qtde_fifo_out)
    # ---------------------------------

    # registrador para permitir o início da leitura das configurações
    start_rd_conf = m.Reg('start_rd_conf')
    # ---------------------------------

    # registrador para permitir o início da leitura de dados
    start_rd_wr_data = m.Reg('start_rd_wr_data')
    # ---------------------------------

    # reset parcial para o circuito
    rst_parc_restart = m.Wire('rst_parc_restart')
    rst_parc_restart.assign(rst_parc | rst)
    # ---------------------------------

    # controle de DONE
    for i in range(qtde_fifo_in):
        done_read[i].assign(counter_received_data_in[i] >= qtde_data_in[i])
    for i in range(qtde_fifo_out):
        done_write_back[i].assign(counter_sent_data_out[i] >= qtde_data_out[i])
    done.assign(EmbeddedCode('&done_write_back'))  # & (&done_uut_interface))'))
    # ---------------------------------

    # Assigns para o acesso direto aos endereços de cada fila de entrada de dados
    # e para as quantidades de dados para leitura de cada fila
    addr_in_all.assign(confs[0])
    qtde_data_in_all.assign(confs[1])
    i = m.Genvar('i')
    m.GenerateFor(i(0), i < qtde_fifo_in, i.inc(), 'gen_1').Assign(
        addr_in[i](addr_in_all[i * max_counter_read_bits:(i * max_counter_read_bits) + max_counter_read_bits])
    )

    i = m.Genvar('j')
    m.GenerateFor(i(0), i < qtde_fifo_in, i.inc(), 'gen_2').Assign(
        qtde_data_in[i](qtde_data_in_all[i * max_counter_read_bits:(i * max_counter_read_bits) + max_counter_read_bits])
    )
    # ---------------------------------

    # Assigns para o acesso direto aos endereços de cada fila de saída de dados
    # e para as quantidades de dados de escrita de cada fila
    addr_out_all.assign(confs[2])
    qtde_data_out_all.assign(confs[3])
    i = m.Genvar('k')
    m.GenerateFor(i(0), i < qtde_fifo_out, i.inc(), 'gen_3').Assign(
        addr_out[i](addr_out_all[i * max_counter_read_bits:(i * max_counter_read_bits) + max_counter_read_bits])
    )

    i = m.Genvar('l')
    m.GenerateFor(i(0), i < qtde_fifo_out, i.inc(), 'gen_4').Assign(
        qtde_data_out[i](
            qtde_data_out_all[i * max_counter_read_bits:(i * max_counter_read_bits) + max_counter_read_bits])
    )
    # ---------------------------------

    # Criação das filas de entrada
    fifo = make_fifo()
    for i in range(qtde_fifo_in):
        re_fifo_in = m.Wire('re_fifo_in%d' % i)
        almostempty_fifo_in = m.Wire('almostempty_fifo_in%d' % i)
        empty_fifo_in = m.Wire('empty_fifo_in%d' % i)
        almostfull_fifo_in = m.Wire('almostfull_fifo_in%d' % i)
        full_fifo_in = m.Wire('full_fifo_in%d' % i)
        count_fifo_in = m.Wire('count_fifo_in%d' % i, fifo_depth)
        dout_fifo_in = m.Wire('dout_fifo_in%d' % i, cache_data_width)
        fifos_in_read_request[i].assign(
            (addr_offset_data_in[i] - counter_received_data_in[i] + count_fifo_in) < FIFO_IN_FULL)

        con = [('clk', clk), ('rst', rst_parc_restart), ('we', we_fifo_in[i]), ('din', resp_rd_data_tmp),
               ('re', re_fifo_in),
               ('dout', dout_fifo_in), ('count', count_fifo_in), ('empty', empty_fifo_in),
               ('almostempty', almostempty_fifo_in),
               ('full', full_fifo_in), ('almostfull', almostfull_fifo_in)]
        params = [('FIFO_WIDTH', cache_data_width), ('FIFO_DEPTH_BITS', fifo_depth)]
        m.Instance(fifo, ('fifo_in%d' % i), params, con)
    # ---------------------------------

    # Criação das filas de saída
    for i in range(qtde_fifo_out):
        we_fifo_out = m.Wire('we_fifo_out%d' % i)
        empty_fifo_out = m.Wire('empty_fifo_out%d' % i)
        almostfull_fifo_out = m.Wire('almostfull_fifo_out%d' % i)
        full_fifo_out = m.Wire('full_fifo_out%d' % i)
        din_fifo_out = m.Wire('din_fifo_out%d' % i, cache_data_width)
        fifos_out_read_request[i].assign(~empty_fifo_out)

        con = [('clk', clk), ('rst', rst_parc_restart), ('we', we_fifo_out), ('din', din_fifo_out),
               ('re', re_fifo_out[i]),
               ('dout', dout_fifo_out[i]), ('count', count_fifo_out[i]), ('empty', empty_fifo_out),
               ('almostempty', almostempty_fifo_out[i]),
               ('full', full_fifo_out), ('almostfull', almostfull_fifo_out)]
        params = [('FIFO_WIDTH', cache_data_width), ('FIFO_DEPTH_BITS', fifo_depth)]
        m.Instance(fifo, ('fifo_out%d' % i), params, con)
    # ---------------------------------

    arbiter = make_arbiter()
    # Arbitro de leitura
    if (qtde_fifo_in > 1):
        grant_in = m.Wire('grant_in', qtde_fifo_in)
        grant_in_valid = m.Wire('grant_in_valid')
        if (numBits(qtde_fifo_in) == 1):
            grant_in_index = m.Wire('grant_in_index')
        else:
            grant_in_index = m.Wire('grant_in_index', numBits(qtde_fifo_in))
        params = [('PORTS', qtde_fifo_in), ('TYPE', "ROUND_ROBIN"), ('BLOCK', "NONE"), ('LSB_PRIORITY', "LOW")]
        ports = [('clk', clk), ('rst', rst), ('request', fifos_in_read_request),
                 ('acknowledge', Int(0, qtde_fifo_in, 10)),
                 ('grant', grant_in),
                 ('grant_valid', grant_in_valid),
                 ('grant_encoded', grant_in_index)]

        m.Instance(arbiter, 'arbiter_in', params, ports)
    # ---------------------------------

    # Arbitro de escrita
    if (qtde_fifo_out > 1):
        grant_out = m.Wire('grant_out', qtde_fifo_out)
        grant_out_valid = m.Wire('grant_out_valid')
        if (numBits(qtde_fifo_out) == 1):
            grant_out_index = m.Wire('grant_out_index')
            grant_out_index_tmp = m.Reg('grant_out_index_tmp')
        else:
            grant_out_index = m.Wire('grant_out_index', numBits(qtde_fifo_out))
            grant_out_index_tmp = m.Reg('grant_out_index_tmp', numBits(qtde_fifo_out))
        params = [('PORTS', qtde_fifo_out), ('TYPE', "ROUND_ROBIN"), ('BLOCK', "NONE"), ('LSB_PRIORITY', "LOW")]
        ports = [('clk', clk), ('rst', rst), ('request', fifos_out_read_request),
                 ('acknowledge', Int(0, qtde_fifo_in, 10)),
                 ('grant', grant_out),
                 ('grant_valid', grant_out_valid),
                 ('grant_encoded', grant_out_index)]

        m.Instance(arbiter, 'arbiter_out', params, ports)
    # ---------------------------------

    # Always responsável por atualizar a palavra do DSM
    m.Always()(
        If(rst)(
            dsm_word(Int(0, dsm_word.width, 10)),
        ).Else(
            dsm_word[0](done),
            dsm_word[1:1 + qtde_fifo_in](done_read),
            dsm_word[1 + qtde_fifo_in:1 + qtde_fifo_in + qtde_fifo_out](done_write_back),
            dsm_word[1 + qtde_fifo_in + qtde_fifo_out:1 + qtde_fifo_in + qtde_fifo_out + len(interfaces)](
                done_uut_interface),
        )
    )

    rst_counter_index = m.Integer('rst_counter_index')

    # Máquina responsável por fazer a leitura das configurações de ponteiros para os
    # registradores respectivos e dos dados para as filas
    if (qtde_fifo_in > 1):
        m.Always(Posedge(clk), Posedge(rst_parc_restart))(
            If(rst)(
                start_rd_conf(Int(0, 1, 2)),
                addr_offset_conf(Int(1, addr_offset_conf.width, 10)),
                req_rd_en(Int(0, 1, 2)),
                req_rd_addr(Int(0, req_rd_addr.width, 2)),
                req_rd_mdata(Int(0, req_rd_mdata.width, 2)),
                fsm_rd(FSM_RD_REQ_READ_CONF),
                For(rst_counter_index(0), rst_counter_index < qtde_fifo_in, rst_counter_index.inc())(
                    addr_offset_data_in[rst_counter_index](Int(0, max_counter_read_bits, 10))
                )
            ).Elif(rst_parc_restart)(
                req_rd_en(Int(0, 1, 2)),
                req_rd_addr(Int(0, req_rd_addr.width, 2)),
                req_rd_mdata(Int(0, req_rd_mdata.width, 2)),
                fsm_rd(FSM_RD_REQ_READ_DATA),

                For(rst_counter_index(0), rst_counter_index < qtde_fifo_in, rst_counter_index.inc())(
                    addr_offset_data_in[rst_counter_index](Int(0, max_counter_read_bits, 10))
                )

            ).Elif(start)(
                req_rd_en(Int(0, 1, 2)),
                Case(fsm_rd)(
                    When(FSM_RD_REQ_READ_CONF)(
                        If(addr_offset_conf <= num_conf)(
                            start_rd_conf(Int(1, 1, 2)),
                            If(req_rd_available)(
                                addr_offset_conf(addr_offset_conf + Int(1, addr_offset_conf.width, 10)),
                                req_rd_addr(workspace_addr_base[6:] + addr_offset_conf),
                                req_rd_mdata(addr_offset_conf),
                                req_rd_en(Int(1, 1, 2)),
                            )
                        ).Elif(start_rd_wr_data)(
                            fsm_rd(FSM_RD_REQ_READ_DATA),
                            start_rd_conf(Int(0, 1, 2)),
                        )
                    ),
                    When(FSM_RD_REQ_READ_DATA)(
                        If(AndList(req_rd_available, grant_in_valid,
                                   (addr_offset_data_in[grant_in_index] < qtde_data_in[grant_in_index])))(
                            addr_offset_data_in[grant_in_index](
                                addr_offset_data_in[grant_in_index] + Int(1, max_counter_read_bits, 10)),
                            req_rd_addr(
                                addr_in[grant_in_index][EmbeddedCode('63:6')] + addr_offset_data_in[grant_in_index]),
                            req_rd_en(Int(1, 1, 2)),
                            req_rd_mdata(grant_in_index)
                        )
                    )
                )
            )
        )
    else:
        m.Always(Posedge(clk), Posedge(rst_parc_restart))(
            If(rst)(
                start_rd_conf(Int(0, 1, 2)),
                addr_offset_conf(Int(1, addr_offset_conf.width, 10)),
                req_rd_en(Int(0, 1, 2)),
                req_rd_addr(Int(0, req_rd_addr.width, 2)),
                req_rd_mdata(Int(0, req_rd_mdata.width, 2)),
                fsm_rd(FSM_RD_REQ_READ_CONF),
                For(rst_counter_index(0), rst_counter_index < qtde_fifo_in, rst_counter_index.inc())(
                    addr_offset_data_in[rst_counter_index](Int(0, max_counter_read_bits, 10))
                )
            ).Elif(rst_parc_restart)(
                req_rd_en(Int(0, 1, 2)),
                req_rd_addr(Int(0, req_rd_addr.width, 2)),
                req_rd_mdata(Int(0, req_rd_mdata.width, 2)),
                fsm_rd(FSM_RD_REQ_READ_DATA),

                For(rst_counter_index(0), rst_counter_index < qtde_fifo_in, rst_counter_index.inc())(
                    addr_offset_data_in[rst_counter_index](Int(0, max_counter_read_bits, 10))
                )

            ).Elif(start)(
                req_rd_en(Int(0, 1, 2)),
                Case(fsm_rd)(
                    When(FSM_RD_REQ_READ_CONF)(
                        If(addr_offset_conf <= num_conf)(
                            start_rd_conf(Int(1, 1, 2)),
                            If(req_rd_available)(
                                addr_offset_conf(addr_offset_conf + Int(1, addr_offset_conf.width, 10)),
                                req_rd_addr(workspace_addr_base[6:] + addr_offset_conf),
                                req_rd_mdata(addr_offset_conf),
                                req_rd_en(Int(1, 1, 2)),
                            )
                        ).Elif(start_rd_wr_data)(
                            fsm_rd(FSM_RD_REQ_READ_DATA),
                            start_rd_conf(Int(0, 1, 2)),
                        )
                    ),
                    When(FSM_RD_REQ_READ_DATA)(
                        If(AndList(req_rd_available, fifos_in_read_request,
                                   (addr_offset_data_in[Int(0, qtde_fifo_in, 2)] < qtde_data_in[
                                       Int(0, qtde_fifo_in, 2)])))(
                            addr_offset_data_in[Int(0, qtde_fifo_in, 2)](
                                addr_offset_data_in[Int(0, qtde_fifo_in, 2)] + Int(1, max_counter_read_bits, 10)),
                            req_rd_addr(
                                addr_in[Int(0, qtde_fifo_in, 2)][EmbeddedCode('63:6')] + addr_offset_data_in[
                                    Int(0, qtde_fifo_in, 2)]),
                            req_rd_en(Int(1, 1, 2)),
                            req_rd_mdata(Int(0, qtde_fifo_in, 2))
                        )
                    )
                )
            )
        )
    # ---------------------------------
    counter_write_dsm = m.Reg('counter_write_dsm')
    counter_written_dsm = m.Reg('counter_written_dsm')

    # Máquina responsável por fazer a leitura das fifos e gravar de volta na memória
    if (qtde_fifo_out > 1):
        m.Always(Posedge(clk), Posedge(rst_parc_restart))(
            If(rst_parc_restart)(
                counter_write_dsm(Int(0, 1, 2)),
                req_wr_en(Int(0, 1, 10)),
                req_wr_addr(Int(0, req_wr_addr.width, 10)),
                req_wr_mdata(Int(0, req_wr_mdata.width, 10)),
                req_wr_data(Int(0, req_wr_data.width, 10)),
                re_fifo_out(Int(0, re_fifo_out.width, 10)),
                grant_out_index_tmp(Int(0, grant_out_index_tmp.width, 10)),
                dsm_word_tmp(dsm_word),
                fsm_wr(FSM_WR_REQ_IDLE),
                For(rst_counter_index(0), rst_counter_index < qtde_fifo_in, rst_counter_index.inc())(
                    addr_offset_data_out[rst_counter_index](Int(0, max_counter_read_bits, 10))
                )
            ).Elif(start_rd_wr_data)(
                req_wr_en(Int(0, 1, 2)),
                re_fifo_out(Int(0, re_fifo_out.width, 10)),
                Case(fsm_wr)(
                    When(FSM_WR_REQ_IDLE)(
                        If(req_wr_available)(
                            If(AndList(dsm_word_tmp != dsm_word, counter_written_dsm == counter_write_dsm))(
                                # Escreve a DSM
                                dsm_word_tmp(dsm_word),
                                req_wr_addr(workspace_addr_base[EmbeddedCode('63:6')]),
                                req_wr_mdata(Int(0xffff, req_wr_mdata.width, 16)),
                                req_wr_data(dsm_word),
                                req_wr_en(Int(1, 1, 2)),
                                counter_write_dsm(~counter_write_dsm)
                            ).Elif(AndList(grant_out_valid, count_fifo_out[grant_out_index] > Int(0, fifo_depth, 2)))(
                                grant_out_index_tmp(grant_out_index),
                                re_fifo_out[grant_out_index](Int(1, 1, 2)),
                                fsm_wr(FSM_WR_REQ_WRITE_DATA)
                            )
                        )
                    ),
                    When(FSM_WR_REQ_WRITE_DATA)(
                        addr_offset_data_out[grant_out_index_tmp](
                            addr_offset_data_out[grant_out_index_tmp] + Int(1, max_counter_read_bits, 10)),
                        req_wr_addr(addr_out[grant_out_index_tmp][EmbeddedCode('63:6')] + addr_offset_data_out[
                            grant_out_index_tmp]),
                        req_wr_mdata(grant_out_index_tmp),
                        req_wr_data(dout_fifo_out[grant_out_index_tmp]),
                        req_wr_en(Int(1, 1, 2)),
                        If(AndList(grant_out_valid, Not(almostempty_fifo_out[grant_out_index]), req_wr_available,
                                   dsm_word_tmp == dsm_word))(
                            grant_out_index_tmp(grant_out_index),
                            re_fifo_out[grant_out_index](Int(1, 1, 2)),
                            fsm_wr(FSM_WR_REQ_WRITE_DATA)
                        ).Else(
                            fsm_wr(FSM_WR_REQ_IDLE),
                        )
                    )
                )
            )
        )
    else:
        m.Always(Posedge(clk), Posedge(rst_parc_restart))(
            If(rst_parc_restart)(
                counter_write_dsm(Int(0, 1, 2)),
                req_wr_en(Int(0, 1, 10)),
                req_wr_addr(Int(0, req_wr_addr.width, 10)),
                req_wr_mdata(Int(0, req_wr_mdata.width, 10)),
                req_wr_data(Int(0, req_wr_data.width, 10)),
                re_fifo_out(Int(0, re_fifo_out.width, 10)),
                dsm_word_tmp(dsm_word),
                fsm_wr(FSM_WR_REQ_IDLE),
                For(rst_counter_index(0), rst_counter_index < qtde_fifo_in, rst_counter_index.inc())(
                    addr_offset_data_out[rst_counter_index](Int(0, max_counter_read_bits, 10))
                )
            ).Elif(start_rd_wr_data)(
                req_wr_en(Int(0, 1, 2)),
                re_fifo_out(Int(0, re_fifo_out.width, 10)),
                Case(fsm_wr)(
                    When(FSM_WR_REQ_IDLE)(
                        If(req_wr_available)(
                            If(AndList(dsm_word_tmp != dsm_word, counter_written_dsm == counter_write_dsm))(
                                # Escreve a DSM
                                dsm_word_tmp(dsm_word),
                                req_wr_addr(workspace_addr_base[EmbeddedCode('63:6')]),
                                req_wr_mdata(Int(0xffff, req_wr_mdata.width, 16)),
                                req_wr_data(dsm_word),
                                req_wr_en(Int(1, 1, 2)),
                                counter_write_dsm(~counter_write_dsm)
                            ).Elif(AndList(fifos_out_read_request,
                                           count_fifo_out[Int(0, qtde_fifo_out, 2)] > Int(0, fifo_depth, 2)))(
                                re_fifo_out[Int(0, qtde_fifo_out, 2)](Int(1, 1, 2)),
                                fsm_wr(FSM_WR_REQ_WRITE_DATA)
                            )
                        )
                    ),
                    When(FSM_WR_REQ_WRITE_DATA)(
                        addr_offset_data_out[Int(0, qtde_fifo_out, 2)](
                            addr_offset_data_out[Int(0, qtde_fifo_out, 2)] + Int(1, max_counter_read_bits, 10)),
                        req_wr_addr(addr_out[Int(0, qtde_fifo_out, 2)][EmbeddedCode('63:6')] + addr_offset_data_out[
                            Int(0, qtde_fifo_out, 2)]),
                        req_wr_mdata(Int(0, mdata_width, 2)),
                        req_wr_data(dout_fifo_out[Int(0, qtde_fifo_out, 2)]),
                        req_wr_en(Int(1, 1, 2)),
                        If(AndList(fifos_out_read_request, Not(almostempty_fifo_out[Int(0, qtde_fifo_out, 2)]),
                                   req_wr_available,
                                   dsm_word_tmp == dsm_word))(
                            re_fifo_out[Int(0, qtde_fifo_out, 2)](Int(1, 1, 2)),
                            fsm_wr(FSM_WR_REQ_WRITE_DATA)
                        ).Else(
                            fsm_wr(FSM_WR_REQ_IDLE),
                        )
                    )
                )
            )
        )

    # ---------------------------------

    # Machine for receiving cache data and:
    # 1 - save in configuration regs and
    # 2 - queuing in the queue.
    m.Always(Posedge(clk), Posedge(rst))(
        If(rst)(
            start_rd_wr_data(Int(0, 1, 2)),
            counter_received_conf(Int(0, 1, 2)),
            we_fifo_in(Int(0, we_fifo_in.width, 10)),
            resp_rd_data_tmp(Int(0, resp_rd_data_tmp.width, 10)),
            For(rst_counter_index(0), rst_counter_index < qtde_fifo_in, rst_counter_index.inc())(
                counter_received_data_in[rst_counter_index](Int(0, max_counter_read_bits, 10))
            ),
            For(rst_counter_index(0), rst_counter_index < num_conf, rst_counter_index.inc())(
                confs[rst_counter_index](Int(0, cache_data_width, 10))
            )

        ).Elif(AndList(start_rd_conf, Not(start_rd_wr_data)))(
            If(counter_received_conf < num_conf)(
                If(resp_rd_valid)(
                    counter_received_conf(counter_received_conf + Int(1, counter_received_conf.width, 10)),
                    confs[counter_received_conf](resp_rd_data)
                )
            ).Else(
                start_rd_wr_data(Int(1, 1, 2))
            )
        ).Elif(start_rd_wr_data)(
            we_fifo_in(Int(0, we_fifo_in.width, 10)),
            resp_rd_data_tmp(resp_rd_data),
            If(resp_rd_valid)(
                counter_received_data_in[resp_rd_mdata](
                    counter_received_data_in[resp_rd_mdata] + Int(1, max_counter_read_bits, 10)),
                we_fifo_in[resp_rd_mdata](Int(1, 1, 10))
            )
        )
    )
    # ---------------------------------

    # Machine responsible for receiving written data responses.
    m.Always(Posedge(clk), Posedge(rst_parc_restart))(
        If(rst_parc_restart)(
            counter_written_dsm(Int(0, 1, 2)),
            For(rst_counter_index(0), rst_counter_index < qtde_fifo_in, rst_counter_index.inc())(
                counter_sent_data_out[rst_counter_index](Int(0, max_counter_read_bits, 10))
            ),
        ).Elif(start_rd_wr_data)(
            If(resp_wr_valid)(
                If(resp_wr_mdata == Int(0xffff, resp_wr_mdata.width, 16))(
                    counter_written_dsm(~counter_written_dsm)
                ).Else(
                    counter_sent_data_out[resp_wr_mdata](
                        counter_sent_data_out[resp_wr_mdata] + Int(1, max_counter_read_bits, 10)),
                )

            )
        )
    )
    # ---------------------------------

    # criação das interfaces de conexão com os circuitos consumidores de dados
    # início do processo de criação das interfaces
    module_number = 0
    idx_fifo_in = 0
    idx_fifo_out = 0
    map_interfaces = {}
    params = []
    for i in interfaces:
        j = 0
        name = 'uut_interface%d_%d_%d' % (i[0], i[1], j)
        while (name in map_interfaces.keys()):
            j = j + 1
            name = 'uut_interface%d_%d_%d' % (i[0], i[1], j)
        interface = make_interface(cache_data_width, name, i[0], i[1])
        map_interfaces[name] = interface
        # if (name in map_interfaces.keys()):
        #    interface = map_interfaces[name]
        # else:
        #    interface = make_interface(cache_data_width, name, i[0], i[1])
        #    map_interfaces[name] = interface
        con = [('clk', clk), ('rst', rst), ('rst_parc', rst_parc), ('start', start_rd_wr_data)]
        # Entradas de dados das filas para a interface
        for j in range(i[0]):
            con.append(('available_read%d' % j, EmbeddedCode('~empty_fifo_in%d' % idx_fifo_in)))
            con.append(('almost_empty_read%d' % j, EmbeddedCode('almostempty_fifo_in%d' % idx_fifo_in)))
            con.append(('req_rd_data%d' % j, EmbeddedCode('re_fifo_in%d' % idx_fifo_in)))
            con.append(('rd_data%d' % j, EmbeddedCode('dout_fifo_in%d' % idx_fifo_in)))
            idx_fifo_in = idx_fifo_in + 1
        # Saídas de dados das filas para a interface
        for j in range(i[1]):
            con.append(('available_write%d' % j, EmbeddedCode('~almostfull_fifo_out%d' % idx_fifo_out)))
            con.append(('req_wr_data%d' % j, EmbeddedCode('we_fifo_out%d' % idx_fifo_out)))
            con.append(('wr_data%d' % j, EmbeddedCode('din_fifo_out%d' % idx_fifo_out)))
            idx_fifo_out = idx_fifo_out + 1
        con.append(('done', done_uut_interface[module_number]))
        # m.Instance(interface, (name + '_' + str(module_number)), params, con)
        m.Instance(interface, name, params, con)
        module_number = module_number + 1
    # ---------------------------------

    return m
