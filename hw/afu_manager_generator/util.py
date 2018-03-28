from veriloggen import *

def numBits(n):
    count = 1
    while n / 2 > 1:
        n = n / 2
        count = count + 1
    return count
