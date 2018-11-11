#include <main.h>

int main(int argc, char *argv[]) {

    int data_num = 128;
    auto data_in = (short *) malloc(sizeof(short) * data_num);
    
    auto cpu_data_out = (short *) malloc(sizeof(short) * data_num);
    auto cgra_data_out = (short *) malloc(sizeof(short) * data_num);
    
    short const_vet[4] = {1, 2, 3, 4};

    for (int i = 0; i < data_num; ++i) {
        data_in[i] = static_cast<short>(i);
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
    cgra_program_t *cgra_program = create_fir4_cgra_program(data_in, n, data_out, n-4, const_vet);
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

    auto init_conf = (pe_initial_conf_t *) malloc(sizeof(pe_initial_conf_t) * 16);
    cgra_prog->pe_initial_conf = init_conf;
    cgra_prog->cgra_intial_conf.qtd_pe_init_conf = 16;
    {

        init_conf[0].pe_id = 1;
        init_conf[0].pe_const = 0;
        init_conf[0].pc_max = 0;
        init_conf[0].loop = 0;
        init_conf[0].ignore = 0;

        init_conf[1].pe_id = 2;
        init_conf[1].pe_const = 0;
        init_conf[1].pc_max = 0;
        init_conf[1].loop = 0;
        init_conf[1].ignore = 49;

        init_conf[2].pe_id = 3;
        init_conf[2].pe_const = const_vet[3];
        init_conf[2].pc_max = 0;
        init_conf[2].loop = 0;
        init_conf[2].ignore = 0;

        init_conf[3].pe_id = 4;
        init_conf[3].pe_const = 0;
        init_conf[3].pc_max = 0;
        init_conf[3].loop = 0;
        init_conf[3].ignore = 0;

        init_conf[4].pe_id = 5;
        init_conf[4].pe_const = 0;
        init_conf[4].pc_max = 0;
        init_conf[4].loop = 0;
        init_conf[4].ignore = 0;

        init_conf[5].pe_id = 6;
        init_conf[5].pe_const = const_vet[2];
        init_conf[5].pc_max = 0;
        init_conf[5].loop = 0;
        init_conf[5].ignore = 0;

        init_conf[6].pe_id = 7;
        init_conf[6].pe_const = 0;
        init_conf[6].pc_max = 0;
        init_conf[6].loop = 0;
        init_conf[6].ignore = 0;

        init_conf[7].pe_id = 8;
        init_conf[7].pe_const = 0;
        init_conf[7].pc_max = 0;
        init_conf[7].loop = 0;
        init_conf[7].ignore = 0;

        init_conf[8].pe_id = 9;
        init_conf[8].pe_const = const_vet[1];
        init_conf[8].pc_max = 0;
        init_conf[8].loop = 0;
        init_conf[8].ignore = 0;

        init_conf[9].pe_id = 10;
        init_conf[9].pe_const = 0;
        init_conf[9].pc_max = 0;
        init_conf[9].loop = 0;
        init_conf[9].ignore = 0;

        init_conf[10].pe_id = 11;
        init_conf[10].pe_const = 0;
        init_conf[10].pc_max = 0;
        init_conf[10].loop = 0;
        init_conf[10].ignore = 0;

        init_conf[11].pe_id = 12;
        init_conf[11].pe_const = const_vet[0];
        init_conf[11].pc_max = 0;
        init_conf[11].loop = 0;
        init_conf[11].ignore = 0;

        init_conf[12].pe_id = 13;
        init_conf[12].pe_const = 0;
        init_conf[12].pc_max = 0;
        init_conf[12].loop = 0;
        init_conf[12].ignore = 0;

        init_conf[13].pe_id = 14;
        init_conf[13].pe_const = 0;
        init_conf[13].pc_max = 0;
        init_conf[13].loop = 0;
        init_conf[13].ignore = 0;

        init_conf[14].pe_id = 15;
        init_conf[14].pe_const = 0;
        init_conf[14].pc_max = 0;
        init_conf[14].loop = 0;
        init_conf[14].ignore = 0;

        init_conf[15].pe_id = 16;
        init_conf[15].pe_const = 0;
        init_conf[15].pc_max = 0;
        init_conf[15].loop = 0;
        init_conf[15].ignore = 0;
    }

    cgra_prog->pe_conf = (pe_conf_t *) malloc(sizeof(pe_conf_t) * 16);;
    cgra_prog->cgra_intial_conf.qtd_pe_conf = 16;
    {
        cgra_prog->pe_conf[0].pe_id = 1;
        cgra_prog->pe_conf[0].alu_op = 0;
        cgra_prog->pe_conf[0].alu_in_a = 3;
        cgra_prog->pe_conf[0].alu_in_b = 0;
        cgra_prog->pe_conf[0].alu_out = 0;
        cgra_prog->pe_conf[0].rf_raddr = 0;
        cgra_prog->pe_conf[0].rf_waddr = 0;

        cgra_prog->pe_conf[1].pe_id = 2;
        cgra_prog->pe_conf[1].alu_op = 0;
        cgra_prog->pe_conf[1].alu_in_a = 0;
        cgra_prog->pe_conf[1].alu_in_b = 0;
        cgra_prog->pe_conf[1].alu_out = 3;
        cgra_prog->pe_conf[1].rf_raddr = 0;
        cgra_prog->pe_conf[1].rf_waddr = 0;

        cgra_prog->pe_conf[2].pe_id = 3;
        cgra_prog->pe_conf[2].alu_op = 4;
        cgra_prog->pe_conf[2].alu_in_a = 0;
        cgra_prog->pe_conf[2].alu_in_b = 2;
        cgra_prog->pe_conf[2].alu_out = 0;
        cgra_prog->pe_conf[2].rf_raddr = 0;
        cgra_prog->pe_conf[2].rf_waddr = 0;

        cgra_prog->pe_conf[3].pe_id = 4;
        cgra_prog->pe_conf[3].alu_op = 0;
        cgra_prog->pe_conf[3].alu_in_a = 0;
        cgra_prog->pe_conf[3].alu_in_b = 0;
        cgra_prog->pe_conf[3].alu_out = 0;
        cgra_prog->pe_conf[3].rf_raddr = 0;
        cgra_prog->pe_conf[3].rf_waddr = 0;

        cgra_prog->pe_conf[4].pe_id = 5;
        cgra_prog->pe_conf[4].alu_op = 0;
        cgra_prog->pe_conf[4].alu_in_a = 0;
        cgra_prog->pe_conf[4].alu_in_b = 0;
        cgra_prog->pe_conf[4].alu_out = 1;
        cgra_prog->pe_conf[4].rf_raddr = 0;
        cgra_prog->pe_conf[4].rf_waddr = 0;

        cgra_prog->pe_conf[5].pe_id = 6;
        cgra_prog->pe_conf[5].alu_op = 4;
        cgra_prog->pe_conf[5].alu_in_a = 0;
        cgra_prog->pe_conf[5].alu_in_b = 2;
        cgra_prog->pe_conf[5].alu_out = 0;
        cgra_prog->pe_conf[5].rf_raddr = 0;
        cgra_prog->pe_conf[5].rf_waddr = 0;

        cgra_prog->pe_conf[6].pe_id = 7;
        cgra_prog->pe_conf[6].alu_op = 0;
        cgra_prog->pe_conf[6].alu_in_a = 0;
        cgra_prog->pe_conf[6].alu_in_b = 0;
        cgra_prog->pe_conf[6].alu_out = 0;
        cgra_prog->pe_conf[6].rf_raddr = 0;
        cgra_prog->pe_conf[6].rf_waddr = 0;

        cgra_prog->pe_conf[7].pe_id = 8;
        cgra_prog->pe_conf[7].alu_op = 2;
        cgra_prog->pe_conf[7].alu_in_a = 0;
        cgra_prog->pe_conf[7].alu_in_b = 1;
        cgra_prog->pe_conf[7].alu_out = 0;
        cgra_prog->pe_conf[7].rf_raddr = 0;
        cgra_prog->pe_conf[7].rf_waddr = 0;

        cgra_prog->pe_conf[8].pe_id = 9;
        cgra_prog->pe_conf[8].alu_op = 4;
        cgra_prog->pe_conf[8].alu_in_a = 0;
        cgra_prog->pe_conf[8].alu_in_b = 2;
        cgra_prog->pe_conf[8].alu_out = 0;
        cgra_prog->pe_conf[8].rf_raddr = 0;
        cgra_prog->pe_conf[8].rf_waddr = 0;

        cgra_prog->pe_conf[9].pe_id = 10;
        cgra_prog->pe_conf[9].alu_op = 0;
        cgra_prog->pe_conf[9].alu_in_a = 0;
        cgra_prog->pe_conf[9].alu_in_b = 0;
        cgra_prog->pe_conf[9].alu_out = 1;
        cgra_prog->pe_conf[9].rf_raddr = 0;
        cgra_prog->pe_conf[9].rf_waddr = 0;

        cgra_prog->pe_conf[10].pe_id = 11;
        cgra_prog->pe_conf[10].alu_op = 2;
        cgra_prog->pe_conf[10].alu_in_a = 0;
        cgra_prog->pe_conf[10].alu_in_b = 1;
        cgra_prog->pe_conf[10].alu_out = 0;
        cgra_prog->pe_conf[10].rf_raddr = 0;
        cgra_prog->pe_conf[10].rf_waddr = 0;

        cgra_prog->pe_conf[11].pe_id = 12;
        cgra_prog->pe_conf[11].alu_op = 4;
        cgra_prog->pe_conf[11].alu_in_a = 0;
        cgra_prog->pe_conf[11].alu_in_b = 2;
        cgra_prog->pe_conf[11].alu_out = 1;
        cgra_prog->pe_conf[11].rf_raddr = 0;
        cgra_prog->pe_conf[11].rf_waddr = 0;

        cgra_prog->pe_conf[12].pe_id = 13;
        cgra_prog->pe_conf[12].alu_op = 2;
        cgra_prog->pe_conf[12].alu_in_a = 0;
        cgra_prog->pe_conf[12].alu_in_b = 1;
        cgra_prog->pe_conf[12].alu_out = 0;
        cgra_prog->pe_conf[12].rf_raddr = 0;
        cgra_prog->pe_conf[12].rf_waddr = 0;

        cgra_prog->pe_conf[13].pe_id = 14;
        cgra_prog->pe_conf[13].alu_op = 0;
        cgra_prog->pe_conf[13].alu_in_a = 0;
        cgra_prog->pe_conf[13].alu_in_b = 0;
        cgra_prog->pe_conf[13].alu_out = 0;
        cgra_prog->pe_conf[13].rf_raddr = 0;
        cgra_prog->pe_conf[13].rf_waddr = 0;

        cgra_prog->pe_conf[14].pe_id = 15;
        cgra_prog->pe_conf[14].alu_op = 0;
        cgra_prog->pe_conf[14].alu_in_a = 0;
        cgra_prog->pe_conf[14].alu_in_b = 0;
        cgra_prog->pe_conf[14].alu_out = 0;
        cgra_prog->pe_conf[14].rf_raddr = 0;
        cgra_prog->pe_conf[14].rf_waddr = 0;

        cgra_prog->pe_conf[15].pe_id = 16;
        cgra_prog->pe_conf[15].alu_op = 0;
        cgra_prog->pe_conf[15].alu_in_a = 0;
        cgra_prog->pe_conf[15].alu_in_b = 0;
        cgra_prog->pe_conf[15].alu_out = 0;
        cgra_prog->pe_conf[15].rf_raddr = 0;
        cgra_prog->pe_conf[15].rf_waddr = 0;
    }

    cgra_prog->net_conf = (net_conf_t *) malloc(sizeof(net_conf_t) * 1);
    cgra_prog->net_conf->swicth_conf = (swicth_conf_t *) malloc(sizeof(swicth_conf_t) * 24);
    cgra_prog->cgra_intial_conf.qtd_net_conf = 1;
    {
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
