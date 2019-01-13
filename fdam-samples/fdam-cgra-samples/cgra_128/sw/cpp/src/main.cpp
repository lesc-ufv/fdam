#include <main.h>

int numThreads = 8;
int dataSize = 32;

int main(int argc, char *argv[]) {


    CgraArch cgraArch(0, 128, 8, 8, 4, 0, 2);
    Scheduler scheduler;
    Cgra cgra;
    short data_in[numThreads][dataSize];
    short data_out[numThreads][dataSize];

    scheduler.setCgra(&cgraArch);
    for (int l = 0; l < numThreads; ++l) {
        scheduler.addDataFlow(Samples::loopback(l,(int)(random() % 30 + 1)), l);
    }
    scheduler.scheduling();
    cgra.loadCgraProgram(cgraArch.getCgraProgram());

    for (int i = 0; i < numThreads; ++i) {
        for (int j = 0; j < dataSize; ++j) {
            data_in[i][j] = (short)(i+1);
            data_out[i][j] = 0;
        }
    }

    for (int k = 0; k < numThreads; ++k) {
        cgra.setCgraProgramInputStreamByID(k,0,data_in[k],dataSize* sizeof(short));
        cgra.setCgraProgramOutputStreamByID(k,1,data_out[k],dataSize* sizeof(short));
    }

    cgra.syncExecute(0);
    printf("\n");
    for (int i = 0; i < numThreads; ++i) {
        printf("Thread %d data in:  [ ",i);
        for (int j = 0; j < dataSize; ++j) {
            printf("%d ",data_in[i][j]);
        }
        printf("]\n");
    }
    printf("\n");
    for (int i = 0; i < numThreads; ++i) {
        printf("Thread %d data out: [ ",i);
        for (int j = 0; j < dataSize; ++j) {
            printf("%d ",data_out[i][j]);
        }
        printf("]\n");
    }
    printf("\n");

    return 0;
}
