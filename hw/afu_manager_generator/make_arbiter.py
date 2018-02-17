from veriloggen import *

from make_priority_encoder import make_priority_encoder


def make_arbiter():
    m = Module('arbiter')
    PORTS = m.Parameter('PORTS', 4)
    TYPE = m.Parameter('TYPE', "ROUND_ROBIN")
    BLOCK = m.Parameter('BLOCK', "NONE")
    LSB_PRIORITY = m.Parameter('LSB_PRIORITY', "LOW")
    WIDTH = m.Parameter('WIDTH', EmbeddedCode('PORTS>1?$clog2(PORTS):1'))
    clk = m.Input('clk')
    rst = m.Input('rst')
    request = m.Input('request', PORTS)
    acknowledge = m.Input('acknowledge', PORTS)
    grant = m.Output('grant', PORTS)
    grant_valid = m.Output('grant_valid')
    grant_encoded = m.Output('grant_encoded',WIDTH)

    grant_reg = m.Reg('grant_reg', PORTS)
    grant_valid_reg = m.Reg('grant_valid_reg')
    grant_encoded_reg = m.Reg('grant_encoded_reg',WIDTH)

    genIf = m.GenerateIf(PORTS>1,'gen_if_true')

    grant_next = genIf.Reg('grant_next', PORTS)
    grant_valid_next = genIf.Reg('grant_valid_next')
    grant_encoded_next = genIf.Reg('grant_encoded_next', EmbeddedCode('$clog2(PORTS)'))

    m.Assign(grant_valid(grant_valid_reg))
    m.Assign(grant(grant_reg))
    m.Assign(grant_encoded(grant_encoded_reg))

    request_valid = genIf.Wire('request_valid')
    request_index = genIf.Wire('request_index', EmbeddedCode('$clog2(PORTS)'))
    request_mask = genIf.Wire('request_mask', PORTS)

    pe = make_priority_encoder()
    params = [('WIDTH', PORTS), ('LSB_PRIORITY', LSB_PRIORITY)]
    con = [
        ('input_unencoded', request),
        ('output_valid', request_valid),
        ('output_encoded', request_index),
        ('output_unencoded', request_mask)
    ]
    genIf.Instance(pe, 'priority_encoder_inst', params, con)
    mask_reg = genIf.Reg('mask_reg', PORTS)
    mask_next = genIf.Reg('mask_next', PORTS)
    masked_request_valid = genIf.Wire('masked_request_valid')
    masked_request_index = genIf.Wire('masked_request_index', EmbeddedCode('$clog2(PORTS)'))
    masked_request_mask = genIf.Wire('masked_request_mask', PORTS)
    params = [('WIDTH', PORTS), ('LSB_PRIORITY', LSB_PRIORITY)]
    con = [
        ('input_unencoded', request & mask_reg),
        ('output_valid', masked_request_valid),
        ('output_encoded', masked_request_index),
        ('output_unencoded', masked_request_mask)
    ]
    genIf.Instance(pe, 'priority_encoder_masked', params, con)

    genIf.Always()(
        grant_next(0, blk=True),
        grant_valid_next(0, blk=True),
        grant_encoded_next(0, blk=True),
        mask_next(mask_reg, blk=True),
        If(AndList(BLOCK == "REQUEST", grant_reg & request))(
            grant_valid_next(grant_valid_reg, blk=True),
            grant_next(grant_reg, blk=True),
            grant_encoded_next(grant_encoded_reg, blk=True)
        ).Elif(AndList(BLOCK == "ACKNOWLEDGE", grant_valid, Not(grant_reg & acknowledge)))(
            grant_valid_next(grant_valid_reg, blk=True),
            grant_next(grant_reg, blk=True),
            grant_encoded_next(grant_encoded_reg, blk=True)
        ).Elif(request_valid)(
            If(TYPE == "PRIORITY")(
                grant_valid_next(1, blk=True),
                grant_next(request_mask, blk=True),
                grant_encoded_next(request_index, blk=True)
            ).Elif(TYPE == "ROUND_ROBIN")(
                If(masked_request_valid)(
                    grant_valid_next(1, blk=True),
                    grant_next(masked_request_mask, blk=True),
                    grant_encoded_next(masked_request_index, blk=True),
                    If(LSB_PRIORITY == "LOW")(
                        EmbeddedCode('mask_next = {PORTS{1\'b1}} >> (PORTS - masked_request_index);')
                    ).Else(
                        EmbeddedCode('mask_next = {PORTS{1\'b1}} << (masked_request_index + 1);')
                    )
                ).Else(
                    grant_valid_next(1, blk=True),
                    grant_next(request_mask, blk=True),
                    grant_encoded_next(request_index, blk=True),
                    If(LSB_PRIORITY == "LOW")(
                        EmbeddedCode('mask_next = {PORTS{1\'b1}} >> (PORTS - request_index);')
                    ).Else(
                        EmbeddedCode('mask_next = {PORTS{1\'b1}} << (request_index + 1);')
                    )
                )
            )

        )
    )
    genIf.Always(Posedge(clk))(
        If(rst)(
            grant_reg(0),
            grant_valid_reg(0),
            grant_encoded_reg(0),
            mask_reg(0)
        ).Else(
            grant_reg(grant_next),
            grant_valid_reg(grant_valid_next),
            grant_encoded_reg(grant_encoded_next),
            mask_reg(mask_next)
        )
    )
    genIf = genIf.Else('gen_if_false')
    genIf.Always(Posedge(clk))(
        If(rst)(
            grant_reg(0),
            grant_valid_reg(0),
            grant_encoded_reg(0),
        ).Else(
            grant_reg(request),
            grant_valid_reg(request),
            grant_encoded_reg(0)
        )
    )

    return m
