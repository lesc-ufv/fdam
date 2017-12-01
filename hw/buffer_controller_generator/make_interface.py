from veriloggen import *


# O arquivo deste módulo deve ser substituído pelo arquivo que ligará o circuito do usuário

def make_interface(data_width, module_name, qtde_in, qtde_out):
    m = Module(module_name)

    m.Input('clk')
    m.Input('rst')
    m.Input('rst_parc')
    m.Input('start')
    for i in range(qtde_out):
        m.Input('available_write%d' % i)
    for i in range(qtde_in):
        m.Input('available_read%d' % i)
    for i in range(qtde_in):
        m.Input('almost_empty_read%d' % i)
    for i in range(qtde_in):
        m.Output('req_rd_data%d' % i)
    for i in range(qtde_in):
        m.Input('rd_data%d' % i, data_width)
    for i in range(qtde_out):
        m.Output('req_wr_data%d' % i)
    for i in range(qtde_out):
        m.Output('wr_data%d' % i, data_width)
    m.Output('done')

    return m
