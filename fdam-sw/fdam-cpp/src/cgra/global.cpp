//
// Created by lucas on 23/01/19.
//

#include <fdam/cgra/global.h>

int Global::idx = 0;


int Global::getNextIdx() {
    return idx++;
}

void Global::resetIndex() {
    Global::idx = 0;
}

int Global::bitRotateLeft(int val, int r_bits, int max_bits) {
    return ((val << r_bits % max_bits) & ((1 << max_bits) - 1)) | (
            (val & ((1 << max_bits) - 1)) >> (max_bits - (r_bits % max_bits)));
}

int Global::bitRotateRigth(int val, int r_bits, int max_bits) {
    return ((val & ((1 << max_bits) - 1)) >> r_bits % max_bits) | (
            val << (max_bits - (r_bits % max_bits)) & ((1 << max_bits) - 1));
}

int Global::intLog(double x, double base) {
    return (int) std::ceil(log(x) / log(base));
}