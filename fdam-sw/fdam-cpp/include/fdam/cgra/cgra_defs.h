#ifndef CGRA_DEFS_H
#define CGRA_DEFS_H

#include <vector>
#include <map>

typedef enum cgra_conf_type_t {
    CGRA_CONF_SET_PE_INSTRUCTION = 1,
    CGRA_CONF_SET_PE_CONST,
    CGRA_CONF_SET_PE_PC_MAX,
    CGRA_CONF_SET_PE_PC_LOOP,
    CGRA_CONF_SET_PE_IGNORE,
    CGRA_CONF_SET_PE_QTD_LOW,
    CGRA_CONF_SET_PE_QTD_HIGH,
    CGRA_CONF_SET_NET_PC_MAX,
    CGRA_CONF_SET_NET_PC_LOOP,
    CGRA_CONF_NET_SWITCH,
    CGRA_CONF_SET_NET_BRANCH_PC_MAX,
    CGRA_CONF_SET_NET_BRANCH_PC_LOOP,
    CGRA_CONF_NET_BRANCH_SWITCH
} cgra_conf_type_t;

typedef struct cl_t {
    unsigned long dword[8];
} cl_t;

typedef struct cgra_intial_conf_t {
    union {
        cl_t cache_line;
        struct {
            unsigned int qtd_conf;
            unsigned int mask_input_fifo;
            unsigned int mask_output_fifo;
        };
    };
} cgra_intial_conf_t;

typedef struct pe_instruction_conf_t {
    unsigned long conf_type:8;
    unsigned long pe_id:16;
    unsigned long thread_id:4;
    unsigned long inst_addr:12;
    unsigned long alu_op:4;
    unsigned long control:4;
    unsigned long rf_raddr:4;
    unsigned long rf_waddr:4;
} pe_instruction_conf_t;

typedef struct pe_constant_conf_t {
    unsigned long conf_type:8;
    unsigned long pe_id:16;
    unsigned long thread_id:4;
    unsigned long const_addr:4;
    unsigned long constant:32;
} pe_constant_conf_t;

typedef struct pe_pc_max_conf_t {
    unsigned long conf_type:8;
    unsigned long pe_id:16;
    unsigned long thread_id:8;
    unsigned long pc_max:32;
} pe_pc_max_conf_t;

typedef struct pe_pc_loop_conf_t {
    unsigned long conf_type:8;
    unsigned long pe_id:16;
    unsigned long thread_id:8;
    unsigned long pc_loop:32;
} pe_pc_loop_conf_t;

typedef struct pe_store_ignore_conf_t {
    unsigned long conf_type:8;
    unsigned long pe_id:16;
    unsigned long thread_id:8;
    unsigned long store_ignore:32;
} pe_store_ignore_conf_t;

typedef struct pe_load_ignore_conf_t {
    unsigned long conf_type:8;
    unsigned long pe_id:16;
    unsigned long thread_id:8;
    unsigned long load_ignore:32;
} pe_load_ignore_conf_t;
typedef struct pe_qtd_data_read_low_conf_t {
    unsigned long conf_type:8;
    unsigned long pe_id:16;
    unsigned long thread_id:8;
    unsigned long qtd:32;
} pe_qtd_data_read_low_conf_t;

typedef struct pe_qtd_data_read_high_conf_t {
    unsigned long conf_type:8;
    unsigned long pe_id:16;
    unsigned long thread_id:8;
    unsigned long qtd:32;
} pe_qtd_data_read_high_conf_t;

typedef struct pe_qtd_data_write_low_conf_t {
    unsigned long conf_type:8;
    unsigned long pe_id:16;
    unsigned long thread_id:8;
    unsigned long qtd:32;
} pe_qtd_data_write_low_conf_t;

typedef struct pe_qtd_data_write_high_conf_t {
    unsigned long conf_type:8;
    unsigned long pe_id:16;
    unsigned long thread_id:8;
    unsigned long qtd:32;
} pe_qtd_data_write_high_conf_t;

typedef struct net_pc_max_conf_t {
    unsigned long conf_type:8;
    unsigned long switch_number:16;
    unsigned long thread_id:8;
    unsigned long pc_max:32;
} net_pc_max_conf_t;

typedef struct net_pc_loop_conf_t {
    unsigned long conf_type:8;
    unsigned long switch_number:16;
    unsigned long thread_id:8;
    unsigned long pc_loop:32;
} net_pc_loop_conf_t;

typedef struct net_switch_conf_t {
    unsigned long conf_type:8;
    unsigned long switch_number:16;
    unsigned long thread_id:4;
    unsigned long inst_addr:12;
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
        pe_load_ignore_conf_t pe_load_ignore_conf;
        
        pe_qtd_data_read_low_conf_t pe_qtd_data_read_low_conf;
        pe_qtd_data_write_low_conf_t pe_qtd_data_write_low_conf;
        pe_qtd_data_read_high_conf_t pe_qtd_data_read_high_conf;
        pe_qtd_data_write_high_conf_t pe_qtd_data_write_high_conf;
        
        net_switch_conf_t net_switch_conf;
        net_pc_max_conf_t net_pc_max_conf;
        net_pc_loop_conf_t net_pc_loop_conf;
    };
} initial_conf_t;


typedef struct cgra_program_t {
    unsigned short cgra_id;
    unsigned short num_pe;
    unsigned short num_pe_io_in;
    unsigned short num_pe_io_out;
    unsigned short net_radix;
    unsigned short num_extra_stage;
    unsigned short word_size;
    cgra_intial_conf_t cgra_intial_conf;
    
    std::vector<initial_conf_t> initial_conf;
    //key = id thread,id dataflow,id operador, dataflow name | value cgraArch in queue id
    std::map<std::tuple<int, int, int, std::string>, int> input_map;
    //key = id thread,id dataflow,id operador, dataflow name | value cgraArch out queue id
    std::map<std::tuple<int, int, int, std::string>, int> output_map;
    //key = id thread, id pe, value = id op
    std::map< std::pair<int, int>, int> map_pe_to_op;
} cgra_program_t;


typedef enum {
    PE_BASIC,
    PE_IN,
    PE_OUT
} pe_type_t;

typedef struct qtd_conf_t {
    union {
        unsigned long qtd;
        struct {
            unsigned int qtd_low;
            unsigned int qtd_high;
        };
    };
} qtd_conf_t;

#endif //CGRA_DEFS_H
