#include <fir_test.h>

int fir_test(int argc, char *argv[]) {


    int num_threads = 1;
    int data_num = 32;
    int fir_size = 4;
    /*
    if (argc > 1) {
        data_num = atoi(argv[1]);
    } else {
        cout << "invalid args!!!" << endl;
        cout << "usage: <input size>" << endl;
        exit(255);
    }
    */
    auto data_in = (short *) malloc(sizeof(short) * num_threads * data_num);
    auto cpu_data_out = (short *) malloc(sizeof(short) * num_threads * data_num);
    auto cgra_data_out = (short *) malloc(sizeof(short) * num_threads * data_num);
    auto const_vet = (short *) malloc(sizeof(short) * fir_size);

    for (int l = 0; l < fir_size; ++l) {
        const_vet[l] = static_cast<short>(l + 1);
    }

    for (int j = 0; j < data_num; ++j) {
        for (int i = 0; i < num_threads; i++) {
            data_in[j * num_threads + i] = static_cast<short>(j + 1);
            cpu_data_out[j * num_threads + i] = 0;
            cgra_data_out[j * num_threads + i] = 0;
        }
    }

    high_resolution_clock::time_point s;
    duration<double> diff{};
    s = high_resolution_clock::now();

    fir_cpu(num_threads, data_in, cpu_data_out, data_num, const_vet, fir_size);

    diff = high_resolution_clock::now() - s;
    double timeExecSw = diff.count();

    s = high_resolution_clock::now();

    fir_cgra(num_threads, data_in, cgra_data_out, num_threads * data_num, fir_size);

    diff = high_resolution_clock::now() - s;
    double timeExecHw = diff.count();

    int index_error = -1;
    for (int i = 0; i < data_num; ++i) {
        if (cpu_data_out[i] != cgra_data_out[i]) {
            index_error = i;
            break;
        }
    }
    printf("Time execute fir cpu: %f\n", timeExecSw);
    printf("Time execute fir cgra-harp: %f\n", timeExecHw);
    if (index_error >= 0) {
        printf("Error found at %d  expected %d found %d!\n", index_error, cpu_data_out[index_error],
               cgra_data_out[index_error]);
    } else {
        printf("Success!\n");
    }

    printf("FIR DATA IN:\n");
    for (int j = 0; j < num_threads; ++j) {
        printf("Thread: %d\n", j);
        for (int i = 0; i < data_num; i++) {
            printf("%d ", data_in[i * num_threads + j]);
        }
        printf("\n");
    }
    printf("\nFIR CPU DATA OUT:\n");
    for (int j = 0; j < num_threads; ++j) {
        printf("Thread: %d\n", j);
        for (int i = 0; i < data_num - fir_size; i++) {
            printf("%d ", cpu_data_out[i * num_threads + j]);
        }
        printf("\n");
    }
    printf("\nFIR CGRA DATA OUT:\n");
    for (int j = 0; j < num_threads; ++j) {
        printf("Thread: %d\n", j);
        for (int i = 0; i < data_num - fir_size; i++) {
            printf("%d ", cgra_data_out[i * num_threads + j]);
        }
        printf("\n");
    }

}

void fir_cpu(int num_threads, const short *data_in, short *data_out, int n, const short *const_vet, int fir_size) {
    for (int j = 0; j < num_threads; j++) {
        for (int i = 0; i < n - fir_size; ++i) {
            short temp = 0;
            for (int k = 0; k < fir_size; ++k) {
                temp += (data_in[(i + k) * num_threads + j] * const_vet[fir_size - k - 1]);
            }
            data_out[i * num_threads + j] = temp;
        }
    }
}

void fir_cgra(int num_thread, short *data_in, short *data_out, int n, int fir_size) {
    char path[20] = "";
    sprintf(path, "../cgra_programs/fir%d.cgra", fir_size);
    cgra_program_t *cgra_program = create_fir_cgra_program_from_file(num_thread,path, data_in, n, data_out, n - fir_size);
    //print_program(cgra_program);
    auto cgra = new Cgra();
    cgra->prepareProgram(cgra_program);
    cgra->syncExecute(0);
    delete cgra;
}

cgra_program_t *
create_fir_cgra_program_from_file(int num_thread, const char *file_path, short *data_in, int data_in_size, short *data_out,
                                  int data_out_size) {
    auto *cgra_prog = (cgra_program_t *) malloc(sizeof(cgra_program_t));
    FILE *fp = fopen(file_path, "rb");
    if (fp != nullptr) {
        fread(&cgra_prog->cgra_id, sizeof(short), 1, fp);
        fread(&cgra_prog->num_pe, sizeof(short), 1, fp);
        fread(&cgra_prog->num_pe_io_in, sizeof(short), 1, fp);
        fread(&cgra_prog->num_pe_io_out, sizeof(short), 1, fp);
        fread(&cgra_prog->cgra_intial_conf, sizeof(int), 16, fp);
        size_t len = cgra_prog->cgra_intial_conf.qtd_conf * sizeof(initial_conf_t);
        cgra_prog->initial_conf = (initial_conf_t *) malloc(len*num_thread);
        fread(cgra_prog->initial_conf, sizeof(initial_conf_t), cgra_prog->cgra_intial_conf.qtd_conf, fp);
        fclose(fp);

        for (int k = 1; k < num_thread; ++k) {
            for (int j = 0; j < cgra_prog->cgra_intial_conf.qtd_conf; ++j) {
                cgra_prog->initial_conf[(k * cgra_prog->cgra_intial_conf.qtd_conf) +
                                        j].pad = cgra_prog->initial_conf[j].pad;
                cgra_prog->initial_conf[(k * cgra_prog->cgra_intial_conf.qtd_conf) +
                                        j].pe_instruction_conf.thread_id = (unsigned long) k;
            }
        }

        cgra_prog->cgra_intial_conf.qtd_conf = cgra_prog->cgra_intial_conf.qtd_conf * num_thread;

        auto input_queues = (queue_t *) malloc(sizeof(queue_t) * cgra_prog->num_pe_io_in);
        auto output_queues = (queue_t *) malloc(sizeof(queue_t) * cgra_prog->num_pe_io_out);

        for (int i = 0; i < cgra_prog->num_pe_io_in; ++i) {
            if (cgra_prog->cgra_intial_conf.mask_input_fifo & (1 << i)) {
                input_queues[i].data = data_in;
                input_queues[i].length = data_in_size;
            }
        }

        for (int i = 0; i < cgra_prog->num_pe_io_out; ++i) {
            if (cgra_prog->cgra_intial_conf.mask_output_fifo & (1 << i)) {
                output_queues[i].data = data_out;
                output_queues[i].length = data_out_size;
            }
        }

        cgra_prog->input_queues = input_queues;
        cgra_prog->output_queues = output_queues;

        return cgra_prog;

    } else {
        printf("Error opening file: %s", file_path);
        exit(1);
    }
    return nullptr;
}