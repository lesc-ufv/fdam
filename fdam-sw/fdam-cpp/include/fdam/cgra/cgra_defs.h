#ifndef CGRA_DEFS_H
#define CGRA_DEFS_H

#define switch_conf_radix2(out0, out1) (unsigned char)(((out1) << 1) | (out0 << 0))
#define switch_conf_radix4(out0, out1, out2, out3) (unsigned char)(((out3) << 6) | ((out2) << 4) | ((out1) << 2) | (out0 << 0))
#define switch_conf_radix8(out0, out1, out2, out3, out4, out5, out6, out7) (unsigned int)( ((out7) << 21)  | \
                                                                                           ((out6) << 18)  | \
                                                                                           ((out5) << 15)  | \
                                                                                           ((out4) << 12)  | \
                                                                                           ((out3) << 9)   | \
                                                                                           ((out2) << 6)   | \
                                                                                           ((out1) << 3)   | \
                                                                                           ((out0) << 0)

typedef enum cgra_conf_type_t {
    CGRA_CONF_SET_PE_INSTRUCTION = 1,
    CGRA_CONF_SET_PE_CONST,
    CGRA_CONF_SET_PE_PC_MAX,
    CGRA_CONF_SET_PE_PC_LOOP,
    CGRA_CONF_SET_PE_STORE_IGNORE,
    CGRA_CONF_SET_NET_PC_MAX,
    CGRA_CONF_SET_NET_PC_LOOP,
    CGRA_CONF_NET_SWITCH
} cgra_conf_type_t;

typedef struct cl_t {
    unsigned long dword[8];
} cl_t;

typedef struct cgra_intial_conf_t {
    union {
        cl_t cache_line;
        struct {
            unsigned int qtd_conf;
            unsigned int read_fifo_mask;
            unsigned int write_fifo_mask;
        };
    };
} cgra_intial_conf_t;

typedef struct pe_instruction_conf_t {
    unsigned long conf_type:8;
    unsigned long pe_id:16;
    unsigned long inst_addr:16;
    unsigned long alu_op:4;
    unsigned long alu_in_a:2;
    unsigned long alu_in_b:2;
    unsigned long alu_out:2;
    unsigned long rf_raddr:3;
    unsigned long rf_waddr:3;
} pe_instruction_conf_t;

typedef struct pe_constant_conf_t {
    unsigned long conf_type:8;
    unsigned long pe_id:16;
    unsigned long const_addr:8;
    unsigned long constant:32;
} pe_constant_conf_t;

typedef struct pe_pc_max_conf_t {
    unsigned long conf_type:8;
    unsigned long pe_id:16;
    unsigned long pc_max:32;
} pe_pc_max_conf_t;

typedef struct pe_pc_loop_conf_t {
    unsigned long conf_type:8;
    unsigned long pe_id:16;
    unsigned long pc_loop:32;
} pe_pc_loop_conf_t;

typedef struct pe_store_ignore_conf_t {
    unsigned long conf_type:8;
    unsigned long pe_id:16;
    unsigned long store_ignore:32;
} pe_store_ignore_conf_t;

typedef struct net_pc_max_conf_t {
    unsigned char conf_type:8;
    unsigned short switch_number:16;
    unsigned int pc_max:32;
} net_pc_max_conf_t;

typedef struct net_pc_loop_conf_t {
    unsigned char conf_type:8;
    unsigned short switch_number:16;
    unsigned int pc_loop:32;
} net_pc_loop_conf_t;

typedef struct net_switch_conf_t {
    unsigned long conf_type:8;
    unsigned long switch_number:16;
    unsigned long inst_addr:16;
    unsigned long switch_conf:24;
} net_switch_conf_t;


typedef struct initial_conf_t {
    union {
        unsigned long pad;
        pe_instruction_conf_t pe_instruction_conf;
        pe_constant_conf_t pe_constant_conf;
        pe_pc_max_conf_t pe_pc_max_conf;
        pe_pc_loop_conf_t pe_pc_loop_conf;
        pe_store_ignore_conf_t pe_store_ignore_conf;
        net_switch_conf_t net_switch_conf;
        net_pc_max_conf_t net_pc_max_conf;
        net_pc_loop_conf_t net_pc_loop_conf;
    };
} initial_conf_t;


typedef struct queue_t {
    short *data;
    long length;
} queue_t;

typedef struct cgra_program_t {

    unsigned short cgra_id;
    cgra_intial_conf_t cgra_intial_conf;
    initial_conf_t *initial_conf;
    queue_t *input_queues;
    queue_t *output_queues;
} cgra_program_t;


#endif //CGRA_DEFS_H
