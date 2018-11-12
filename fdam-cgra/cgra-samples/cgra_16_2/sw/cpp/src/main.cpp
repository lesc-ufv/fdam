#include <main.h>

int main(int argc, char *argv[]) {

    int data_num = 128;
    auto data_in = (short *) malloc(sizeof(short) * data_num);

    auto cpu_data_out = (short *) malloc(sizeof(short) * data_num);
    auto cgra_data_out = (short *) malloc(sizeof(short) * data_num);

    short const_vet[4] = {1, 2, 3, 4};

    for (int i = 0; i < data_num; ++i) {
        data_in[i] = static_cast<short>(i+1);
        cpu_data_out[i] = 0;
        cgra_data_out[i] = 0;

    }
    fir4_cgra(data_in, cgra_data_out, data_num, const_vet);

    fir4_cpu(data_in, cpu_data_out, data_num, const_vet);

    printf("FIR4 DATA IN:\n");
    for (int i = 0; i < data_num; ++i) {
        printf("%d ", data_in[i]);
    }
    printf("\n");

    printf("FIR4 CGRA DATA OUT:\n");
    for (int i = 0; i < data_num; ++i) {
        printf("%d ", cgra_data_out[i]);
    }
    printf("\n");

    printf("FIR4 CPU DATA OUT:\n");
    for (int i = 0; i < data_num; ++i) {
        printf("%d ", cpu_data_out[i]);
    }
    printf("\n");

}

void fir4_cpu(const short *data_in, short *data_out, int n, const short *const_vet) {
    for (int i = 0; i < n - 4; i++) {
        data_out[i] = (data_in[i] * const_vet[3]) + (data_in[i + 1] * const_vet[2]) + (data_in[i + 2] * const_vet[1]) +
                      (data_in[i + 3] * const_vet[0]);
    }
}

void fir4_cgra(short *data_in, short *data_out, int n, const short *const_vet) {
    cgra_program_t *cgra_program = create_fir4_cgra_program(data_in, n, data_out, n - 4, const_vet);
    auto cgra = new Cgra(16, 2, 4);
    cgra->prepareProgram(cgra_program);
    cgra->syncExecute(3000);
    delete cgra;
}


