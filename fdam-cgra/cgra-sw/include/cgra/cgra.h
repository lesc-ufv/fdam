#ifndef CGRA_H
#define CGRA_H

#include <cstdint>
#include <cstring>
#include <cmath>
#include <cgra/cgra_defs.h>
#include <fdam/AccManagement.h>

using namespace std;

class Cgra{

private:
    cgra_program_t *cgra_program;
    AccManagement *accManagement;

    unsigned int num_pe;
    unsigned int num_pe_io;
    unsigned int net_radix;
    unsigned int net_conf_bytes;

private:
    int intlog(double x,double base);

public:
    explicit Cgra(unsigned num_pe, unsigned int num_pe_io, unsigned int net_radix);

    ~Cgra();

    void prepareProgram(cgra_program_t *program);

    void syncExecute(long waitTime);

    void asyncExecute();

    void waitExecute(long waitTime);

};


#endif //CGRA_H
