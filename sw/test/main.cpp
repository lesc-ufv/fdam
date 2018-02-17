#include <iostream>
#include "../AFUManagerLIB/AFUManager.h"

using namespace std;


void testLB_afu(AFUManager &afuManager,afuid_t id);

int main(int argc, char *argv[]) {

    auto *afuManager = new AFUManager("9f81ba12-1d38-4cc7-953a-dafeef45065b");
    testLB_afu(*afuManager,0);
    afuManager->printInfoAFUManager();
    afuManager->printStatics();

    delete afuManager;
    return 0;
}
void testLB_afu(AFUManager &afuManager,afuid_t id){
    
    
    AFU *afu = afuManager.getAFU(id);
    size_t numdados = 32*50000;
    short dataIn[numdados];
    for (short i = 0; i < numdados; ++i) {
        dataIn[i] = static_cast<short>(afu->getID() + 1);
    }
    afu->createInputBufferSW(0, numdados*2, dataIn);
    afu->createOutputBufferSW(0, numdados*2);
    afu->start();
    afu->waitDone(0);

    MSG("AFU "<< afu->getID() <<": INPUT QUEUE 0:");
    cout << "  [APP]  ";
    auto  *dataInpt = (short *) afu->getInputBuffer(0);
    for (short i = 0; i < numdados; ++i) {
        cout << dataInpt[i] << " ";
    }
    cout << endl;
    MSG("AFU "<< afu->getID() <<": OUTPUT QUEUE 0:");
    cout << "  [APP]  ";
    auto  *dataOut = (short *) afu->getOutputBuffer(0);
    for (short i = 0; i < numdados; ++i) {
       cout << dataOut[i] << " ";
    }
    cout << endl;

    if (afu->isDone()) {
        MSG("AFU " << afu->getID() << " DONE!");
    } else {
        MSG("AFU "<< afu->getID()  << " NOT DONE!");
    }

    afu->printDSM();
    
}