#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fdam/AccManagement.h>
#include <md5.h>
#include <chrono>

// #define NI 20000000*CL(1)/sizeof(uint32_t) // number of itens
// #define NJ 10000000*CL(1)/sizeof(uint32_t) // number of itens

#define NI 2000*CL(1)/sizeof(uint32_t) // number of itens
#define NJ NI/4 // number of itens

using namespace std;
using namespace std::chrono;

int main(int argc, char *argv[]);

void md5_sw(const WORD32 *message, long len_message, WORD32 *output,long len_output);
void md5_hw(const WORD32 *message, long len_message, WORD32 *output,long len_output);

