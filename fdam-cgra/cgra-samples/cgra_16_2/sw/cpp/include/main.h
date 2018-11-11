#include <cgra/cgra.h>

using namespace std;

int main(int argc, char *argv[]);

void fir4_cpu(const short *data_in, short *data_out, int n, const short *const_vet);

void fir4_cgra(short *data_in, short *data_out, int n, const short *const_vet);

cgra_program_t *create_fir4_cgra_program(short *data_in, int data_in_size, short *data_out, int data_out_size,
                                         const short *const_vet);