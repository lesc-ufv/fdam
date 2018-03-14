from veriloggen import *

def numBits(n):
    count = 1
    while n / 2 > 1:
        n = n / 2
        count = count + 1
    return count

def make_const(value,width):
    #bits_value = numBits(value)
    #const = Cat(Repeat(Int(0,1,2),width-bits_value),Int(value,bits_value,10))
    return value