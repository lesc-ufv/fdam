#include <main.h>

int main(int argc, char *argv[]) {

    int data_num = 1024;
    if (argc > 1) {
        data_num = atoi(argv[1]);
    } else {
        cout << "invalid args!!!" << endl;
        cout << "usage: <input size>" << endl;
        exit(255);
    }
    auto data_in = (short *) malloc(sizeof(short) * data_num);
    auto cpu_data_out = (short *) malloc(sizeof(short) * data_num);
    auto cgra_data_out = (short *) malloc(sizeof(short) * data_num);
    short const_vet[4] = {1, 2, 3, 4};

    for (int i = 0; i < data_num; ++i) {
        data_in[i] = static_cast<short>(i + 1);
        cpu_data_out[i] = 0;
        cgra_data_out[i] = 0;
    }

    high_resolution_clock::time_point s;
    duration<double> diff{};
    s = high_resolution_clock::now();

    fir4_cpu(data_in, cpu_data_out, data_num, const_vet);

    diff = high_resolution_clock::now() - s;
    double timeExecSw = diff.count();

    s = high_resolution_clock::now();

    //fir4_cgra(data_in, cgra_data_out, data_num, const_vet);
    fir4_cgra_from_file(data_in, cgra_data_out, data_num);

    diff = high_resolution_clock::now() - s;
    double timeExecHw = diff.count();

    int index_error = -1;
    for (int i = 0; i < data_num; ++i) {
        if (cpu_data_out[i] != cgra_data_out[i]) {
            index_error = i;
            break;
        }
    }
    printf("Time execute fir4 cpu: %f\n", timeExecSw);
    printf("Time execute fir4 cgra-harp: %f\n", timeExecHw);
    if (index_error >= 0) {
        printf("Error found at %d  expected %d found %d!\n", index_error, cpu_data_out[index_error],
               cgra_data_out[index_error]);
    } else {
        printf("Success!\n");
    }

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
    auto cgra = new Cgra();
    cgra->prepareProgram(cgra_program);
    cgra->syncExecute(0);
    delete cgra;
}

void fir4_cgra_from_file(short *data_in, short *data_out, int n) {
    cgra_program_t *cgra_program = create_fir4_cgra_program_from_file("../src/fir4.cgra", data_in, n, data_out, n - 4);
    auto cgra = new Cgra();
    cgra->prepareProgram(cgra_program);
    cgra->syncExecute(0);
    cgra_program->cgra_intial_conf.qtd_conf = 0;
    cgra->prepareProgram(cgra_program);
    cgra->syncExecute(0);
    delete cgra;
}

