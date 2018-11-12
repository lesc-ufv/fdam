#ifndef CGRA_DEFS_H
#define CGRA_DEFS_H

typedef enum CGRA_CONF_OP {
    CGRA_CONF_SET_INSTRUCTION,
    CGRA_CONF_SET_CONST,
    CGRA_CONF_SET_PC_MAX,
    CGRA_CONF_SET_LOOP,
    CGRA_CONF_SET_IGNORE,
} CGRA_CONF_OP;

typedef struct cl_t {
    unsigned long dword[8];
} cl_t;

typedef struct cgra_intial_conf_t {
    union {
        cl_t cache_line;
        struct {
            unsigned int qtd_pe_conf;
            unsigned int qtd_net_conf;
            unsigned int read_fifo_mask;
            unsigned int write_fifo_mask;
        };
    };
} cgra_intial_conf_t;

typedef struct pe_instruction_t {
    unsigned short inst_addr;
    union {
        unsigned short instruction;
        struct {
            unsigned short alu_op:4;
            unsigned short alu_in_a:2;
            unsigned short alu_in_b:2;
            unsigned short alu_out:2;
            unsigned short rf_raddr:3;
            unsigned short rf_waddr:3;
        };
    };
} pe_instruction_t;

typedef struct pe_initial_conf_t {
    union {
        unsigned long dword;
        struct {
            unsigned char op_conf;
            unsigned char const_addr;
            unsigned short id_pe;
            union {
                int constant;
                pe_instruction_t pe_instruction;
                int pc_max;
                int pc_loop;
                int store_ignore;
            };
        };
    };
} pe_initial_conf_t;

typedef struct swicth_conf_t {
    union {
        unsigned char swicth_conf;
        struct {
            unsigned char out0:2;
            unsigned char out1:2;
            unsigned char out2:2;
            unsigned char out3:2;
        };
    };
} swicth_conf_t;

typedef struct net_conf_t {
    swicth_conf_t *swicth_conf;
} net_conf_t;

typedef struct queue_t {
    short *data;
    long length;
} queue_t;

typedef struct cgra_program_t {

    unsigned short cgra_id;
    cgra_intial_conf_t cgra_intial_conf;
    pe_initial_conf_t *pe_initial_conf;
    net_conf_t *net_conf;
    queue_t *input_queues;
    queue_t *output_queues;
} cgra_program_t;


#endif //CGRA_DEFS_H
