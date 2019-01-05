#include <fdam/cgra/cgra.h>
#include <chrono>
#include <common.h>

using namespace std;
using namespace std::chrono;

int loopback_test(int argc, char *argv[]);

void loopback_cpu(int num_threads, const short *data_in, short *data_out, int n);

void loopback_cgra(short *data_in, short *data_out, int n);

cgra_program_t *
create_loopback_cgra_program_from_file(const char *file_path, short *data_in, int data_in_size, short *data_out,
                                  int data_out_size);
