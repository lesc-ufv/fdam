//
// Created by lucas on 17/12/18.
//

#include <common.h>

void print_program(cgra_program_t *cgra_program) {
    printf("CGRA ID: %d\n", cgra_program->cgra_id);
    printf("CGRA NUM PE: %d\n", cgra_program->num_pe);
    printf("CGRA IO IN: %d\n", cgra_program->num_pe_io_in);
    printf("CGRA IO OUT: %d\n", cgra_program->num_pe_io_out);
    printf("MASK INPUT FIFO: %x\n", cgra_program->cgra_intial_conf.mask_input_fifo);
    printf("MASK OUTPUT FIFO: %x\n", cgra_program->cgra_intial_conf.mask_output_fifo);
    for (int i = 0; i < cgra_program->cgra_intial_conf.qtd_conf; ++i) {
        switch (cgra_program->initial_conf[i].pe_instruction_conf.conf_type) {
            case CGRA_CONF_SET_PE_INSTRUCTION:
                printf("CGRA_CONF_SET_PE_INSTRUCTION: PE_ID: %lu, THREAD ID: %lu, INST_ADDR: %lu, ALU_OP: %lu, CONTROL: %lu, RF_RADDR: %lu RF_WADDR: %lu \n",
                       cgra_program->initial_conf[i].pe_instruction_conf.pe_id,
                       cgra_program->initial_conf[i].pe_instruction_conf.thread_id,
                       cgra_program->initial_conf[i].pe_instruction_conf.inst_addr,
                       cgra_program->initial_conf[i].pe_instruction_conf.alu_op,
                       cgra_program->initial_conf[i].pe_instruction_conf.control,
                       cgra_program->initial_conf[i].pe_instruction_conf.rf_raddr,
                       cgra_program->initial_conf[i].pe_instruction_conf.rf_waddr);
                break;
            case CGRA_CONF_SET_PE_CONST:
                printf("CGRA_CONF_SET_PE_CONST: PE_ID: %lu, THREAD ID: %lu, CONST_ADDR: %lu, CONST: %lu\n",
                       cgra_program->initial_conf[i].pe_constant_conf.pe_id,
                       cgra_program->initial_conf[i].pe_constant_conf.thread_id,
                       cgra_program->initial_conf[i].pe_constant_conf.const_addr,
                       cgra_program->initial_conf[i].pe_constant_conf.constant);
                break;
            case CGRA_CONF_SET_PE_PC_MAX:
                printf("CGRA_CONF_SET_PE_PC_MAX: PE_ID: %lu, THREAD ID: %lu, PC_MAX: %lu\n",
                       cgra_program->initial_conf[i].pe_pc_max_conf.pe_id,
                       cgra_program->initial_conf[i].pe_pc_max_conf.thread_id,
                       cgra_program->initial_conf[i].pe_pc_max_conf.pc_max);
                break;
            case CGRA_CONF_SET_PE_PC_LOOP:
                printf("CGRA_CONF_SET_PE_PC_LOOP: PE_ID: %lu, THREAD ID: %lu, PC_LOOP: %lu\n",
                       cgra_program->initial_conf[i].pe_pc_loop_conf.pe_id,
                       cgra_program->initial_conf[i].pe_pc_loop_conf.thread_id,
                       cgra_program->initial_conf[i].pe_pc_loop_conf.pc_loop);
                break;
            case CGRA_CONF_SET_PE_IGNORE:
                printf("CGRA_CONF_SET_PE_IGNORE: PE_ID: %lu, THREAD ID: %lu, IGNORE: %lu\n",
                       cgra_program->initial_conf[i].pe_store_ignore_conf.pe_id,
                       cgra_program->initial_conf[i].pe_store_ignore_conf.thread_id,
                       cgra_program->initial_conf[i].pe_store_ignore_conf.store_ignore);
                break;
            case CGRA_CONF_SET_NET_PC_MAX:
                printf("CGRA_CONF_SET_NET_PC_MAX: SWITCH_NUMBER: %lu, THREAD ID: %lu, PC_MAX: %lu\n",
                       cgra_program->initial_conf[i].net_pc_max_conf.switch_number,
                       cgra_program->initial_conf[i].net_pc_max_conf.thread_id,
                       cgra_program->initial_conf[i].net_pc_max_conf.pc_max);
                break;
            case CGRA_CONF_SET_NET_PC_LOOP:
                printf("CGRA_CONF_SET_NET_PC_LOOP: SWITCH_NUMBER: %lu, THREAD ID: %lu, PC_LOOP: %lu\n",
                       cgra_program->initial_conf[i].net_pc_loop_conf.switch_number,
                       cgra_program->initial_conf[i].net_pc_loop_conf.thread_id,
                       cgra_program->initial_conf[i].net_pc_loop_conf.pc_loop);
                break;
            case CGRA_CONF_NET_SWITCH:
                printf("CGRA_CONF_NET_SWITCH: SWITCH_NUMBER: %lu, THREAD ID: %lu, INST_ADDR: %lu, SWITCH_CONF: %08lX\n",
                       cgra_program->initial_conf[i].net_switch_conf.switch_number,
                       cgra_program->initial_conf[i].net_switch_conf.thread_id,
                       cgra_program->initial_conf[i].net_switch_conf.inst_addr,
                       cgra_program->initial_conf[i].net_switch_conf.switch_conf);
                break;
            default:
                break;
        }
    }
}
