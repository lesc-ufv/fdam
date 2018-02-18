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
    size_t numdados = 32*1024;
    short dataIn[numdados];
    for(short j = 0;j < afu->getNumInputBuffer();j++){
       for (short i = 0; i < numdados; ++i) {
          dataIn[i] = static_cast<short>(j + 1);
       }
       afu->createInputBufferSW(j, numdados*2, dataIn);
       afu->createOutputBufferSW(j, numdados*2);
    }
    
    afu->start();
    afu->waitDone(0);
    if(numdados <= 32*128){
       for(short j = 0;j < afu->getNumInputBuffer();j++){
          MSG("AFU "<< afu->getID() <<": INPUT QUEUE "<<j<<":");
          cout << "  [APP]  ";
          auto  *dataInpt = (short *) afu->getInputBuffer(j);
          for (short i = 0; i < numdados; ++i) {
              cout << dataInpt[i] << " ";
          }
          cout << endl;
          MSG("AFU "<< afu->getID() <<": OUTPUT QUEUE "<<j<<":");
          cout << "  [APP]  ";
          auto  *dataOut = (short *) afu->getOutputBuffer(j);
          for (short i = 0; i < numdados; ++i) {
             cout << dataOut[i] << " ";
          }
          cout << endl;
       }
    }
    if (afu->isDone()) {
        MSG("AFU " << afu->getID() << " DONE!");
    } else {
        MSG("AFU "<< afu->getID()  << " NOT DONE!");
    }

    afu->printDSM();
    
}
