from math import ceil

from veriloggen import *

from make_arbiter import make_arbiter
from make_fifo import make_fifo
from make_interface import make_interface
from util import numBits


def make_buffer_controller(cache_data_width, fifo_depth, interfaces):
    MAX_INTERFACES = 64
    # verificação da possibilidade de construção do circuito
    qtde_fifo_in = 0
    qtde_fifo_out = 0
    if len(interfaces) < 1:
        print('No mínimo uma interface deve ser criada!')
        print('Processo abortado.')
        exit()
    for i in interfaces:
        if i[0] == 0 or i[1] == 0:
            print('As quantidades de entradas ou de saídas das interfaces não podem ser 0!')
            print('Processo abortado.')
            exit()
        qtde_fifo_in = qtde_fifo_in + i[0]
        qtde_fifo_out = qtde_fifo_out + i[1]
    if (qtde_fifo_in + qtde_fifo_out) > MAX_INTERFACES:
        print('As quantidades de entradas ou de saídas não podem ultrapassar %d!' % MAX_INTERFACES)
        print('Processo abortado.')
        exit()
    else:
        try:
            addr_width = 64
            mdata_width = 16
            max_counter_read_bits = 64
            num_cl_conf_in = int(2 * ceil(qtde_fifo_in / 8))
            num_cl_conf_out = int(2 * ceil(qtde_fifo_out / 8))
            num_cl_conf_total = num_cl_conf_in + num_cl_conf_out
            num_cl_dsm = 1 + (num_cl_conf_total // 2)
            num_pointers_in = 8 * (num_cl_conf_in // 2)
            num_pointers_out = 8 * (num_cl_conf_out // 2)
            m = Module('buffer_controller')
            clk = m.Input('clk')
            rst = m.Input('rst')
            start = m.Input('start')
            rst_interfaces = m.Input('rst_interfaces', MAX_INTERFACES)
            start_interfaces = m.Input('start_interfaces', MAX_INTERFACES)
            rst_buffer_in_index = m.Input('rst_buffer_in_index', numBits(MAX_INTERFACES) + 1)
            rst_buffer_out_index = m.Input('rst_buffer_out_index', numBits(MAX_INTERFACES) + 1)
            # Endereço da linha de cache com as configurações
            workspace_addr_base = m.Input('workspace_addr_base', addr_width)
            conf_size = m.Input('conf_size', 16)
            dsm_size = m.Input('dsm_size', 16)
            update_workspace = m.Input('update_workspace')

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
            m.EmbeddedCode("//Parâmetros locais:")
            FIFO_IN_FULL = m.Localparam('FIFO_IN_FULL', pow(2, fifo_depth))
            TAG_WR_DSM = m.Localparam('TAG_WR_DSM', Int(0xfffe, mdata_width, 16))
            TAG_RD_CONF = m.Localparam('TAG_RD_CONF', Int(0xffff, mdata_width, 16))

            # Registrador para pipeline do dado a ser enfileirado nas filas de entrada
            m.EmbeddedCode("\n//Registrador para pipeline do dado a ser enfileirado nas filas de entrada")
            resp_rd_data_tmp = m.Reg('resp_rd_data_tmp', cache_data_width)
            reset_buffers_in = m.Reg('reset_buffers_in', qtde_fifo_in)
            reset_buffers_out = m.Reg('reset_buffers_out', qtde_fifo_out)
            # Gerenciamento dos Done signals
            # Gerenciamento dos dones de leitura e escrita para o circuito
            # Apalavra é constituída por:
            # bit 0 - done total
            # proximos Nbits - término de leitura das fifos_in
            # próximos Mbits- término do processamento das interfaces
            m.EmbeddedCode("\n//Gerenciamento dos Done signals")
            done_read_buffers = m.Wire('done_read_buffers', qtde_fifo_in)
            done_write_buffers = m.Wire('done_write_buffers', qtde_fifo_out)
            done_uut_interface = m.Wire('done_uut_interface', len(interfaces))
            done_reg_vet = m.Reg('done_reg_vet', cache_data_width)
            done_reg_vet_last = m.Reg('done_reg_vet_last', cache_data_width)

            # controle da fsm de leitura de dados e configurações
            m.EmbeddedCode("\n//controle da fsm de leitura de dados e configurações")
            fsm_rd = m.Reg('fsm_rd', 2)
            m.EmbeddedCode("\n//controle da fsm de escrita de dados")
            # controle da fsm de escrita de dados
            fsm_wr = m.Reg('fsm_wr', 2)

            # registradores onde ficarão armazenados os endereços,
            # as quantidades de dados para cada fila de entrada e para cada fila de saída
            # control for conf
            m.EmbeddedCode("\n//registradores onde ficarão armazenados os endereços")
            m.EmbeddedCode("//as quantidades de dados para cada fila de entrada e para cada fila de saída")
            m.EmbeddedCode("//control for conf")

            configurations = m.Reg('configurations', cache_data_width, num_cl_conf_total)
            dsm = m.Reg('dsm', cache_data_width, num_cl_dsm)
            counter_received_conf = m.Reg('counter_received_conf', numBits(num_cl_conf_total) + 1)
            addr_offset_conf = m.Reg('addr_offset_conf', numBits(num_cl_conf_total) + 1)

            # control for data_in
            m.EmbeddedCode("\n//control for data_in")
            counter_received_data_in = m.Reg('counter_received_data_in', max_counter_read_bits, num_pointers_in)
            addr_offset_data_in = m.Reg('addr_offset_data_in', max_counter_read_bits, num_pointers_in)
            qtde_data_in = m.Wire('qtde_data_in', max_counter_read_bits, num_pointers_in)
            addr_in = m.Wire('addr_in', max_counter_read_bits, num_pointers_in)

            # control for data_out
            m.EmbeddedCode("\n// control for data_out")
            counter_sent_data_out = m.Reg('counter_sent_data_out', max_counter_read_bits, num_pointers_out)
            addr_offset_data_out = m.Reg('addr_offset_data_out', max_counter_read_bits, num_pointers_out)
            qtde_data_out = m.Wire('qtde_data_out', max_counter_read_bits, num_pointers_out)
            addr_out = m.Wire('addr_out', max_counter_read_bits, num_pointers_out)

            # criação das fifos, regs para as filas de entrada e saída
            m.EmbeddedCode("\n//criação das fifos, regs para as filas de entrada e saída")
            fifos_in_read_request = m.Wire('fifos_in_read_request', qtde_fifo_in)
            fifos_out_read_request = m.Wire('fifos_out_read_request', qtde_fifo_in)
            we_fifo_in = m.Reg('we_fifo_in', qtde_fifo_in)
            re_fifo_out = m.Reg('re_fifo_out', qtde_fifo_out)
            dout_fifo_out = m.Wire('dout_fifo_out', cache_data_width, qtde_fifo_out)
            almostempty_fifo_out = m.Wire('almostempty_fifo_out', qtde_fifo_out)
            count_fifo_out = m.Wire('count_fifo_out', fifo_depth, qtde_fifo_out)
            i = m.Genvar('i')
            j = m.Genvar('j')
            # controle de DONE
            m.EmbeddedCode("\n//controle de DONE")
            m.GenerateFor(i(0), i < qtde_fifo_in, i.inc(), 'gen_0').Assign(
                done_read_buffers[i](counter_received_data_in[i] >= qtde_data_in[i])
            )
            m.GenerateFor(i(qtde_fifo_in), i < qtde_fifo_in + qtde_fifo_out, i.inc(), 'gen_1').Assign(
                done_write_buffers[i](counter_sent_data_out[i] >= qtde_data_out[i])
            )
            code = '(&done_buffers_read && &done_buffers_write && &done_uut_interface)'
            done_reg_vet[0].assign(EmbeddedCode(code))
            done_reg_vet[1:qtde_fifo_in + 1].assign(done_read_buffers)
            done_reg_vet[qtde_fifo_in + 1:qtde_fifo_in + qtde_fifo_out + 1].assign(done_write_buffers)
            # Assigns para o acesso direto aos endereços de cada fila de entrada de dados
            # e para as quantidades de dados para leitura de cada fila
            m.EmbeddedCode("\n//Assigns para o acesso direto aos endereços de cada fila de entrada de dados")
            m.EmbeddedCode("//e para as quantidades de dados para leitura de cada fila")

            genfor0 = m.GenerateFor(i(0), i < (num_cl_conf_in), i(i + 2), 'gen_2')
            genfor1 = genfor0.GenerateFor(j(0), j < 8, j.inc(), 'gen_3')

            code = "assign addr_in[((i)*8) + j] = configurations[i][j * %d:(j * %d) + %d]" % (
                max_counter_read_bits, max_counter_read_bits, max_counter_read_bits)
            genfor1.EmbeddedCode(code)
            code = "assign qtde_data_in[(i*8) + j] = configurations[i+1][j * %d:(j * %d) + %d]" % (
                max_counter_read_bits, max_counter_read_bits, max_counter_read_bits)
            genfor1.EmbeddedCode(code)

            # Assigns para o acesso direto aos endereços de cada fila de saída de dados
            # e para as quantidades de dados de escrita de cada fila
            m.EmbeddedCode("//Assigns para o acesso direto aos endereços de cada fila de saída de dados")
            m.EmbeddedCode("//e para as quantidades de dados de escrita de cada fila")
            genfor0 = m.GenerateFor(i(num_cl_conf_in), i < num_cl_conf_total, i(i + 2), 'gen_4')
            genfor1 = genfor0.GenerateFor(j(0), j < 8, j.inc(), 'gen_5')
            code = "assign addr_out[(i*8) + j] = configurations[i][j * %d:(j * %d) + %d]" % (
                max_counter_read_bits, max_counter_read_bits, max_counter_read_bits)
            genfor1.EmbeddedCode(code)
            code = "assign qtde_data_out[(i*8) + j] = configurations[i+1][j * %d:(j * %d) + %d]" % (
                max_counter_read_bits, max_counter_read_bits, max_counter_read_bits)
            genfor1.EmbeddedCode(code)
            # Criação das filas de entrada
            m.EmbeddedCode("//Criação das filas de entrada")
            fifo = make_fifo()
            re_fifo_in = m.Wire('re_fifo_in', qtde_fifo_in)
            almostempty_fifo_in = m.Wire('almostempty_fifo_in', qtde_fifo_in)
            empty_fifo_in = m.Wire('empty_fifo_in', qtde_fifo_in)
            almostfull_fifo_in = m.Wire('almostfull_fifo_in', qtde_fifo_in)
            full_fifo_in = m.Wire('full_fifo_in', qtde_fifo_in)
            count_fifo_in = m.Wire('count_fifo_in', fifo_depth, qtde_fifo_in)
            dout_fifo_in = m.Wire('dout_fifo_in', cache_data_width, qtde_fifo_in)
            con = [('clk', clk), ('rst', rst | reset_buffers_in[i]), ('we', we_fifo_in[i]),
                   ('din', resp_rd_data_tmp),
                   ('re', re_fifo_in[i]),
                   ('dout', dout_fifo_in[i]), ('count', count_fifo_in[i]), ('empty', empty_fifo_in[i]),
                   ('almostempty', almostempty_fifo_in[i]),
                   ('full', full_fifo_in[i]), ('almostfull', almostfull_fifo_in[i])]
            params = [('FIFO_WIDTH', cache_data_width), ('FIFO_DEPTH_BITS', fifo_depth)]

            m.GenerateFor(i(0), i < qtde_fifo_in, i.inc(), 'gen_6').Assign(
                fifos_in_read_request[i](
                    (addr_offset_data_in[i] - counter_received_data_in[i] + count_fifo_in[i]) < FIFO_IN_FULL)
            )
            genfor3 = m.GenerateFor(i(0), i < qtde_fifo_in, i.inc(), 'gen_7')
            genfor3.Instance(fifo, ('fifo_in'), params, con)

            # Criação das filas de saída
            m.EmbeddedCode("//Criação das filas de saída")
            we_fifo_out = m.Wire('we_fifo_out', qtde_fifo_out)
            empty_fifo_out = m.Wire('empty_fifo_out', qtde_fifo_out)
            almostfull_fifo_out = m.Wire('almostfull_fifo_out', qtde_fifo_out)
            full_fifo_out = m.Wire('full_fifo_out', qtde_fifo_out)
            din_fifo_out = m.Wire('din_fifo_out', cache_data_width, qtde_fifo_out)
            con = [('clk', clk), ('rst', rst | reset_buffers_out[i]), ('we', we_fifo_out[i]), ('din', din_fifo_out[i]),
                   ('re', re_fifo_out[i]),
                   ('dout', dout_fifo_out[i]), ('count', count_fifo_out[i]), ('empty', empty_fifo_out[i]),
                   ('almostempty', almostempty_fifo_out[i]),
                   ('full', full_fifo_out[i]), ('almostfull', almostfull_fifo_out[i])]
            params = [('FIFO_WIDTH', cache_data_width), ('FIFO_DEPTH_BITS', fifo_depth)]
            m.GenerateFor(i(0), i < qtde_fifo_in, i.inc(), 'gen_8').Assign(
                fifos_out_read_request[i](~empty_fifo_out[i])
            )
            genfor4 = m.GenerateFor(i(0), i < qtde_fifo_in, i.inc(), 'gen_9')
            genfor4.Instance(fifo, ('fifo_out'), params, con)

            arbiter = make_arbiter()
            grant_in_valid = Wire()
            grant_in_index = Wire()
            grant_out_index = Wire()
            grant_out_index_tmp = Wire()
            # Arbitro de leitura
            if qtde_fifo_in > 1:
                m.EmbeddedCode("//Arbitro de leitura")
                grant_in = m.Wire('grant_in', qtde_fifo_in)
                grant_in_valid = m.Wire('grant_in_valid')
                if numBits(qtde_fifo_in) == 1:
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

            # Arbitro de escrita
            if qtde_fifo_out > 1:
                m.EmbeddedCode("//Arbitro de escrita")
                grant_out = m.Wire('grant_out', qtde_fifo_out)
                grant_out_valid = m.Wire('grant_out_valid')
                if numBits(qtde_fifo_out) == 1:
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

            # Máquina responsável por fazer a leitura das configurações de ponteiros para os
            # registradores respectivos e dos dados para as filas
            m.EmbeddedCode("//Máquina responsável por fazer a leitura das configurações de ponteiros para os")
            m.EmbeddedCode("//registradores respectivos e dos dados para as filas")
            m.EmbeddedCode("//Parâmetros locais:")
            FSM_RD_REQ_READ_CONF = m.Localparam('FSM_RD_REQ_READ_CONF', 0)
            FSM_RD_REQ_READ_DATA = m.Localparam('FSM_RD_REQ_READ_DATA', 1)
            rst_counter_index = m.Integer('rst_counter_index')
            if qtde_fifo_in > 1:
                m.Always(Posedge(clk), Posedge(rst), Posedge(update_workspace))(
                    If(rst | update_workspace)(
                        addr_offset_conf(Int(0, addr_offset_conf.width, 10)),
                        req_rd_en(Int(0, 1, 2)),
                        req_rd_addr(Int(0, req_rd_addr.width, 2)),
                        req_rd_mdata(Int(0, req_rd_mdata.width, 2)),
                        fsm_rd(FSM_RD_REQ_READ_CONF),
                        For(rst_counter_index(0), rst_counter_index < qtde_fifo_in, rst_counter_index.inc())(
                            addr_offset_data_in[rst_counter_index](Int(0, max_counter_read_bits, 10))
                        )
                    ).Elif(start)(
                        req_rd_en(Int(0, 1, 2)),
                        Case(fsm_rd)(
                            When(FSM_RD_REQ_READ_CONF)(
                                If(addr_offset_conf <= conf_size)(
                                    If(req_rd_available)(
                                        addr_offset_conf(addr_offset_conf + Int(1, addr_offset_conf.width, 10)),
                                        req_rd_addr(workspace_addr_base[6:] + addr_offset_conf),
                                        req_rd_mdata(addr_offset_conf),
                                        req_rd_en(Int(1, 1, 2)),
                                    )
                                ).Elif(counter_received_conf >= conf_size)(
                                    fsm_rd(FSM_RD_REQ_READ_DATA),
                                ),
                                If(rst_buffer_in_index != Int(0, rst_buffer_in_index.width, 10))(
                                    addr_offset_data_in[rst_buffer_in_index](
                                        Int(0, addr_offset_data_in.width, 10))
                                )
                            ),
                            When(FSM_RD_REQ_READ_DATA)(
                                If(AndList(req_rd_available, grant_in_valid,
                                           (addr_offset_data_in[grant_in_index] < qtde_data_in[grant_in_index])))(
                                    If(rst_buffer_in_index != Int(0, rst_buffer_in_index.width, 10))(
                                        addr_offset_data_in[rst_buffer_in_index](
                                            Int(0, addr_offset_data_in.width, 10))
                                    ).Else(
                                        addr_offset_data_in[grant_in_index](
                                            addr_offset_data_in[grant_in_index] + Int(1, max_counter_read_bits, 10)),
                                    ),
                                    req_rd_addr(
                                        addr_in[grant_in_index][EmbeddedCode('63:6')] + addr_offset_data_in[
                                            grant_in_index]),
                                    req_rd_en(Int(1, 1, 2)),
                                    req_rd_mdata(grant_in_index)
                                )
                            )
                        )
                    )
                )
            else:
                m.Always(Posedge(clk), Posedge(rst), Posedge(update_workspace))(
                    If(rst | update_workspace)(
                        addr_offset_conf(Int(0, addr_offset_conf.width, 10)),
                        req_rd_en(Int(0, 1, 2)),
                        req_rd_addr(Int(0, req_rd_addr.width, 2)),
                        req_rd_mdata(Int(0, req_rd_mdata.width, 2)),
                        fsm_rd(FSM_RD_REQ_READ_CONF),
                        For(rst_counter_index(0), rst_counter_index < qtde_fifo_in, rst_counter_index.inc())(
                            addr_offset_data_in[rst_counter_index](Int(0, max_counter_read_bits, 10))
                        )
                    ).Elif(start)(
                        req_rd_en(Int(0, 1, 2)),
                        Case(fsm_rd)(
                            When(FSM_RD_REQ_READ_CONF)(
                                If(addr_offset_conf <= conf_size)(
                                    If(req_rd_available)(
                                        addr_offset_conf(addr_offset_conf + Int(1, addr_offset_conf.width, 10)),
                                        req_rd_addr(workspace_addr_base[6:] + addr_offset_conf),
                                        req_rd_mdata(TAG_RD_CONF),
                                        req_rd_en(Int(1, 1, 2)),
                                    )
                                ).Elif(counter_received_conf >= conf_size)(
                                    fsm_rd(FSM_RD_REQ_READ_DATA),
                                ),
                                If(rst_buffer_in_index != Int(0, rst_buffer_in_index.width, 10))(
                                    addr_offset_data_in[rst_buffer_in_index - 1](
                                        Int(0, addr_offset_data_in.width, 10))
                                )
                            ),
                            When(FSM_RD_REQ_READ_DATA)(
                                If(AndList(req_rd_available, fifos_in_read_request,
                                           (addr_offset_data_in[Int(0, qtde_fifo_in, 2)] < qtde_data_in[
                                               Int(0, qtde_fifo_in, 2)])))(
                                    If(rst_buffer_in_index != Int(0, rst_buffer_in_index.width, 10))(
                                        addr_offset_data_in[rst_buffer_in_index - 1](
                                            Int(0, addr_offset_data_in.width, 10))
                                    ).Else(
                                        addr_offset_data_in[Int(0, qtde_fifo_in, 2)](
                                            addr_offset_data_in[Int(0, qtde_fifo_in, 2)] + Int(1, max_counter_read_bits,
                                                                                               10))),
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

            # Máquina responsável por fazer a leitura das fifos e gravar de volta na memória
            m.EmbeddedCode("//Máquina responsável por fazer a leitura das fifos e gravar de volta na memória")
            m.EmbeddedCode("//Parâmetros locais:")
            FSM_WR_REQ_IDLE = m.Localparam('FSM_WR_REQ_IDLE', 0)
            FSM_WR_REQ_WRITE_DSM = m.Localparam('FSM_WR_REQ_WRITE_DSM', 1)
            FSM_WR_REQ_WRITE_DATA = m.Localparam('FSM_WR_REQ_WRITE_DATA', 2)
            m.EmbeddedCode('')
            counter_write_dsm = m.Reg('counter_write_dsm', dsm_size.width)
            counter_written_dsm = m.Reg('counter_written_dsm', dsm_size.width)
            dsm_wr_ready = m.Reg('dsm_wr_ready')
            m.EmbeddedCode('')
            dsm_written = m.Wire('dsm_written')
            update_dsm = m.Wire('update_dsm')
            fifo_out_Almost_ready = m.Wire('fifo_out_almost_ready')
            fifo_out_ready = m.Wire('fifo_out_ready')
            dsm_written.assign(counter_written_dsm >= dsm_size)
            code = '|((done_reg_vet ^ done_reg_vet_last) & done_reg_vet)'
            update_dsm.assign(EmbeddedCode(code))
            if qtde_fifo_out > 1:
                fifo_out_Almost_ready.assign(
                    AndList(fifos_out_read_request, Not(almostempty_fifo_out[grant_in_index])))
                fifo_out_ready.assign(AndList(fifos_out_read_request,
                                              (count_fifo_out[grant_in_index] > Int(0, count_fifo_out.width, 10))))
            else:
                fifo_out_Almost_ready.assign(
                    AndList(fifos_out_read_request, Not(almostempty_fifo_out[Int(0, qtde_fifo_out, 2)])))
                fifo_out_ready.assign(AndList(fifos_out_read_request,
                                              (count_fifo_out[Int(0, qtde_fifo_out, 2)] > Int(0, count_fifo_out.width,
                                                                                              10))))

            if qtde_fifo_out > 1:
                m.Always(Posedge(clk), Posedge(rst))(
                    If(rst)(
                        counter_write_dsm(Int(0, counter_write_dsm.width, 10)),
                        req_wr_en(Int(0, 1, 10)),
                        req_wr_addr(Int(0, req_wr_addr.width, 10)),
                        req_wr_mdata(Int(0, req_wr_mdata.width, 10)),
                        req_wr_data(Int(0, req_wr_data.width, 10)),
                        re_fifo_out(Int(0, re_fifo_out.width, 10)),
                        grant_out_index_tmp(Int(0, grant_out_index_tmp.width, 10)),
                        fsm_wr(FSM_WR_REQ_IDLE),
                        For(rst_counter_index(0), rst_counter_index < qtde_fifo_in, rst_counter_index.inc())(
                            addr_offset_data_out[rst_counter_index](Int(0, max_counter_read_bits, 10))
                        )
                    ).Else(
                        req_wr_en(Int(0, 1, 2)),
                        re_fifo_out(Int(0, re_fifo_out.width, 10)),
                        Case(fsm_wr)(
                            When(FSM_WR_REQ_IDLE)(
                                If(req_wr_available)(
                                    If(dsm_wr_ready)(
                                        counter_write_dsm(Int(0, counter_write_dsm.width, 10)),
                                        fsm_wr(FSM_WR_REQ_WRITE_DSM)
                                    ).Elif(fifo_out_ready)(
                                        grant_out_index_tmp(grant_out_index),
                                        re_fifo_out[grant_out_index](Int(1, 1, 2)),
                                        fsm_wr(FSM_WR_REQ_WRITE_DATA)
                                    ),
                                    If(rst_buffer_out_index != Int(0, rst_buffer_out_index.width, 10))(
                                        addr_offset_data_out[rst_buffer_out_index - 1](
                                            Int(0, addr_offset_data_out.width, 10))
                                    )
                                )
                            ),
                            When(FSM_WR_REQ_WRITE_DSM)(
                                If(req_wr_available)(
                                    req_wr_addr(workspace_addr_base[EmbeddedCode('63:6')]),
                                    req_wr_mdata(TAG_WR_DSM),
                                    req_wr_data(dsm[counter_write_dsm]),
                                    req_wr_en(Int(1, 1, 2)),
                                    counter_write_dsm(counter_write_dsm + Int(1, counter_write_dsm.width, 10)),
                                ),
                                If(counter_write_dsm >= dsm_size)(
                                    fsm_wr(FSM_WR_REQ_IDLE)
                                )
                            ),
                            When(FSM_WR_REQ_WRITE_DATA)(
                                If(rst_buffer_out_index != Int(0, rst_buffer_out_index.width, 10))(
                                    addr_offset_data_out[rst_buffer_out_index - 1](
                                        Int(0, addr_offset_data_out.width, 10))
                                ).Else(
                                    addr_offset_data_out[grant_out_index_tmp](
                                        addr_offset_data_out[grant_out_index_tmp] + Int(1, max_counter_read_bits,
                                                                                        10))
                                ),
                                req_wr_addr(
                                    addr_out[grant_out_index_tmp][EmbeddedCode('63:6')] + addr_offset_data_out[
                                        grant_out_index_tmp]),
                                req_wr_mdata(grant_out_index_tmp),
                                req_wr_data(dout_fifo_out[grant_out_index_tmp]),
                                req_wr_en(Int(1, 1, 2)),
                                If(AndList(fifo_out_Almost_ready, req_wr_available, Not(dsm_wr_ready)))(
                                    grant_out_index_tmp(grant_out_index),
                                    re_fifo_out[Int(0, qtde_fifo_out, 2)](Int(1, 1, 2)),
                                    fsm_wr(FSM_WR_REQ_WRITE_DATA)
                                ).Else(
                                    fsm_wr(FSM_WR_REQ_IDLE),
                                )
                            )
                        )
                    )
                )
            else:
                m.Always(Posedge(clk), Posedge(rst))(
                    If(rst)(
                        counter_write_dsm(Int(0, counter_write_dsm.width, 10)),
                        req_wr_en(Int(0, 1, 10)),
                        req_wr_addr(Int(0, req_wr_addr.width, 10)),
                        req_wr_mdata(Int(0, req_wr_mdata.width, 10)),
                        req_wr_data(Int(0, req_wr_data.width, 10)),
                        re_fifo_out(Int(0, re_fifo_out.width, 10)),
                        fsm_wr(FSM_WR_REQ_IDLE),
                        For(rst_counter_index(0), rst_counter_index < qtde_fifo_in, rst_counter_index.inc())(
                            addr_offset_data_out[rst_counter_index](Int(0, max_counter_read_bits, 10))
                        )
                    ).Else(
                        req_wr_en(Int(0, 1, 2)),
                        re_fifo_out(Int(0, re_fifo_out.width, 10)),
                        Case(fsm_wr)(
                            When(FSM_WR_REQ_IDLE)(
                                If(req_wr_available)(
                                    If(dsm_wr_ready)(
                                        counter_write_dsm(Int(0, counter_write_dsm.width, 10)),
                                        fsm_wr(FSM_WR_REQ_WRITE_DSM)
                                    ).Elif(fifo_out_ready)(
                                        re_fifo_out[Int(0, qtde_fifo_out, 2)](Int(1, 1, 2)),
                                        fsm_wr(FSM_WR_REQ_WRITE_DATA)
                                    ),
                                    If(rst_buffer_out_index != Int(0, rst_buffer_out_index.width, 10))(
                                        addr_offset_data_out[rst_buffer_out_index - 1](
                                            Int(0, addr_offset_data_out.width, 10))
                                    )
                                )
                            ),
                            When(FSM_WR_REQ_WRITE_DSM)(
                                If(req_wr_available)(
                                    req_wr_addr(workspace_addr_base[EmbeddedCode('63:6')]),
                                    req_wr_mdata(TAG_WR_DSM),
                                    req_wr_data(dsm[counter_write_dsm]),
                                    req_wr_en(Int(1, 1, 2)),
                                    counter_write_dsm(counter_write_dsm + Int(1, counter_write_dsm.width, 10)),
                                ),
                                If(counter_write_dsm >= dsm_size)(
                                    fsm_wr(FSM_WR_REQ_IDLE)
                                )
                            ),
                            When(FSM_WR_REQ_WRITE_DATA)(
                                If(rst_buffer_out_index != Int(0, rst_buffer_out_index.width, 10))(
                                    addr_offset_data_out[rst_buffer_out_index - 1](
                                        Int(0, addr_offset_data_out.width, 10))
                                ).Else(
                                    addr_offset_data_out[Int(0, 1, 2)](
                                        addr_offset_data_out[Int(0, 1, 2)] + Int(1, max_counter_read_bits, 10))
                                ),
                                req_wr_addr(
                                    addr_out[Int(0, qtde_fifo_out, 2)][EmbeddedCode('63:6')] + addr_offset_data_out[
                                        Int(0, qtde_fifo_out, 2)]),
                                req_wr_mdata(Int(0, mdata_width, 2)),
                                req_wr_data(dout_fifo_out[Int(0, qtde_fifo_out, 2)]),
                                req_wr_en(Int(1, 1, 2)),
                                If(AndList(fifo_out_Almost_ready, req_wr_available, Not(dsm_wr_ready)))(
                                    re_fifo_out[Int(0, qtde_fifo_out, 2)](Int(1, 1, 2)),
                                    fsm_wr(FSM_WR_REQ_WRITE_DATA)
                                ).Else(
                                    fsm_wr(FSM_WR_REQ_IDLE),
                                )
                            )
                        )
                    )
                )
            # Machine for receiving cache data and:
            # 1 - save in configuration regs and
            # 2 - queuing in the queue.
            m.EmbeddedCode("//Machine for receiving cache data and:")
            m.EmbeddedCode("//1 - save in configuration regs and")
            m.EmbeddedCode("//2 - queuing in the queue.")
            m.Always(Posedge(clk), Posedge(rst), Posedge(update_workspace))(
                If(rst | update_workspace)(
                    counter_received_conf(Int(0, 1, 2)),
                    we_fifo_in(Int(0, we_fifo_in.width, 10)),
                    resp_rd_data_tmp(Int(0, resp_rd_data_tmp.width, 10)),
                    For(rst_counter_index(0), rst_counter_index < qtde_fifo_in, rst_counter_index.inc())(
                        counter_received_data_in[rst_counter_index](Int(0, max_counter_read_bits, 10))
                    ),
                    For(rst_counter_index(0), rst_counter_index < num_cl_conf_total, rst_counter_index.inc())(
                        configurations[rst_counter_index](Int(0, cache_data_width, 10))
                    )

                ).Else(
                    we_fifo_in(Int(0, we_fifo_in.width, 10)),
                    If(resp_rd_valid)(
                        If(resp_rd_mdata == TAG_RD_CONF)(
                            If(counter_received_conf < conf_size)(
                                configurations[counter_received_conf](resp_rd_data),
                                counter_received_conf(counter_received_conf + Int(1, counter_received_conf.width, 10))
                            )
                        ).Else(
                            resp_rd_data_tmp(resp_rd_data),
                            counter_received_data_in[resp_rd_mdata](
                                counter_received_data_in[resp_rd_mdata] + Int(1, max_counter_read_bits, 10)),
                            we_fifo_in[resp_rd_mdata](Int(1, 1, 10))
                        )
                    )
                )
            )
            # Machine responsible for receiving written data responses.
            m.EmbeddedCode("//Machine responsible for receiving written data responses.")
            m.Always(Posedge(clk), Posedge(rst))(
                If(rst)(
                    For(rst_counter_index(0), rst_counter_index < qtde_fifo_in, rst_counter_index.inc())(
                        counter_sent_data_out[rst_counter_index](Int(0, max_counter_read_bits, 10))
                    ),
                ).Elif(resp_wr_valid)(
                    If(resp_wr_mdata != TAG_WR_DSM)(
                        counter_sent_data_out[resp_wr_mdata](
                            counter_sent_data_out[resp_wr_mdata] + Int(1, max_counter_read_bits, 10)),
                    )

                )
            )
            # Recebimento de reset dos buffers de entrada e saida
            m.EmbeddedCode('//Recebimento de reset dos buffers de entrada e saida')
            m.Always(Posedge(clk), Posedge(rst))(
                If(rst)(
                    reset_buffers_in(Int(0, reset_buffers_in.width, 10)),
                    reset_buffers_out(Int(0, reset_buffers_out.width, 10))
                ).Elif(start)(
                    If(rst_buffer_in_index != Int(0, rst_buffer_in_index.width, 10))(
                        reset_buffers_in[rst_buffer_in_index](Int(1, 1, 2))
                    ),
                    If(rst_buffer_out_index != Int(0, rst_buffer_out_index.width, 10))(
                        reset_buffers_out[rst_buffer_out_index](Int(1, 1, 2))
                    )
                )
            )

            CHECK_DSM = m.Localparam('CHECK_DSM', 0)
            WAIT_DSM_WRITE = m.Localparam('WAIT_DSM_WRITE', 1)

            index = m.Integer('index')
            fsm_dsm = m.Reg('fsm_dsm', 1)
            dsm_is_written = m.Wire('dsm_is_written')
            dsm_is_written.assign(counter_written_dsm >= counter_write_dsm)

            m.Always(Posedge(clk), Posedge(rst))(
                If(rst)(
                    dsm_wr_ready(Int(0, 1, 2)),
                    counter_written_dsm(Int(0, counter_written_dsm.width, 10)),
                    done_reg_vet_last(Int(0, done_reg_vet_last.width, 10)),
                    For(rst_counter_index(0), rst_counter_index < num_cl_dsm, rst_counter_index.inc())(
                        dsm[rst_counter_index](Int(0, cache_data_width, 10))
                    ),
                    fsm_dsm(CHECK_DSM)
                ).Elif(start)(
                    Case(fsm_dsm)(
                        When(CHECK_DSM)(
                            If(AndList(update_dsm, dsm_is_written))(
                                done_reg_vet_last(done_reg_vet),
                                dsm[0](done_reg_vet),
                                For(index(0), index < num_cl_conf_in // 2, index.inc())(
                                    dsm[index](Cat(counter_received_data_in[(index * 8) + 7],
                                                   counter_received_data_in[(index * 8) + 6],
                                                   counter_received_data_in[(index * 8) + 5],
                                                   counter_received_data_in[(index * 8) + 4],
                                                   counter_received_data_in[(index * 8) + 3],
                                                   counter_received_data_in[(index * 8) + 2],
                                                   counter_received_data_in[(index * 8) + 1],
                                                   counter_received_data_in[(index * 8) + 0])
                                               ),
                                    dsm[index + num_cl_conf_in // 2](Cat(counter_sent_data_out[(index * 8) + 7],
                                                                         counter_sent_data_out[(index * 8) + 6],
                                                                         counter_sent_data_out[(index * 8) + 5],
                                                                         counter_sent_data_out[(index * 8) + 4],
                                                                         counter_sent_data_out[(index * 8) + 3],
                                                                         counter_sent_data_out[(index * 8) + 2],
                                                                         counter_sent_data_out[(index * 8) + 1],
                                                                         counter_sent_data_out[(index * 8) + 0])
                                                                     ),

                                ),
                                fsm_dsm(WAIT_DSM_WRITE)
                            )
                        ),
                        When(WAIT_DSM_WRITE)(
                            If(fsm_wr == FSM_WR_REQ_WRITE_DSM)(
                                dsm_wr_ready(Int(0, 1, 2))
                            ),
                            If(resp_wr_valid)(
                                If(resp_wr_mdata == TAG_WR_DSM)(
                                    counter_written_dsm(counter_written_dsm + Int(1, counter_written_dsm.width, 10))
                                )
                            ),
                            If(dsm_is_written)(
                                fsm_dsm(CHECK_DSM)
                            )
                        )
                    )
                )

            )
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
                while name in map_interfaces.keys():
                    j = j + 1
                    name = 'uut_interface%d_%d_%d' % (i[0], i[1], j)
                interface = make_interface(cache_data_width, name, i[0], i[1])
                map_interfaces[name] = interface
                con = [('clk', clk), ('rst', rst | rst_interfaces[module_number]),
                       ('start', start_interfaces[module_number])]
                # Entradas de dados das filas para a interface
                for j in range(i[0]):
                    con.append(('available_read%d' % j, ~empty_fifo_in[idx_fifo_in]))
                    con.append(('almost_empty_read%d' % j, almostempty_fifo_in[idx_fifo_in]))
                    con.append(('req_rd_data%d' % j, re_fifo_in[idx_fifo_in]))
                    con.append(('rd_data%d' % j, dout_fifo_in[idx_fifo_in]))
                    idx_fifo_in = idx_fifo_in + 1
                # Saídas de dados das filas para a interface
                for j in range(i[1]):
                    con.append(('available_write%d' % j, ~almostfull_fifo_out[idx_fifo_out]))
                    con.append(('req_wr_data%d' % j, we_fifo_out[idx_fifo_out]))
                    con.append(('wr_data%d' % j, din_fifo_out[idx_fifo_out]))
                    idx_fifo_out = idx_fifo_out + 1

                con.append(('done', done_uut_interface[module_number]))
                # m.Instance(interface, (name + '_' + str(module_number)), params, con)
                m.Instance(interface, name, params, con)
                module_number = module_number + 1

            # # criação das interfaces de conexão com os circuitos consumidores de dados
            # # início do processo de criação das interfaces
            # module_number = 0
            # idx_fifo_in = 0
            # idx_fifo_out = 0
            # map_interfaces = {}
            # params = []
            # for i in interfaces:
            #     j = 0
            #     name = 'uut_interface%d_%d_%d' % (i[0], i[1], j)
            #     while name in map_interfaces.keys():
            #         j = j + 1
            #         name = 'uut_interface%d_%d_%d' % (i[0], i[1], j)
            #     interface = make_interface(cache_data_width, name, i[0], i[1])
            #     map_interfaces[name] = interface
            #     con = [('clk', clk), ('rst', rst | rst_interfaces[module_number]),
            #            ('start', start_interfaces[module_number])]
            #     # Entradas de dados das filas para a interface
            #     for j in range(i[0]):
            #         con.append(('available_read%d' % j, EmbeddedCode('~empty_fifo_in%d' % idx_fifo_in)))
            #         con.append(('almost_empty_read%d' % j, EmbeddedCode('almostempty_fifo_in%d' % idx_fifo_in)))
            #         con.append(('req_rd_data%d' % j, EmbeddedCode('re_fifo_in%d' % idx_fifo_in)))
            #         con.append(('rd_data%d' % j, EmbeddedCode('dout_fifo_in%d' % idx_fifo_in)))
            #         idx_fifo_in = idx_fifo_in + 1
            #     # Saídas de dados das filas para a interface
            #     for j in range(i[1]):
            #         con.append(('available_write%d' % j, EmbeddedCode('~almostfull_fifo_out%d' % idx_fifo_out)))
            #         con.append(('req_wr_data%d' % j, EmbeddedCode('we_fifo_out%d' % idx_fifo_out)))
            #         con.append(('wr_data%d' % j, EmbeddedCode('din_fifo_out%d' % idx_fifo_out)))
            #         idx_fifo_out = idx_fifo_out + 1
            #     con.append(('done', done_uut_interface[module_number]))
            #     # m.Instance(interface, (name + '_' + str(module_number)), params, con)
            #     m.Instance(interface, name, params, con)
            #     module_number = module_number + 1




            # ---------------------------------
            print('Interface gerada!')
            print('Quantidade de buffer de entrada: %d' % qtde_fifo_in)
            print('Quantidade de buffer de saida: %d' % qtde_fifo_out)
            return m
        except:
            print("Erro:", sys.exc_info()[0])
            raise