cgra_program_t *create_fir4_cgra_program(short *data_in, int data_in_size, short *data_out, int data_out_size,
                                         const short *const_vet) {

    auto *cgra_prog = (cgra_program_t *) malloc(sizeof(cgra_program_t));

    cgra_prog->cgra_id = 0;

    memset(&cgra_prog->cgra_intial_conf, 0, sizeof(cgra_intial_conf_t));

    cgra_prog->cgra_intial_conf.read_fifo_mask = 0x1;
    cgra_prog->cgra_intial_conf.write_fifo_mask = 0x2;

    {
        cgra_prog->cgra_intial_conf.qtd_pe_conf = 21;
        size_t len = 21 * sizeof(pe_initial_conf_t);
        cgra_prog->pe_initial_conf = (pe_initial_conf_t *) malloc(len);
        cgra_prog->pe_initial_conf[0].op_conf = CGRA_CONF_SET_INSTRUCTION;
        cgra_prog->pe_initial_conf[0].id_pe = 1;
        cgra_prog->pe_initial_conf[0].pe_instruction.inst_addr = 0;
        cgra_prog->pe_initial_conf[0].pe_instruction.alu_op = 0;
        cgra_prog->pe_initial_conf[0].pe_instruction.alu_in_a = 3;
        cgra_prog->pe_initial_conf[0].pe_instruction.alu_in_b = 0;
        cgra_prog->pe_initial_conf[0].pe_instruction.alu_out = 0;
        cgra_prog->pe_initial_conf[0].pe_instruction.rf_raddr = 0;
        cgra_prog->pe_initial_conf[0].pe_instruction.rf_waddr = 0;

        cgra_prog->pe_initial_conf[1].op_conf = CGRA_CONF_SET_INSTRUCTION;
        cgra_prog->pe_initial_conf[1].id_pe = 2;
        cgra_prog->pe_initial_conf[1].pe_instruction.inst_addr = 0;
        cgra_prog->pe_initial_conf[1].pe_instruction.alu_op = 0;
        cgra_prog->pe_initial_conf[1].pe_instruction.alu_in_a = 0;
        cgra_prog->pe_initial_conf[1].pe_instruction.alu_in_b = 0;
        cgra_prog->pe_initial_conf[1].pe_instruction.alu_out = 3;
        cgra_prog->pe_initial_conf[1].pe_instruction.rf_raddr = 0;
        cgra_prog->pe_initial_conf[1].pe_instruction.rf_waddr = 0;

        cgra_prog->pe_initial_conf[2].op_conf = CGRA_CONF_SET_INSTRUCTION;
        cgra_prog->pe_initial_conf[2].id_pe = 3;
        cgra_prog->pe_initial_conf[2].pe_instruction.inst_addr = 0;
        cgra_prog->pe_initial_conf[2].pe_instruction.alu_op = 4;
        cgra_prog->pe_initial_conf[2].pe_instruction.alu_in_a = 0;
        cgra_prog->pe_initial_conf[2].pe_instruction.alu_in_b = 2;
        cgra_prog->pe_initial_conf[2].pe_instruction.alu_out = 0;
        cgra_prog->pe_initial_conf[2].pe_instruction.rf_raddr = 0;
        cgra_prog->pe_initial_conf[2].pe_instruction.rf_waddr = 0;

        cgra_prog->pe_initial_conf[3].op_conf = CGRA_CONF_SET_INSTRUCTION;
        cgra_prog->pe_initial_conf[3].id_pe = 4;
        cgra_prog->pe_initial_conf[3].pe_instruction.inst_addr = 0;
        cgra_prog->pe_initial_conf[3].pe_instruction.alu_op = 0;
        cgra_prog->pe_initial_conf[3].pe_instruction.alu_in_a = 0;
        cgra_prog->pe_initial_conf[3].pe_instruction.alu_in_b = 0;
        cgra_prog->pe_initial_conf[3].pe_instruction.alu_out = 0;
        cgra_prog->pe_initial_conf[3].pe_instruction.rf_raddr = 0;
        cgra_prog->pe_initial_conf[3].pe_instruction.rf_waddr = 0;

        cgra_prog->pe_initial_conf[4].op_conf = CGRA_CONF_SET_INSTRUCTION;
        cgra_prog->pe_initial_conf[4].id_pe = 5;
        cgra_prog->pe_initial_conf[4].pe_instruction.inst_addr = 0;
        cgra_prog->pe_initial_conf[4].pe_instruction.alu_op = 0;
        cgra_prog->pe_initial_conf[4].pe_instruction.alu_in_a = 0;
        cgra_prog->pe_initial_conf[4].pe_instruction.alu_in_b = 0;
        cgra_prog->pe_initial_conf[4].pe_instruction.alu_out = 1;
        cgra_prog->pe_initial_conf[4].pe_instruction.rf_raddr = 0;
        cgra_prog->pe_initial_conf[4].pe_instruction.rf_waddr = 0;

        cgra_prog->pe_initial_conf[5].op_conf = CGRA_CONF_SET_INSTRUCTION;
        cgra_prog->pe_initial_conf[5].id_pe = 6;
        cgra_prog->pe_initial_conf[5].pe_instruction.inst_addr = 0;
        cgra_prog->pe_initial_conf[5].pe_instruction.alu_op = 4;
        cgra_prog->pe_initial_conf[5].pe_instruction.alu_in_a = 0;
        cgra_prog->pe_initial_conf[5].pe_instruction.alu_in_b = 2;
        cgra_prog->pe_initial_conf[5].pe_instruction.alu_out = 0;
        cgra_prog->pe_initial_conf[5].pe_instruction.rf_raddr = 0;
        cgra_prog->pe_initial_conf[5].pe_instruction.rf_waddr = 0;

        cgra_prog->pe_initial_conf[6].op_conf = CGRA_CONF_SET_INSTRUCTION;
        cgra_prog->pe_initial_conf[6].id_pe = 7;
        cgra_prog->pe_initial_conf[6].pe_instruction.inst_addr = 0;
        cgra_prog->pe_initial_conf[6].pe_instruction.alu_op = 0;
        cgra_prog->pe_initial_conf[6].pe_instruction.alu_in_a = 0;
        cgra_prog->pe_initial_conf[6].pe_instruction.alu_in_b = 0;
        cgra_prog->pe_initial_conf[6].pe_instruction.alu_out = 0;
        cgra_prog->pe_initial_conf[6].pe_instruction.rf_raddr = 0;
        cgra_prog->pe_initial_conf[6].pe_instruction.rf_waddr = 0;

        cgra_prog->pe_initial_conf[7].op_conf = CGRA_CONF_SET_INSTRUCTION;
        cgra_prog->pe_initial_conf[7].id_pe = 8;
        cgra_prog->pe_initial_conf[7].pe_instruction.inst_addr = 0;
        cgra_prog->pe_initial_conf[7].pe_instruction.alu_op = 2;
        cgra_prog->pe_initial_conf[7].pe_instruction.alu_in_a = 0;
        cgra_prog->pe_initial_conf[7].pe_instruction.alu_in_b = 1;
        cgra_prog->pe_initial_conf[7].pe_instruction.alu_out = 0;
        cgra_prog->pe_initial_conf[7].pe_instruction.rf_raddr = 0;
        cgra_prog->pe_initial_conf[7].pe_instruction.rf_waddr = 0;

        cgra_prog->pe_initial_conf[8].op_conf = CGRA_CONF_SET_INSTRUCTION;
        cgra_prog->pe_initial_conf[8].id_pe = 9;
        cgra_prog->pe_initial_conf[8].pe_instruction.inst_addr = 0;
        cgra_prog->pe_initial_conf[8].pe_instruction.alu_op = 4;
        cgra_prog->pe_initial_conf[8].pe_instruction.alu_in_a = 0;
        cgra_prog->pe_initial_conf[8].pe_instruction.alu_in_b = 2;
        cgra_prog->pe_initial_conf[8].pe_instruction.alu_out = 0;
        cgra_prog->pe_initial_conf[8].pe_instruction.rf_raddr = 0;
        cgra_prog->pe_initial_conf[8].pe_instruction.rf_waddr = 0;

        cgra_prog->pe_initial_conf[9].op_conf = CGRA_CONF_SET_INSTRUCTION;
        cgra_prog->pe_initial_conf[9].id_pe = 10;
        cgra_prog->pe_initial_conf[9].pe_instruction.inst_addr = 0;
        cgra_prog->pe_initial_conf[9].pe_instruction.alu_op = 0;
        cgra_prog->pe_initial_conf[9].pe_instruction.alu_in_a = 0;
        cgra_prog->pe_initial_conf[9].pe_instruction.alu_in_b = 0;
        cgra_prog->pe_initial_conf[9].pe_instruction.alu_out = 1;
        cgra_prog->pe_initial_conf[9].pe_instruction.rf_raddr = 0;
        cgra_prog->pe_initial_conf[9].pe_instruction.rf_waddr = 0;

        cgra_prog->pe_initial_conf[10].op_conf = CGRA_CONF_SET_INSTRUCTION;
        cgra_prog->pe_initial_conf[10].id_pe = 11;
        cgra_prog->pe_initial_conf[10].pe_instruction.inst_addr = 0;
        cgra_prog->pe_initial_conf[10].pe_instruction.alu_op = 2;
        cgra_prog->pe_initial_conf[10].pe_instruction.alu_in_a = 0;
        cgra_prog->pe_initial_conf[10].pe_instruction.alu_in_b = 1;
        cgra_prog->pe_initial_conf[10].pe_instruction.alu_out = 0;
        cgra_prog->pe_initial_conf[10].pe_instruction.rf_raddr = 0;
        cgra_prog->pe_initial_conf[10].pe_instruction.rf_waddr = 0;

        cgra_prog->pe_initial_conf[11].op_conf = CGRA_CONF_SET_INSTRUCTION;
        cgra_prog->pe_initial_conf[11].id_pe = 12;
        cgra_prog->pe_initial_conf[11].pe_instruction.inst_addr = 0;
        cgra_prog->pe_initial_conf[11].pe_instruction.alu_op = 4;
        cgra_prog->pe_initial_conf[11].pe_instruction.alu_in_a = 0;
        cgra_prog->pe_initial_conf[11].pe_instruction.alu_in_b = 2;
        cgra_prog->pe_initial_conf[11].pe_instruction.alu_out = 1;
        cgra_prog->pe_initial_conf[11].pe_instruction.rf_raddr = 0;
        cgra_prog->pe_initial_conf[11].pe_instruction.rf_waddr = 0;

        cgra_prog->pe_initial_conf[12].op_conf = CGRA_CONF_SET_INSTRUCTION;
        cgra_prog->pe_initial_conf[12].id_pe = 13;
        cgra_prog->pe_initial_conf[12].pe_instruction.inst_addr = 0;
        cgra_prog->pe_initial_conf[12].pe_instruction.alu_op = 2;
        cgra_prog->pe_initial_conf[12].pe_instruction.alu_in_a = 0;
        cgra_prog->pe_initial_conf[12].pe_instruction.alu_in_b = 1;
        cgra_prog->pe_initial_conf[12].pe_instruction.alu_out = 0;
        cgra_prog->pe_initial_conf[12].pe_instruction.rf_raddr = 0;
        cgra_prog->pe_initial_conf[12].pe_instruction.rf_waddr = 0;

        cgra_prog->pe_initial_conf[13].op_conf = CGRA_CONF_SET_INSTRUCTION;
        cgra_prog->pe_initial_conf[13].id_pe = 14;
        cgra_prog->pe_initial_conf[13].pe_instruction.inst_addr = 0;
        cgra_prog->pe_initial_conf[13].pe_instruction.alu_op = 0;
        cgra_prog->pe_initial_conf[13].pe_instruction.alu_in_a = 0;
        cgra_prog->pe_initial_conf[13].pe_instruction.alu_in_b = 0;
        cgra_prog->pe_initial_conf[13].pe_instruction.alu_out = 0;
        cgra_prog->pe_initial_conf[13].pe_instruction.rf_raddr = 0;
        cgra_prog->pe_initial_conf[13].pe_instruction.rf_waddr = 0;

        cgra_prog->pe_initial_conf[14].op_conf = CGRA_CONF_SET_INSTRUCTION;
        cgra_prog->pe_initial_conf[14].id_pe = 15;
        cgra_prog->pe_initial_conf[14].pe_instruction.inst_addr = 0;
        cgra_prog->pe_initial_conf[14].pe_instruction.alu_op = 0;
        cgra_prog->pe_initial_conf[14].pe_instruction.alu_in_a = 0;
        cgra_prog->pe_initial_conf[14].pe_instruction.alu_in_b = 0;
        cgra_prog->pe_initial_conf[14].pe_instruction.alu_out = 0;
        cgra_prog->pe_initial_conf[14].pe_instruction.rf_raddr = 0;
        cgra_prog->pe_initial_conf[14].pe_instruction.rf_waddr = 0;

        cgra_prog->pe_initial_conf[15].op_conf = CGRA_CONF_SET_INSTRUCTION;
        cgra_prog->pe_initial_conf[15].id_pe = 16;
        cgra_prog->pe_initial_conf[15].pe_instruction.inst_addr = 0;
        cgra_prog->pe_initial_conf[15].pe_instruction.alu_op = 0;
        cgra_prog->pe_initial_conf[15].pe_instruction.alu_in_a = 0;
        cgra_prog->pe_initial_conf[15].pe_instruction.alu_in_b = 0;
        cgra_prog->pe_initial_conf[15].pe_instruction.alu_out = 0;
        cgra_prog->pe_initial_conf[15].pe_instruction.rf_raddr = 0;
        cgra_prog->pe_initial_conf[15].pe_instruction.rf_waddr = 0;

        cgra_prog->pe_initial_conf[16].id_pe = 2;
        cgra_prog->pe_initial_conf[16].pe_instruction.inst_addr = 0;
        cgra_prog->pe_initial_conf[16].op_conf = CGRA_CONF_SET_IGNORE;
        cgra_prog->pe_initial_conf[16].store_ignore = 49;

        cgra_prog->pe_initial_conf[17].id_pe = 3;
        cgra_prog->pe_initial_conf[17].op_conf = CGRA_CONF_SET_CONST;
        cgra_prog->pe_initial_conf[17].const_addr = 0;
        cgra_prog->pe_initial_conf[17].constant = (unsigned int) const_vet[3];

        cgra_prog->pe_initial_conf[18].id_pe = 6;
        cgra_prog->pe_initial_conf[18].op_conf = CGRA_CONF_SET_CONST;
        cgra_prog->pe_initial_conf[18].const_addr = 0;
        cgra_prog->pe_initial_conf[18].constant = (unsigned int) const_vet[2];

        cgra_prog->pe_initial_conf[19].id_pe = 9;
        cgra_prog->pe_initial_conf[19].op_conf = CGRA_CONF_SET_CONST;
        cgra_prog->pe_initial_conf[19].const_addr = 0;
        cgra_prog->pe_initial_conf[19].constant = (unsigned int) const_vet[1];

        cgra_prog->pe_initial_conf[20].id_pe = 12;
        cgra_prog->pe_initial_conf[20].op_conf = CGRA_CONF_SET_CONST;
        cgra_prog->pe_initial_conf[20].const_addr = 0;
        cgra_prog->pe_initial_conf[20].constant = (unsigned int) const_vet[0];

    }

    {

        cgra_prog->net_conf = (net_conf_t *) malloc(sizeof(net_conf_t));
        cgra_prog->net_conf->swicth_conf = (swicth_conf_t *) malloc(sizeof(swicth_conf_t) * 24);
        cgra_prog->cgra_intial_conf.qtd_net_conf = 1;

        // stage 1:
        cgra_prog->net_conf[0].swicth_conf[0].out0 = 0;
        cgra_prog->net_conf[0].swicth_conf[0].out1 = 0;
        cgra_prog->net_conf[0].swicth_conf[0].out2 = 3;
        cgra_prog->net_conf[0].swicth_conf[0].out3 = 2;

        cgra_prog->net_conf[0].swicth_conf[1].out0 = 1;
        cgra_prog->net_conf[0].swicth_conf[1].out1 = 0;
        cgra_prog->net_conf[0].swicth_conf[1].out2 = 0;
        cgra_prog->net_conf[0].swicth_conf[1].out3 = 0;

        cgra_prog->net_conf[0].swicth_conf[2].out0 = 1;
        cgra_prog->net_conf[0].swicth_conf[2].out1 = 0;
        cgra_prog->net_conf[0].swicth_conf[2].out2 = 0;
        cgra_prog->net_conf[0].swicth_conf[2].out3 = 0;

        cgra_prog->net_conf[0].swicth_conf[3].out0 = 0;
        cgra_prog->net_conf[0].swicth_conf[3].out1 = 0;
        cgra_prog->net_conf[0].swicth_conf[3].out2 = 0;
        cgra_prog->net_conf[0].swicth_conf[3].out3 = 2;

        cgra_prog->net_conf[0].swicth_conf[4].out0 = 0;
        cgra_prog->net_conf[0].swicth_conf[4].out1 = 1;
        cgra_prog->net_conf[0].swicth_conf[4].out2 = 0;
        cgra_prog->net_conf[0].swicth_conf[4].out3 = 2;

        cgra_prog->net_conf[0].swicth_conf[5].out0 = 0;
        cgra_prog->net_conf[0].swicth_conf[5].out1 = 0;
        cgra_prog->net_conf[0].swicth_conf[5].out2 = 0;
        cgra_prog->net_conf[0].swicth_conf[5].out3 = 0;

        cgra_prog->net_conf[0].swicth_conf[6].out0 = 0;
        cgra_prog->net_conf[0].swicth_conf[6].out1 = 1;
        cgra_prog->net_conf[0].swicth_conf[6].out2 = 0;
        cgra_prog->net_conf[0].swicth_conf[6].out3 = 0;

        cgra_prog->net_conf[0].swicth_conf[7].out0 = 0;
        cgra_prog->net_conf[0].swicth_conf[7].out1 = 2;
        cgra_prog->net_conf[0].swicth_conf[7].out2 = 0;
        cgra_prog->net_conf[0].swicth_conf[7].out3 = 0;
        // stage 2:
        cgra_prog->net_conf[0].swicth_conf[8].out0 = 0;
        cgra_prog->net_conf[0].swicth_conf[8].out1 = 0;
        cgra_prog->net_conf[0].swicth_conf[8].out2 = 2;
        cgra_prog->net_conf[0].swicth_conf[8].out3 = 1;

        cgra_prog->net_conf[0].swicth_conf[9].out0 = 2;
        cgra_prog->net_conf[0].swicth_conf[9].out1 = 3;
        cgra_prog->net_conf[0].swicth_conf[9].out2 = 0;
        cgra_prog->net_conf[0].swicth_conf[9].out3 = 0;

        cgra_prog->net_conf[0].swicth_conf[10].out0 = 0;
        cgra_prog->net_conf[0].swicth_conf[10].out1 = 0;
        cgra_prog->net_conf[0].swicth_conf[10].out2 = 3;
        cgra_prog->net_conf[0].swicth_conf[10].out3 = 3;

        cgra_prog->net_conf[0].swicth_conf[11].out0 = 0;
        cgra_prog->net_conf[0].swicth_conf[11].out1 = 0;
        cgra_prog->net_conf[0].swicth_conf[11].out2 = 2;
        cgra_prog->net_conf[0].swicth_conf[11].out3 = 0;

        cgra_prog->net_conf[0].swicth_conf[12].out0 = 0;
        cgra_prog->net_conf[0].swicth_conf[12].out1 = 0;
        cgra_prog->net_conf[0].swicth_conf[12].out2 = 0;
        cgra_prog->net_conf[0].swicth_conf[12].out3 = 0;

        cgra_prog->net_conf[0].swicth_conf[13].out0 = 0;
        cgra_prog->net_conf[0].swicth_conf[13].out1 = 0;
        cgra_prog->net_conf[0].swicth_conf[13].out2 = 3;
        cgra_prog->net_conf[0].swicth_conf[13].out3 = 0;

        cgra_prog->net_conf[0].swicth_conf[14].out0 = 0;
        cgra_prog->net_conf[0].swicth_conf[14].out1 = 0;
        cgra_prog->net_conf[0].swicth_conf[14].out2 = 0;
        cgra_prog->net_conf[0].swicth_conf[14].out3 = 2;

        cgra_prog->net_conf[0].swicth_conf[15].out0 = 0;
        cgra_prog->net_conf[0].swicth_conf[15].out1 = 1;
        cgra_prog->net_conf[0].swicth_conf[15].out2 = 0;
        cgra_prog->net_conf[0].swicth_conf[15].out3 = 0;
        // stage 3:
        cgra_prog->net_conf[0].swicth_conf[16].out0 = 0;
        cgra_prog->net_conf[0].swicth_conf[16].out1 = 0;
        cgra_prog->net_conf[0].swicth_conf[16].out2 = 1;
        cgra_prog->net_conf[0].swicth_conf[16].out3 = 0;

        cgra_prog->net_conf[0].swicth_conf[17].out0 = 0;
        cgra_prog->net_conf[0].swicth_conf[17].out1 = 0;
        cgra_prog->net_conf[0].swicth_conf[17].out2 = 0;
        cgra_prog->net_conf[0].swicth_conf[17].out3 = 0;

        cgra_prog->net_conf[0].swicth_conf[18].out0 = 0;
        cgra_prog->net_conf[0].swicth_conf[18].out1 = 0;
        cgra_prog->net_conf[0].swicth_conf[18].out2 = 1;
        cgra_prog->net_conf[0].swicth_conf[18].out3 = 0;

        cgra_prog->net_conf[0].swicth_conf[19].out0 = 1;
        cgra_prog->net_conf[0].swicth_conf[19].out1 = 0;
        cgra_prog->net_conf[0].swicth_conf[19].out2 = 2;
        cgra_prog->net_conf[0].swicth_conf[19].out3 = 0;

        cgra_prog->net_conf[0].swicth_conf[20].out0 = 0;
        cgra_prog->net_conf[0].swicth_conf[20].out1 = 0;
        cgra_prog->net_conf[0].swicth_conf[20].out2 = 0;
        cgra_prog->net_conf[0].swicth_conf[20].out3 = 0;

        cgra_prog->net_conf[0].swicth_conf[21].out0 = 0;
        cgra_prog->net_conf[0].swicth_conf[21].out1 = 1;
        cgra_prog->net_conf[0].swicth_conf[21].out2 = 3;
        cgra_prog->net_conf[0].swicth_conf[21].out3 = 0;

        cgra_prog->net_conf[0].swicth_conf[22].out0 = 1;
        cgra_prog->net_conf[0].swicth_conf[22].out1 = 2;
        cgra_prog->net_conf[0].swicth_conf[22].out2 = 0;
        cgra_prog->net_conf[0].swicth_conf[22].out3 = 0;

        cgra_prog->net_conf[0].swicth_conf[23].out0 = 0;
        cgra_prog->net_conf[0].swicth_conf[23].out1 = 0;
        cgra_prog->net_conf[0].swicth_conf[23].out2 = 0;
        cgra_prog->net_conf[0].swicth_conf[23].out3 = 0;
    }

    auto input_queues = (queue_t *) malloc(sizeof(queue_t) * 2);
    auto output_queues = (queue_t *) malloc(sizeof(queue_t) * 2);

    input_queues[0].data = data_in;
    input_queues[0].length = data_in_size;

    output_queues[1].data = data_out;
    output_queues[1].length = data_out_size;

    cgra_prog->input_queues = input_queues;
    cgra_prog->output_queues = output_queues;

    return cgra_prog;
}
