def bit_rotate_left(val, r_bits, max_bits):
    return (val << r_bits % max_bits) & (2 ** max_bits - 1) | (
            (val & (2 ** max_bits - 1)) >> (max_bits - (r_bits % max_bits)))


def bit_rotate_rigth(val, r_bits, max_bits):
    return ((val & (2 ** max_bits - 1)) >> r_bits % max_bits) | (
            val << (max_bits - (r_bits % max_bits)) & (2 ** max_bits - 1))
