#ifndef CGRASCHEDULER_DATAFLOWDEFS_H
#define CGRASCHEDULER_DATAFLOWDEFS_H

typedef enum {
    OP_BASIC,
    OP_IMMEDIATE,
    OP_IN,
    OP_OUT
} op_type_t;

typedef enum {
    PE_OP_PASS_A = 0,
    PE_OP_PASS_B = 1,
    PE_OP_MIN = 2,
    PE_OP_MAX = 3,
    PE_OP_BEQ = 0,
    PE_OP_BNE = 1,
    PE_OP_SLT = 2,
    PE_OP_SGT = 3,
    PE_OP_ADD = 4,
    PE_OP_SUB = 5,
    PE_OP_MULT = 6,
    PE_OP_XOR = 7,
    PE_OP_AND = 8,
    PE_OP_OR = 9,
    PE_OP_NOT = 10,
    PE_OP_SHL = 11,
    PE_OP_SHR = 12,
    PE_OP_MUX = 13,
    PE_OP_ABS = 14
} pe_opcode_t;


typedef enum {
    OP_PASS_A = 0,
    OP_PASS_B = 1,
    OP_MIN = 2,
    OP_MAX = 3,
    OP_BEQ = 4,
    OP_BNE = 5,
    OP_SLT = 6,
    OP_SGT = 7,
    OP_ADD = 8,
    OP_SUB = 9,
    OP_MULT = 10,
    OP_XOR = 11,
    OP_AND = 12,
    OP_OR = 13,
    OP_NOT = 14,
    OP_SHL = 15,
    OP_SHR = 16,
    OP_MUX = 17,
    OP_ABS = 18
} op_opcode_t;

const unsigned long pe_op_code[19]{
        PE_OP_PASS_A,
        PE_OP_PASS_B,
        PE_OP_MIN,
        PE_OP_MAX,
        PE_OP_BEQ,
        PE_OP_BNE,
        PE_OP_SLT,
        PE_OP_SGT,
        PE_OP_ADD,
        PE_OP_SUB,
        PE_OP_MULT,
        PE_OP_XOR,
        PE_OP_AND,
        PE_OP_OR,
        PE_OP_NOT,
        PE_OP_SHL,
        PE_OP_SHR,
        PE_OP_MUX,
        PE_OP_ABS
};

const char op_label[19][8]{
        "REG",
        "REG",
        "MIN",
        "MAX",
        "BEQ",
        "BNE",
        "SLT",
        "SGT",
        "ADD",
        "SUB",
        "MUL",
        "XOR",
        "AND",
        "OR",
        "NOT",
        "SHL",
        "SHR",
        "MUX",
        "ABS"
};

#endif //CGRASCHEDULER_DATAFLOWDEFS_H
