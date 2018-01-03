from veriloggen import *


def make_priority_encoder():
    m = Module('priority_encoder')
    WIDTH = m.Parameter('WIDTH', 4)
    LSB_PRIORITY = m.Parameter('LSB_PRIORITY', "LOW")
    input_unencoded = m.Input('input_unencoded', WIDTH)
    output_valid = m.Output('output_valid', 1)
    output_encoded = m.Output('output_encoded', EmbeddedCode('$clog2(WIDTH)'))
    output_unencoded = m.Output('output_unencoded', WIDTH)

    W1 = m.Localparam('W1', EmbeddedCode('2**$clog2(WIDTH)'))
    W2 = m.Localparam('W2', EmbeddedCode('W1/2'))

    gen_if = m.GenerateIf(WIDTH == 2, 'if_width')
    gen_if.Assign(output_valid(input_unencoded[0] | input_unencoded[1]))
    gen_if2 = gen_if.GenerateIf(LSB_PRIORITY == 'LOW', 'if_low')
    gen_if2.Assign(output_encoded(input_unencoded[1]))
    gen_if2 = gen_if2.Else('else_low')
    gen_if2.EmbeddedCode('assign output_encoded = ~input_unencoded[0];')
    gen_if = gen_if.Else('else_width')
    gen_if.Wire('out1', EmbeddedCode('$clog2(W2)'))
    gen_if.Wire('out2', EmbeddedCode('$clog2(W2)'))
    gen_if.Wire('valid1')
    gen_if.Wire('valid2')
    gen_if.Wire('out_un', WIDTH)
    gen_if.EmbeddedCode('priority_encoder #(\n\
            .WIDTH(W2),\n\
            .LSB_PRIORITY(LSB_PRIORITY)\n\
        )\n\
        priority_encoder_inst1 (\n\
            .input_unencoded(input_unencoded[W2-1:0]),\n\
            .output_valid(valid1),\n\
            .output_encoded(out1),\n\
            .output_unencoded(out_un[W2-1:0])\n\
        );\n\
        priority_encoder #(\n\
            .WIDTH(W2),\n\
            .LSB_PRIORITY(LSB_PRIORITY)\n\
        )\n\
        priority_encoder_inst2 (\n\
            .input_unencoded({{W1-WIDTH{1\'b0}}, input_unencoded[WIDTH-1:W2]}),\n\
            .output_valid(valid2),\n\
            .output_encoded(out2),\n\
            .output_unencoded(out_un[WIDTH-1:W2])\n\
        );')
    gen_if.EmbeddedCode('       assign output_valid = valid1 | valid2;\n\
        if (LSB_PRIORITY == "LOW") begin\n\
            assign output_encoded = valid2 ? {1\'b1, out2} : {1\'b0, out1};\n\
        end else begin\n\
            assign output_encoded = valid1 ? {1\'b0, out1} : {1\'b1, out2};\n\
        end')
    m.EmbeddedCode('assign output_unencoded = 1 << output_encoded;')
    return m
