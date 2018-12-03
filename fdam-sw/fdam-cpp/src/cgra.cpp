
#include <fdam/cgra/cgra.h>

Cgra::Cgra() {
    Cgra::cgra_program = nullptr;
    Cgra::accManagement = new AccManagement();
}

Cgra::~Cgra() {
    Cgra::cgra_program = nullptr;
    delete Cgra::accManagement;
}

void Cgra::prepareProgram(cgra_program_t *program) {

    Cgra::cgra_program = program;
    Accelerator &acc = Cgra::accManagement->getAccelerator(Cgra::cgra_program->cgra_id);

    auto mask_in_queue = Cgra::cgra_program->cgra_intial_conf.read_fifo_mask;
    auto mask_out_queue = Cgra::cgra_program->cgra_intial_conf.write_fifo_mask;

    long long int num_bytes = 0;
    if (mask_in_queue & 1) {
        num_bytes = Cgra::cgra_program->input_queues[0].length * sizeof(unsigned short);
    }

    size_t cgra_intial_conf_bytes = sizeof(cgra_intial_conf_t);
    size_t conf_bytes = Cgra::cgra_program->cgra_intial_conf.qtd_conf * sizeof(initial_conf_t);

    auto cgra_intial_conf_bytes_align = static_cast<long long int>((std::ceil(cgra_intial_conf_bytes / 64.0)) * 64.0);
    auto conf_bytes_align = static_cast<long long int>((std::ceil(conf_bytes / 64.0)) * 64.0);

    long long int total_bytes;
    total_bytes = cgra_intial_conf_bytes_align;
    total_bytes += conf_bytes_align;
    total_bytes += num_bytes;

    acc.createInputQueue(static_cast<unsigned char>(0), total_bytes);
    auto queue_data_ptr = (unsigned char *) acc.getInputQueue(0);

    for (int i = 1; i < Cgra::cgra_program->cgra_intial_conf.qtd_conf; ++i) {
        if (Cgra::cgra_program->initial_conf[i].pe_store_ignore_conf.conf_type == CGRA_CONF_SET_PE_STORE_IGNORE) {
            auto store_ignore =
                    (Cgra::cgra_program->initial_conf[i].pe_store_ignore_conf.store_ignore *
                     (Cgra::cgra_program->net_stagies + 1 + 8)) + 1;
            Cgra::cgra_program->initial_conf[i].pe_store_ignore_conf.store_ignore = store_ignore;
        }
    }

    memcpy(queue_data_ptr, &Cgra::cgra_program->cgra_intial_conf, cgra_intial_conf_bytes);
    queue_data_ptr = queue_data_ptr + cgra_intial_conf_bytes_align;
    memcpy(queue_data_ptr, Cgra::cgra_program->initial_conf, conf_bytes);
    queue_data_ptr = queue_data_ptr + conf_bytes_align;

    if (mask_in_queue & 1) {
        memcpy(queue_data_ptr, Cgra::cgra_program->input_queues[0].data, static_cast<size_t>(num_bytes));
    }

    for (int i = 1; i < Cgra::cgra_program->num_pe_io; ++i) {
        if (mask_in_queue & (1 << i)) {
            num_bytes = Cgra::cgra_program->input_queues[i].length * sizeof(short);
            auto id_queue = static_cast<unsigned char>(i);
            acc.createInputQueue(id_queue, num_bytes);
            acc.copyToInputQueue(id_queue, Cgra::cgra_program->input_queues[i].data,
                                 Cgra::cgra_program->input_queues[i].length);
        }
    }
    for (int i = 0; i < Cgra::cgra_program->num_pe_io; ++i) {
        if (mask_out_queue & (1 << i)) {
            auto id_queue = static_cast<unsigned char>(i);
            num_bytes = Cgra::cgra_program->output_queues[i].length * sizeof(short);
            acc.createOutputQueue(id_queue, num_bytes);
        }
    }
}

void Cgra::syncExecute(long waitTime) {
    Accelerator &acc = Cgra::accManagement->getAccelerator((Cgra::cgra_program->cgra_id));
    acc.start();
    acc.waitDone(waitTime);
    for (int i = 0; i < Cgra::cgra_program->num_pe_io; ++i) {
        if (Cgra::cgra_program->cgra_intial_conf.write_fifo_mask & (1 << i)) {
            auto id_queue = static_cast<unsigned char>(i);
            acc.copyFromOutputQueue(id_queue, Cgra::cgra_program->output_queues[i].data,
                                    Cgra::cgra_program->output_queues[i].length);
        }
    }
}

void Cgra::asyncExecute() {
    Accelerator &acc = Cgra::accManagement->getAccelerator((Cgra::cgra_program->cgra_id));
    acc.start();
}

void Cgra::waitExecute(long waitTime) {
    Accelerator &acc = Cgra::accManagement->getAccelerator((Cgra::cgra_program->cgra_id));
    acc.waitDone(waitTime);
    for (int i = 0; i < Cgra::cgra_program->num_pe_io; ++i) {
        if (Cgra::cgra_program->cgra_intial_conf.write_fifo_mask & (1 << i)) {
            auto id_queue = static_cast<unsigned char>(i);
            acc.copyFromOutputQueue(id_queue, Cgra::cgra_program->output_queues[i].data,
                                    Cgra::cgra_program->output_queues[i].length);
        }
    }
}

int Cgra::intlog(double x, double base) {
    return (int) std::ceil(log(x) / log(base));
}

