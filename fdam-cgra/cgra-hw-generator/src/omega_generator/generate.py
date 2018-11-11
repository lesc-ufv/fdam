# for i in range(9):
#    size = 8 << i
#    code = make_omega(size, 0, 4).to_verilog()
#    split_modules(code, 'code_gen/redes_radix_4/omega%dx%d' % (size, size))
from src.common.cgra_utils import bit_rotate_left
from src.omega_generator.make_omega import make_omega

make_omega(8, 0, 4, True).to_verilog()

for i in range(8):
    print(i, '->', '{:d}'.format(bit_rotate_left(i, 2, 3)))
