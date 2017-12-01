#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <iostream>
#include <cstring>
#include <math.h>

#include "../../base/sw/opae_svc_wrapper.h"
#include "../../base/sw/csr_mgr.h"

#define MB(x) (x << 20) // the same as x*1024*1024
#define GB(x) (x << 30) // the same as x*1024*1024*1024

#define RED          31
#define GREEN        32
#define YELLOW       33
#define BLUE         34

#define MSG(x, c) cout << "\033[1;"<<c<< "m  [APP]  " << x << "\033[0m" << endl
#define PRINT_CL 0
#define TIMEOUT_EN 0
#define TIMEOUT_SEC 10

typedef enum CSR_WR {
    CFG_REG,
    ADDR_WORKSPACE_BASE,
} CSR_WR;

typedef struct CacheLine {
    char bytes[64];
} CacheLine;

typedef struct DSM {
    union {
        CacheLine cl;
        struct {
            char done:1;
        };
    };
} DSM;

typedef struct Workspace {
    DSM dsm;
    union {
        CacheLine cl[4];
        struct {
            uint64_t inputBuffer[8];
            uint64_t numDataIn[8];
            uint64_t outputBuffer[8];
            uint64_t numDataOut[8];
        };
    };
} Workspace;

using namespace std;

class AFU {

public:
    static const bool INPUT_BUFFER = true;
    static const bool OUTPUT_BUFFER = false;
    static const char AFU_START = 1;

    AFU(const char *accel_uuid);

    ~AFU();

    void *addInputBuffer(int bufferId, size_t nBytes, void *dataToCopy = NULL);

    void *addOutputBuffer(int bufferId, size_t nBytes);

    void printBuffer(int bufferId, bool isInput);

    void run();

    void reset();

    void printStatics();

private:
    OPAE_SVC_WRAPPER *fpga;
    CSR_MGR *csrs;
    Workspace *wkp;
    int totalBytesBuffers;

};