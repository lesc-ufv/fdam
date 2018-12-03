#ifndef CGRA_H
#define CGRA_H

#include <cstdint>
#include <cstring>
#include <cmath>
#include <fdam/cgra/cgra_defs.h>
#include <fdam/acc/acc_management.h>

using namespace std;

class Cgra {

private:
    cgra_program_t *cgra_program;
    AccManagement *accManagement;

private:
    int intlog(double x, double base);

public:
    explicit Cgra();

    ~Cgra();

    void prepareProgram(cgra_program_t *program);

    void syncExecute(long waitTime);

    void asyncExecute();

    void waitExecute(long waitTime);

};


#endif //CGRA_H
