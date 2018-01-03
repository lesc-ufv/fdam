from veriloggen import *

from make_priority_encoder import make_priority_encoder


def make_arbiter():
    m = Module('arbiter')
    PORTS = m.Parameter('PORTS', 4)
    TYPE = m.Parameter('TYPE', "ROUND_ROBIN")
    BLOCK = m.Parameter('BLOCK', "NONE")
    LSB_PRIORITY = m.Parameter('LSB_PRIORITY', "LOW")
    clk = m.Input('clk')
    rst = m.Input('rst')
    request = m.Input('request', PORTS)
    acknowledge = m.Input('acknowledge', PORTS)
    grant = m.Output('grant', PORTS)
    grant_valid = m.Output('grant_valid')
    grant_encoded = m.Output('grant_encoded', EmbeddedCode('$clog2(PORTS)'))

    grant_reg = m.Reg('grant_reg', PORTS)
    grant_next = m.Reg('grant_next', PORTS)
    grant_valid_reg = m.Reg('grant_valid_reg')
    grant_valid_next = m.Reg('grant_valid_next')
    grant_encoded_reg = m.Reg('grant_encoded_reg', EmbeddedCode('$clog2(PORTS)'))
    grant_encoded_next = m.Reg('grant_encoded_next', EmbeddedCode('$clog2(PORTS)'))

    m.Assign(grant_valid(grant_valid_reg))
    m.Assign(grant(grant_reg))
    m.Assign(grant_encoded(grant_encoded_reg))

    request_valid = m.Wire('request_valid')
    request_index = m.Wire('request_index', EmbeddedCode('$clog2(PORTS)'))
    request_mask = m.Wire('request_mask', PORTS)

    pe = make_priority_encoder()
    params = [('WIDTH', PORTS), ('LSB_PRIORITY', LSB_PRIORITY)]
    con = [
        ('input_unencoded', request),
        ('output_valid', request_valid),
        ('output_encoded', request_index),
        ('output_unencoded', request_mask)
    ]
    m.Instance(pe, 'priority_encoder_inst', params, con)
    mask_reg = m.Reg('mask_reg', PORTS)
    mask_next = m.Reg('mask_next', PORTS)
    masked_request_valid = m.Wire('masked_request_valid')
    masked_request_index = m.Wire('masked_request_index', EmbeddedCode('$clog2(PORTS)'))
    masked_request_mask = m.Wire('masked_request_mask', PORTS)
    params = [('WIDTH', PORTS), ('LSB_PRIORITY', LSB_PRIORITY)]
    con = [
        ('input_unencoded', request & mask_reg),
        ('output_valid', masked_request_valid),
        ('output_encoded', masked_request_index),
        ('output_unencoded', masked_request_mask)
    ]
    m.Instance(pe, 'priority_encoder_masked', params, con)

    m.Always()(
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
    m.Always(Posedge(clk))(
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

    return m