void print_program(cgra_program_t *cgra_program) {

    printf("CGAR ID: %d\n", cgra_program->cgra_id);
    printf("CGAR NUM PE: %d\n", cgra_program->num_pe);
    printf("CGAR IO: %d\n", cgra_program->num_pe_io);
    printf("CGAR NET STAGIES: %d\n", cgra_program->net_stagies);
    printf("QUANTIDADE CONF: %d\n", cgra_program->cgra_intial_conf.qtd_conf);
    printf("FIFO READ MASK: %x\n", cgra_program->cgra_intial_conf.read_fifo_mask);
    printf("FIFO WRITE MASK: %x\n", cgra_program->cgra_intial_conf.write_fifo_mask);
    for (int i = 0; i < cgra_program->cgra_intial_conf.qtd_conf; ++i) {
        switch (cgra_program->initial_conf[i].pe_instruction_conf.conf_type) {
            case CGRA_CONF_SET_PE_INSTRUCTION:
                printf("CGRA_CONF_SET_PE_INSTRUCTION: PE_ID: %lu, INST_ADDR: %lu, ALU_OP: %lu, ALU_INA: %lu, ALU_INB: %lu,"
                               " ALU_OUT: %lu, RF_RADDR: %lu RF_WADDR: %lu \n",
                       cgra_program->initial_conf[i].pe_instruction_conf.pe_id,
                       cgra_program->initial_conf[i].pe_instruction_conf.inst_addr,
                       cgra_program->initial_conf[i].pe_instruction_conf.alu_op,
                       cgra_program->initial_conf[i].pe_instruction_conf.alu_in_a,
                       cgra_program->initial_conf[i].pe_instruction_conf.alu_in_b,
                       cgra_program->initial_conf[i].pe_instruction_conf.alu_out,
                       cgra_program->initial_conf[i].pe_instruction_conf.rf_raddr,
                       cgra_program->initial_conf[i].pe_instruction_conf.rf_waddr);
                break;
            case CGRA_CONF_SET_PE_CONST:
                printf("CGRA_CONF_SET_PE_CONST: PE_ID: %lu, CONST_ADDR: %lu, CONST: %lu\n",
                       cgra_program->initial_conf[i].pe_constant_conf.pe_id,
                       cgra_program->initial_conf[i].pe_constant_conf.const_addr,
                       cgra_program->initial_conf[i].pe_constant_conf.constant);
                break;
            case CGRA_CONF_SET_PE_PC_MAX:
                printf("CGRA_CONF_SET_PE_PC_MAX: PE_ID: %lu, PC_MAX: %lu\n",
                       cgra_program->initial_conf[i].pe_pc_max_conf.pe_id,
                       cgra_program->initial_conf[i].pe_pc_max_conf.pc_max);
                break;
            case CGRA_CONF_SET_PE_PC_LOOP:
                printf("CGRA_CONF_SET_PE_PC_LOOP: PE_ID: %lu, PC_LOOP: %lu\n",
                       cgra_program->initial_conf[i].pe_pc_loop_conf.pe_id,
                       cgra_program->initial_conf[i].pe_pc_loop_conf.pc_loop);
                break;
            case CGRA_CONF_SET_PE_STORE_IGNORE:
                printf("CGRA_CONF_SET_PE_STORE_IGNORE: PE_ID: %lu, STORE_IGNORE: %lu\n",
                       cgra_program->initial_conf[i].pe_store_ignore_conf.pe_id,
                       cgra_program->initial_conf[i].pe_store_ignore_conf.store_ignore);
                break;
            case CGRA_CONF_SET_NET_PC_MAX:
                printf("CGRA_CONF_SET_NET_PC_MAX: SWITCH_NUMBER: %lu, PC_MAX: %lu\n",
                       cgra_program->initial_conf[i].net_pc_max_conf.switch_number,
                       cgra_program->initial_conf[i].net_pc_max_conf.pc_max);
                break;
            case CGRA_CONF_SET_NET_PC_LOOP:
                printf("CGRA_CONF_SET_NET_PC_LOOP: SWITCH_NUMBER: %lu, PC_LOOP: %lu\n",
                       cgra_program->initial_conf[i].net_pc_loop_conf.switch_number,
                       cgra_program->initial_conf[i].net_pc_loop_conf.pc_loop);
                break;
            case CGRA_CONF_NET_SWITCH:
                printf("CGRA_CONF_NET_SWITCH: SWITCH_NUMBER: %lu, INST_ADDR: %lu, SWITCH_CONF: %08lX\n",
                       cgra_program->initial_conf[i].net_switch_conf.switch_number,
                       cgra_program->initial_conf[i].net_switch_conf.inst_addr,
                       cgra_program->initial_conf[i].net_switch_conf.switch_conf);
                break;
            default:
                break;
        }
    }
}

cgra_program_t *
create_fir4_cgra_program_from_file(const char *file_path, short *data_in, int data_in_size, short *data_out,
                                   int data_out_size) {
    auto *cgra_prog = (cgra_program_t *) malloc(sizeof(cgra_program_t));
    FILE *fp = fopen(file_path, "rb");
    if (fp != nullptr) {
        fread(&cgra_prog->cgra_id, sizeof(short), 1, fp);
        fseek(fp, sizeof(short), SEEK_SET);
        fread(&cgra_prog->num_pe, sizeof(short), 1, fp);
        fseek(fp, sizeof(short) * 2, SEEK_SET);
        fread(&cgra_prog->num_pe_io, sizeof(short), 1, fp);
        fseek(fp, sizeof(short) * 3, SEEK_SET);
        fread(&cgra_prog->net_stagies, sizeof(short), 1, fp);
        fseek(fp, sizeof(short) * 4, SEEK_SET);
        fread(&cgra_prog->cgra_intial_conf, sizeof(int), 16, fp);
        fseek(fp, 72, SEEK_SET);
        size_t len = cgra_prog->cgra_intial_conf.qtd_conf * sizeof(initial_conf_t);
        cgra_prog->initial_conf = (initial_conf_t *) malloc(len);
        fread(cgra_prog->initial_conf, sizeof(initial_conf_t), cgra_prog->cgra_intial_conf.qtd_conf, fp);
        fclose(fp);

        auto input_queues = (queue_t *) malloc(sizeof(queue_t) * cgra_prog->num_pe_io);
        auto output_queues = (queue_t *) malloc(sizeof(queue_t) * cgra_prog->num_pe_io);

        for (int i = 0; i < cgra_prog->num_pe_io; ++i) {
            if (cgra_prog->cgra_intial_conf.read_fifo_mask & (1 << i)) {
                input_queues[i].data = data_in;
                input_queues[i].length = data_in_size;
            }
            if (cgra_prog->cgra_intial_conf.write_fifo_mask & (1 << i)) {
                output_queues[i].data = data_out;
                output_queues[i].length = data_out_size;
            }
        }

        cgra_prog->input_queues = input_queues;
        cgra_prog->output_queues = output_queues;

        return cgra_prog;

    } else {
        printf("Error opening file: %s", file_path);
    }
    return nullptr;
}

