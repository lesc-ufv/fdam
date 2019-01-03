#include <fdam/cgra/cgra.h>
#include <chrono>
#include <common.h>

using namespace std;
using namespace std::chrono;

int fir_test(int argc, char *argv[]);

void fir_cpu(int num_threads, const short *data_in, short *data_out, int n, const short *const_vet, int fir_size);

void fir_cgra(short *data_in, short *data_out, int n, int fir_size);

cgra_program_t *
create_fir_cgra_program_from_file(const char *file_path, short *data_in, int data_in_size, short *data_out,
                                  int data_out_size);
