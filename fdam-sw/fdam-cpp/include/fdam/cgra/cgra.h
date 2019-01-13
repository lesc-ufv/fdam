#ifndef CGRA_H
#define CGRA_H

#include <cstdint>
#include <cstring>
#include <cmath>
#include <fstream>
#include <fdam/cgra/cgra_defs.h>
#include <fdam/acc/acc_management.h>

using namespace std;

class Cgra {

private:
    // key = Input ID, Value = key = ThreadID value =  ( Pointer Data, Size Data)
    std::map<int, std::map<int, std::pair<unsigned char *, size_t>>> input_queue;
    // key = Output ID, Value = key = ThreadID value =  ( Pointer Data, Size Data)
    std::map<int, std::map<int,std::pair<unsigned char *, size_t >>> output_queue;

    cgra_program_t cgra_program;

    AccManagement *accManagement;

    int intlog(double x, double base);

    bool readProgramFile(std::string filePath);

public:
    explicit Cgra();

    ~Cgra();

    bool loadCgraProgramFromFile(std::string filePath);

    bool loadCgraProgram(cgra_program_t cp);

    bool setCgraProgramInputStreamByID(int dataFlowID, int inputStreamID, const void *inputStreamData, size_t size);

    bool setCgraProgramInputStreamByName(std::string dataFlowName, int inputStreamID, const void *inputStreamData,
                                         size_t size);

    bool setCgraProgramOutputStreamByID(int dataFlowID, int outputStreamID, void *outputStreamData, size_t size);

    bool setCgraProgramOutputStreamByName(std::string dataFlowName, int outputStreamID, const void *outputStreamData,
                                          size_t size);

    void syncExecute(long waitTime);
};

#endif //CGRA_H