cgra_program_t *create_fir4_cgra_program(short *data_in, int data_in_size, short *data_out, int data_out_size,
                                         const short *const_vet) {

    auto *cgra_prog = (cgra_program_t *) malloc(sizeof(cgra_program_t));

    cgra_prog->cgra_id = 0;
    memset(&cgra_prog->cgra_intial_conf, 0, sizeof(cgra_intial_conf_t));
    cgra_prog->cgra_intial_conf.read_fifo_mask = 0x1;
    cgra_prog->cgra_intial_conf.write_fifo_mask = 0x2;

    {
        cgra_prog->cgra_intial_conf.qtd_conf = 45;
        size_t len = 45 * sizeof(initial_conf_t);
        cgra_prog->initial_conf = (initial_conf_t *) malloc(len);

        cgra_prog->initial_conf[0].pe_instruction_conf.conf_type = CGRA_CONF_SET_PE_INSTRUCTION;
        cgra_prog->initial_conf[0].pe_instruction_conf.pe_id = 1;
        cgra_prog->initial_conf[0].pe_instruction_conf.inst_addr = 0;
        cgra_prog->initial_conf[0].pe_instruction_conf.alu_op = 0;
        cgra_prog->initial_conf[0].pe_instruction_conf.alu_in_a = 3;
        cgra_prog->initial_conf[0].pe_instruction_conf.alu_in_b = 0;
        cgra_prog->initial_conf[0].pe_instruction_conf.alu_out = 0;
        cgra_prog->initial_conf[0].pe_instruction_conf.rf_raddr = 0;
        cgra_prog->initial_conf[0].pe_instruction_conf.rf_waddr = 0;

        cgra_prog->initial_conf[1].pe_instruction_conf.conf_type = CGRA_CONF_SET_PE_INSTRUCTION;
        cgra_prog->initial_conf[1].pe_instruction_conf.pe_id = 2;
        cgra_prog->initial_conf[1].pe_instruction_conf.inst_addr = 0;
        cgra_prog->initial_conf[1].pe_instruction_conf.alu_op = 0;
        cgra_prog->initial_conf[1].pe_instruction_conf.alu_in_a = 0;
        cgra_prog->initial_conf[1].pe_instruction_conf.alu_in_b = 0;
        cgra_prog->initial_conf[1].pe_instruction_conf.alu_out = 3;
        cgra_prog->initial_conf[1].pe_instruction_conf.rf_raddr = 0;
        cgra_prog->initial_conf[1].pe_instruction_conf.rf_waddr = 0;

        cgra_prog->initial_conf[2].pe_instruction_conf.conf_type = CGRA_CONF_SET_PE_INSTRUCTION;
        cgra_prog->initial_conf[2].pe_instruction_conf.pe_id = 3;
        cgra_prog->initial_conf[2].pe_instruction_conf.inst_addr = 0;
        cgra_prog->initial_conf[2].pe_instruction_conf.alu_op = 4;
        cgra_prog->initial_conf[2].pe_instruction_conf.alu_in_a = 0;
        cgra_prog->initial_conf[2].pe_instruction_conf.alu_in_b = 2;
        cgra_prog->initial_conf[2].pe_instruction_conf.alu_out = 0;
        cgra_prog->initial_conf[2].pe_instruction_conf.rf_raddr = 0;
        cgra_prog->initial_conf[2].pe_instruction_conf.rf_waddr = 0;

        cgra_prog->initial_conf[3].pe_instruction_conf.conf_type = CGRA_CONF_SET_PE_INSTRUCTION;
        cgra_prog->initial_conf[3].pe_instruction_conf.pe_id = 4;
        cgra_prog->initial_conf[3].pe_instruction_conf.inst_addr = 0;
        cgra_prog->initial_conf[3].pe_instruction_conf.alu_op = 0;
        cgra_prog->initial_conf[3].pe_instruction_conf.alu_in_a = 0;
        cgra_prog->initial_conf[3].pe_instruction_conf.alu_in_b = 0;
        cgra_prog->initial_conf[3].pe_instruction_conf.alu_out = 0;
        cgra_prog->initial_conf[3].pe_instruction_conf.rf_raddr = 0;
        cgra_prog->initial_conf[3].pe_instruction_conf.rf_waddr = 0;

        cgra_prog->initial_conf[4].pe_instruction_conf.conf_type = CGRA_CONF_SET_PE_INSTRUCTION;
        cgra_prog->initial_conf[4].pe_instruction_conf.pe_id = 5;
        cgra_prog->initial_conf[4].pe_instruction_conf.inst_addr = 0;
        cgra_prog->initial_conf[4].pe_instruction_conf.alu_op = 0;
        cgra_prog->initial_conf[4].pe_instruction_conf.alu_in_a = 0;
        cgra_prog->initial_conf[4].pe_instruction_conf.alu_in_b = 0;
        cgra_prog->initial_conf[4].pe_instruction_conf.alu_out = 1;
        cgra_prog->initial_conf[4].pe_instruction_conf.rf_raddr = 0;
        cgra_prog->initial_conf[4].pe_instruction_conf.rf_waddr = 0;

        cgra_prog->initial_conf[5].pe_instruction_conf.conf_type = CGRA_CONF_SET_PE_INSTRUCTION;
        cgra_prog->initial_conf[5].pe_instruction_conf.pe_id = 6;
        cgra_prog->initial_conf[5].pe_instruction_conf.inst_addr = 0;
        cgra_prog->initial_conf[5].pe_instruction_conf.alu_op = 4;
        cgra_prog->initial_conf[5].pe_instruction_conf.alu_in_a = 0;
        cgra_prog->initial_conf[5].pe_instruction_conf.alu_in_b = 2;
        cgra_prog->initial_conf[5].pe_instruction_conf.alu_out = 0;
        cgra_prog->initial_conf[5].pe_instruction_conf.rf_raddr = 0;
        cgra_prog->initial_conf[5].pe_instruction_conf.rf_waddr = 0;

        cgra_prog->initial_conf[6].pe_instruction_conf.conf_type = CGRA_CONF_SET_PE_INSTRUCTION;
        cgra_prog->initial_conf[6].pe_instruction_conf.pe_id = 7;
        cgra_prog->initial_conf[6].pe_instruction_conf.inst_addr = 0;
        cgra_prog->initial_conf[6].pe_instruction_conf.alu_op = 0;
        cgra_prog->initial_conf[6].pe_instruction_conf.alu_in_a = 0;
        cgra_prog->initial_conf[6].pe_instruction_conf.alu_in_b = 0;
        cgra_prog->initial_conf[6].pe_instruction_conf.alu_out = 0;
        cgra_prog->initial_conf[6].pe_instruction_conf.rf_raddr = 0;
        cgra_prog->initial_conf[6].pe_instruction_conf.rf_waddr = 0;

        cgra_prog->initial_conf[7].pe_instruction_conf.conf_type = CGRA_CONF_SET_PE_INSTRUCTION;
        cgra_prog->initial_conf[7].pe_instruction_conf.pe_id = 8;
        cgra_prog->initial_conf[7].pe_instruction_conf.inst_addr = 0;
        cgra_prog->initial_conf[7].pe_instruction_conf.alu_op = 2;
        cgra_prog->initial_conf[7].pe_instruction_conf.alu_in_a = 0;
        cgra_prog->initial_conf[7].pe_instruction_conf.alu_in_b = 1;
        cgra_prog->initial_conf[7].pe_instruction_conf.alu_out = 0;
        cgra_prog->initial_conf[7].pe_instruction_conf.rf_raddr = 0;
        cgra_prog->initial_conf[7].pe_instruction_conf.rf_waddr = 0;

        cgra_prog->initial_conf[8].pe_instruction_conf.conf_type = CGRA_CONF_SET_PE_INSTRUCTION;
        cgra_prog->initial_conf[8].pe_instruction_conf.pe_id = 9;
        cgra_prog->initial_conf[8].pe_instruction_conf.inst_addr = 0;
        cgra_prog->initial_conf[8].pe_instruction_conf.alu_op = 4;
        cgra_prog->initial_conf[8].pe_instruction_conf.alu_in_a = 0;
        cgra_prog->initial_conf[8].pe_instruction_conf.alu_in_b = 2;
        cgra_prog->initial_conf[8].pe_instruction_conf.alu_out = 0;
        cgra_prog->initial_conf[8].pe_instruction_conf.rf_raddr = 0;
        cgra_prog->initial_conf[8].pe_instruction_conf.rf_waddr = 0;

        cgra_prog->initial_conf[9].pe_instruction_conf.conf_type = CGRA_CONF_SET_PE_INSTRUCTION;
        cgra_prog->initial_conf[9].pe_instruction_conf.pe_id = 10;
        cgra_prog->initial_conf[9].pe_instruction_conf.inst_addr = 0;
        cgra_prog->initial_conf[9].pe_instruction_conf.alu_op = 0;
        cgra_prog->initial_conf[9].pe_instruction_conf.alu_in_a = 0;
        cgra_prog->initial_conf[9].pe_instruction_conf.alu_in_b = 0;
        cgra_prog->initial_conf[9].pe_instruction_conf.alu_out = 1;
        cgra_prog->initial_conf[9].pe_instruction_conf.rf_raddr = 0;
        cgra_prog->initial_conf[9].pe_instruction_conf.rf_waddr = 0;

        cgra_prog->initial_conf[10].pe_instruction_conf.conf_type = CGRA_CONF_SET_PE_INSTRUCTION;
        cgra_prog->initial_conf[10].pe_instruction_conf.pe_id = 11;
        cgra_prog->initial_conf[10].pe_instruction_conf.inst_addr = 0;
        cgra_prog->initial_conf[10].pe_instruction_conf.alu_op = 2;
        cgra_prog->initial_conf[10].pe_instruction_conf.alu_in_a = 0;
        cgra_prog->initial_conf[10].pe_instruction_conf.alu_in_b = 1;
        cgra_prog->initial_conf[10].pe_instruction_conf.alu_out = 0;
        cgra_prog->initial_conf[10].pe_instruction_conf.rf_raddr = 0;
        cgra_prog->initial_conf[10].pe_instruction_conf.rf_waddr = 0;

        cgra_prog->initial_conf[11].pe_instruction_conf.conf_type = CGRA_CONF_SET_PE_INSTRUCTION;
        cgra_prog->initial_conf[11].pe_instruction_conf.pe_id = 12;
        cgra_prog->initial_conf[11].pe_instruction_conf.inst_addr = 0;
        cgra_prog->initial_conf[11].pe_instruction_conf.alu_op = 4;
        cgra_prog->initial_conf[11].pe_instruction_conf.alu_in_a = 0;
        cgra_prog->initial_conf[11].pe_instruction_conf.alu_in_b = 2;
        cgra_prog->initial_conf[11].pe_instruction_conf.alu_out = 1;
        cgra_prog->initial_conf[11].pe_instruction_conf.rf_raddr = 0;
        cgra_prog->initial_conf[11].pe_instruction_conf.rf_waddr = 0;

        cgra_prog->initial_conf[12].pe_instruction_conf.conf_type = CGRA_CONF_SET_PE_INSTRUCTION;
        cgra_prog->initial_conf[12].pe_instruction_conf.pe_id = 13;
        cgra_prog->initial_conf[12].pe_instruction_conf.inst_addr = 0;
        cgra_prog->initial_conf[12].pe_instruction_conf.alu_op = 2;
        cgra_prog->initial_conf[12].pe_instruction_conf.alu_in_a = 0;
        cgra_prog->initial_conf[12].pe_instruction_conf.alu_in_b = 1;
        cgra_prog->initial_conf[12].pe_instruction_conf.alu_out = 0;
        cgra_prog->initial_conf[12].pe_instruction_conf.rf_raddr = 0;
        cgra_prog->initial_conf[12].pe_instruction_conf.rf_waddr = 0;

        cgra_prog->initial_conf[13].pe_instruction_conf.conf_type = CGRA_CONF_SET_PE_INSTRUCTION;
        cgra_prog->initial_conf[13].pe_instruction_conf.pe_id = 14;
        cgra_prog->initial_conf[13].pe_instruction_conf.inst_addr = 0;
        cgra_prog->initial_conf[13].pe_instruction_conf.alu_op = 0;
        cgra_prog->initial_conf[13].pe_instruction_conf.alu_in_a = 0;
        cgra_prog->initial_conf[13].pe_instruction_conf.alu_in_b = 0;
        cgra_prog->initial_conf[13].pe_instruction_conf.alu_out = 0;
        cgra_prog->initial_conf[13].pe_instruction_conf.rf_raddr = 0;
        cgra_prog->initial_conf[13].pe_instruction_conf.rf_waddr = 0;

        cgra_prog->initial_conf[14].pe_instruction_conf.conf_type = CGRA_CONF_SET_PE_INSTRUCTION;
        cgra_prog->initial_conf[14].pe_instruction_conf.pe_id = 15;
        cgra_prog->initial_conf[14].pe_instruction_conf.inst_addr = 0;
        cgra_prog->initial_conf[14].pe_instruction_conf.alu_op = 0;
        cgra_prog->initial_conf[14].pe_instruction_conf.alu_in_a = 0;
        cgra_prog->initial_conf[14].pe_instruction_conf.alu_in_b = 0;
        cgra_prog->initial_conf[14].pe_instruction_conf.alu_out = 0;
        cgra_prog->initial_conf[14].pe_instruction_conf.rf_raddr = 0;
        cgra_prog->initial_conf[14].pe_instruction_conf.rf_waddr = 0;

        cgra_prog->initial_conf[15].pe_instruction_conf.conf_type = CGRA_CONF_SET_PE_INSTRUCTION;
        cgra_prog->initial_conf[15].pe_instruction_conf.pe_id = 16;
        cgra_prog->initial_conf[15].pe_instruction_conf.inst_addr = 0;
        cgra_prog->initial_conf[15].pe_instruction_conf.alu_op = 0;
        cgra_prog->initial_conf[15].pe_instruction_conf.alu_in_a = 0;
        cgra_prog->initial_conf[15].pe_instruction_conf.alu_in_b = 0;
        cgra_prog->initial_conf[15].pe_instruction_conf.alu_out = 0;
        cgra_prog->initial_conf[15].pe_instruction_conf.rf_raddr = 0;
        cgra_prog->initial_conf[15].pe_instruction_conf.rf_waddr = 0;

        cgra_prog->initial_conf[16].pe_store_ignore_conf.conf_type = CGRA_CONF_SET_PE_STORE_IGNORE;
        cgra_prog->initial_conf[16].pe_store_ignore_conf.pe_id = 2;
        cgra_prog->initial_conf[16].pe_store_ignore_conf.store_ignore = 6;

        cgra_prog->initial_conf[17].pe_constant_conf.conf_type = CGRA_CONF_SET_PE_CONST;
        cgra_prog->initial_conf[17].pe_constant_conf.pe_id = 3;
        cgra_prog->initial_conf[17].pe_constant_conf.const_addr = 0;
        cgra_prog->initial_conf[17].pe_constant_conf.constant = (unsigned int) const_vet[3];

        cgra_prog->initial_conf[18].pe_constant_conf.conf_type = CGRA_CONF_SET_PE_CONST;
        cgra_prog->initial_conf[18].pe_constant_conf.pe_id = 6;
        cgra_prog->initial_conf[18].pe_constant_conf.const_addr = 0;
        cgra_prog->initial_conf[18].pe_constant_conf.constant = (unsigned int) const_vet[2];

        cgra_prog->initial_conf[19].pe_constant_conf.conf_type = CGRA_CONF_SET_PE_CONST;
        cgra_prog->initial_conf[19].pe_constant_conf.pe_id = 9;
        cgra_prog->initial_conf[19].pe_constant_conf.const_addr = 0;
        cgra_prog->initial_conf[19].pe_constant_conf.constant = (unsigned int) const_vet[1];

        cgra_prog->initial_conf[20].pe_constant_conf.conf_type = CGRA_CONF_SET_PE_CONST;
        cgra_prog->initial_conf[20].pe_constant_conf.pe_id = 12;
        cgra_prog->initial_conf[20].pe_constant_conf.const_addr = 0;
        cgra_prog->initial_conf[20].pe_constant_conf.constant = (unsigned int) const_vet[0];

        // stage 1:
        cgra_prog->initial_conf[21].net_switch_conf.conf_type = CGRA_CONF_NET_SWITCH;
        cgra_prog->initial_conf[21].net_switch_conf.switch_number = 1;
        cgra_prog->initial_conf[21].net_switch_conf.inst_addr = 0;
        cgra_prog->initial_conf[21].net_switch_conf.switch_conf = switch_conf_radix4(0, 0, 3, 2);

        cgra_prog->initial_conf[22].net_switch_conf.conf_type = CGRA_CONF_NET_SWITCH;
        cgra_prog->initial_conf[22].net_switch_conf.switch_number = 2;
        cgra_prog->initial_conf[22].net_switch_conf.inst_addr = 0;
        cgra_prog->initial_conf[22].net_switch_conf.switch_conf = switch_conf_radix4(1, 0, 0, 0);

        cgra_prog->initial_conf[23].net_switch_conf.conf_type = CGRA_CONF_NET_SWITCH;
        cgra_prog->initial_conf[23].net_switch_conf.switch_number = 3;
        cgra_prog->initial_conf[23].net_switch_conf.inst_addr = 0;
        cgra_prog->initial_conf[23].net_switch_conf.switch_conf = switch_conf_radix4(1, 0, 0, 0);

        cgra_prog->initial_conf[24].net_switch_conf.conf_type = CGRA_CONF_NET_SWITCH;
        cgra_prog->initial_conf[24].net_switch_conf.switch_number = 4;
        cgra_prog->initial_conf[24].net_switch_conf.inst_addr = 0;
        cgra_prog->initial_conf[24].net_switch_conf.switch_conf = switch_conf_radix4(0, 0, 0, 2);

        cgra_prog->initial_conf[25].net_switch_conf.conf_type = CGRA_CONF_NET_SWITCH;
        cgra_prog->initial_conf[25].net_switch_conf.switch_number = 5;
        cgra_prog->initial_conf[25].net_switch_conf.inst_addr = 0;
        cgra_prog->initial_conf[25].net_switch_conf.switch_conf = switch_conf_radix4(0, 1, 0, 2);

        cgra_prog->initial_conf[26].net_switch_conf.conf_type = CGRA_CONF_NET_SWITCH;
        cgra_prog->initial_conf[26].net_switch_conf.switch_number = 6;
        cgra_prog->initial_conf[26].net_switch_conf.inst_addr = 0;
        cgra_prog->initial_conf[26].net_switch_conf.switch_conf = switch_conf_radix4(0, 0, 0, 0);

        cgra_prog->initial_conf[27].net_switch_conf.conf_type = CGRA_CONF_NET_SWITCH;
        cgra_prog->initial_conf[27].net_switch_conf.switch_number = 7;
        cgra_prog->initial_conf[27].net_switch_conf.inst_addr = 0;
        cgra_prog->initial_conf[27].net_switch_conf.switch_conf = switch_conf_radix4(0, 1, 0, 0);

        cgra_prog->initial_conf[28].net_switch_conf.conf_type = CGRA_CONF_NET_SWITCH;
        cgra_prog->initial_conf[28].net_switch_conf.switch_number = 8;
        cgra_prog->initial_conf[28].net_switch_conf.inst_addr = 0;
        cgra_prog->initial_conf[28].net_switch_conf.switch_conf = switch_conf_radix4(0, 2, 0, 0);

        // stage 2:
        cgra_prog->initial_conf[29].net_switch_conf.conf_type = CGRA_CONF_NET_SWITCH;
        cgra_prog->initial_conf[29].net_switch_conf.switch_number = 9;
        cgra_prog->initial_conf[29].net_switch_conf.inst_addr = 0;
        cgra_prog->initial_conf[29].net_switch_conf.switch_conf = switch_conf_radix4(0, 0, 2, 1);

        cgra_prog->initial_conf[30].net_switch_conf.conf_type = CGRA_CONF_NET_SWITCH;
        cgra_prog->initial_conf[30].net_switch_conf.switch_number = 10;
        cgra_prog->initial_conf[30].net_switch_conf.inst_addr = 0;
        cgra_prog->initial_conf[30].net_switch_conf.switch_conf = switch_conf_radix4(2, 3, 0, 0);

        cgra_prog->initial_conf[31].net_switch_conf.conf_type = CGRA_CONF_NET_SWITCH;
        cgra_prog->initial_conf[31].net_switch_conf.switch_number = 11;
        cgra_prog->initial_conf[31].net_switch_conf.inst_addr = 0;
        cgra_prog->initial_conf[31].net_switch_conf.switch_conf = switch_conf_radix4(0, 0, 3, 3);

        cgra_prog->initial_conf[32].net_switch_conf.conf_type = CGRA_CONF_NET_SWITCH;
        cgra_prog->initial_conf[32].net_switch_conf.switch_number = 12;
        cgra_prog->initial_conf[32].net_switch_conf.inst_addr = 0;
        cgra_prog->initial_conf[32].net_switch_conf.switch_conf = switch_conf_radix4(0, 0, 2, 0);

        cgra_prog->initial_conf[33].net_switch_conf.conf_type = CGRA_CONF_NET_SWITCH;
        cgra_prog->initial_conf[33].net_switch_conf.switch_number = 13;
        cgra_prog->initial_conf[33].net_switch_conf.inst_addr = 0;
        cgra_prog->initial_conf[33].net_switch_conf.switch_conf = switch_conf_radix4(0, 0, 0, 0);

        cgra_prog->initial_conf[34].net_switch_conf.conf_type = CGRA_CONF_NET_SWITCH;
        cgra_prog->initial_conf[34].net_switch_conf.switch_number = 14;
        cgra_prog->initial_conf[34].net_switch_conf.inst_addr = 0;
        cgra_prog->initial_conf[34].net_switch_conf.switch_conf = switch_conf_radix4(0, 0, 3, 0);

        cgra_prog->initial_conf[35].net_switch_conf.conf_type = CGRA_CONF_NET_SWITCH;
        cgra_prog->initial_conf[35].net_switch_conf.switch_number = 15;
        cgra_prog->initial_conf[35].net_switch_conf.inst_addr = 0;
        cgra_prog->initial_conf[35].net_switch_conf.switch_conf = switch_conf_radix4(0, 0, 0, 2);

        cgra_prog->initial_conf[36].net_switch_conf.conf_type = CGRA_CONF_NET_SWITCH;
        cgra_prog->initial_conf[36].net_switch_conf.switch_number = 16;
        cgra_prog->initial_conf[36].net_switch_conf.inst_addr = 0;
        cgra_prog->initial_conf[36].net_switch_conf.switch_conf = switch_conf_radix4(0, 1, 0, 0);

        // stage 3:
        cgra_prog->initial_conf[37].net_switch_conf.conf_type = CGRA_CONF_NET_SWITCH;
        cgra_prog->initial_conf[37].net_switch_conf.switch_number = 17;
        cgra_prog->initial_conf[37].net_switch_conf.inst_addr = 0;
        cgra_prog->initial_conf[37].net_switch_conf.switch_conf = switch_conf_radix4(0, 0, 1, 0);

        cgra_prog->initial_conf[38].net_switch_conf.conf_type = CGRA_CONF_NET_SWITCH;
        cgra_prog->initial_conf[38].net_switch_conf.switch_number = 18;
        cgra_prog->initial_conf[38].net_switch_conf.inst_addr = 0;
        cgra_prog->initial_conf[38].net_switch_conf.switch_conf = switch_conf_radix4(0, 0, 0, 0);

        cgra_prog->initial_conf[39].net_switch_conf.conf_type = CGRA_CONF_NET_SWITCH;
        cgra_prog->initial_conf[39].net_switch_conf.switch_number = 19;
        cgra_prog->initial_conf[39].net_switch_conf.inst_addr = 0;
        cgra_prog->initial_conf[39].net_switch_conf.switch_conf = switch_conf_radix4(0, 0, 1, 0);

        cgra_prog->initial_conf[40].net_switch_conf.conf_type = CGRA_CONF_NET_SWITCH;
        cgra_prog->initial_conf[40].net_switch_conf.switch_number = 20;
        cgra_prog->initial_conf[40].net_switch_conf.inst_addr = 0;
        cgra_prog->initial_conf[40].net_switch_conf.switch_conf = switch_conf_radix4(1, 0, 2, 0);

        cgra_prog->initial_conf[41].net_switch_conf.conf_type = CGRA_CONF_NET_SWITCH;
        cgra_prog->initial_conf[41].net_switch_conf.switch_number = 21;
        cgra_prog->initial_conf[41].net_switch_conf.inst_addr = 0;
        cgra_prog->initial_conf[41].net_switch_conf.switch_conf = switch_conf_radix4(0, 0, 0, 0);

        cgra_prog->initial_conf[42].net_switch_conf.conf_type = CGRA_CONF_NET_SWITCH;
        cgra_prog->initial_conf[42].net_switch_conf.switch_number = 22;
        cgra_prog->initial_conf[42].net_switch_conf.inst_addr = 0;
        cgra_prog->initial_conf[42].net_switch_conf.switch_conf = switch_conf_radix4(0, 1, 3, 0);

        cgra_prog->initial_conf[43].net_switch_conf.conf_type = CGRA_CONF_NET_SWITCH;
        cgra_prog->initial_conf[43].net_switch_conf.switch_number = 23;
        cgra_prog->initial_conf[43].net_switch_conf.inst_addr = 0;
        cgra_prog->initial_conf[43].net_switch_conf.switch_conf = switch_conf_radix4(1, 2, 0, 0);

        cgra_prog->initial_conf[44].net_switch_conf.conf_type = CGRA_CONF_NET_SWITCH;
        cgra_prog->initial_conf[44].net_switch_conf.switch_number = 24;
        cgra_prog->initial_conf[44].net_switch_conf.inst_addr = 0;
        cgra_prog->initial_conf[44].net_switch_conf.switch_conf = switch_conf_radix4(0, 0, 0, 0);
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

