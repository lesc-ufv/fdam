from veriloggen import *


def make_acc_user(acc_id, data_width, num_input_queues, num_output_queues, accelerator_module=None):
    m = Module('fdam_acc_user_%d' % acc_id)

    clk = m.Input('clk')
    rst = m.Input('rst')
    start = m.Input('start')
    acc_user_done_rd_data = m.Input('acc_user_done_rd_data', num_input_queues)
    acc_user_done_wr_data = m.Input('acc_user_done_wr_data', num_output_queues)
    acc_user_available_read = m.Input('acc_user_available_read', num_input_queues)
    acc_user_request_read = m.OutputReg('acc_user_request_read', num_input_queues)
    acc_user_read_data_valid = m.Input('acc_user_read_data_valid', num_input_queues)
    acc_user_read_data = m.Input('acc_user_read_data', data_width * num_input_queues)
    acc_user_available_write = m.Input('acc_user_available_write', num_output_queues)
    acc_user_request_write = m.OutputReg('acc_user_request_write', num_output_queues)
    acc_user_write_data = m.OutputReg('acc_user_write_data', data_width * num_output_queues)
    acc_user_done = m.OutputReg('acc_user_done')

    if accelerator_module:
        m.Instance(accelerator_module, accelerator_module.name, [], m.get_ports())
    else:
        m.EmbeddedCode('\n//Sample code for loopback:')
        m.Always(Posedge(clk))(
            If(rst)(
                acc_user_request_read(0),
                acc_user_request_write(0),
                acc_user_write_data(0),
                acc_user_done(0)
            ).Else(
                If(start)(
                    acc_user_done(acc_user_done_rd_data & acc_user_done_wr_data),
                    acc_user_request_read(acc_user_available_write & acc_user_available_read),
                    acc_user_write_data(acc_user_read_data),
                    acc_user_request_write(acc_user_read_data_valid)
                )
            )
        )

    return m
