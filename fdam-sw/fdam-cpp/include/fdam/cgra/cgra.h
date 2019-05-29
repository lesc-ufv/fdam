#ifndef CGRA_H
#define CGRA_H

#include <chrono>
#include <cstdint>
#include <cstring>
#include <cmath>
#include <fstream>
#include <fdam/cgra/cgra_defs.h>
#include <fdam/acc/acc_management.h>

using namespace std::chrono;
using namespace std;

class Cgra {

private:
    double timeExecCgra;
    // key = Input ID, Value = key = ThreadID value =  ( Pointer Data, Size Data)
    std::map<int, std::map<int, std::pair<unsigned char *, size_t>>> input_queue;
    // key = Output ID, Value = key = ThreadID value =  ( Pointer Data, Size Data)
    std::map<int, std::map<int, std::pair<unsigned char *, size_t >>> output_queue;

    cgra_program_t *cgra_program;

    AccManagement *accManagement;

    int intlog(double x, double base);

    bool readProgramFile(std::string filePath);

    std::map<int, int> makeListPe(int num_pe, int num_pe_in, int num_pe_out);

    cgra_program_t *makeCopy(cgra_program_t cp);

    void freeProgram(cgra_program_t *cp);

public:
    explicit Cgra();

    ~Cgra();

    bool loadCgraProgram(std::string filePath);

    bool loadCgraProgram(cgra_program_t cp);

    bool setCgraProgramInputStreamByID(int dataFlowID, int inputStreamID, const void *inputStreamData, size_t size);

    bool setCgraProgramInputStreamByName(std::string dataFlowName, int inputStreamID, const void *inputStreamData,
                                         size_t size);

    bool setCgraProgramOutputStreamByID(int dataFlowID, int outputStreamID, void *outputStreamData, size_t size);

    bool setCgraProgramOutputStreamByName(std::string dataFlowName, int outputStreamID, const void *outputStreamData,
                                          size_t size);

    void syncExecute(long waitTime);

    cgra_program_t *getCgraProgram();

    AccManagement *getAccManagement();

    double getTimeExec();
};

#endif //CGRA_H