#include <fdam/cgra/cgra.h>
#include <chrono>
#include <common.h>

using namespace std;
using namespace std::chrono;

int branch_test(int argc, char *argv[]);

void branch_cpu(int num_thread, const short *data_in, short *data_out, int n);

void branch_cgra(int num_thread,short *data_in, short *data_out, int n);

cgra_program_t *
create_branch_cgra_program_from_file(int num_thread, const char *file_path, short *data_in, int data_in_size, short *data_out,
                                              int data_out_size);
