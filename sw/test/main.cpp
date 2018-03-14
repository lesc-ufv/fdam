#include <iostream>
#include "../AFUManagerLIB/include/AFUManager.h"
#include <string.h>

using namespace std;

typedef uint64_t data_t;

void testLB_afu(AFUManager &afuManager,afuid_t id);

int main(int argc, char *argv[]) {

    auto *afuManager = new AFUManager();
    testLB_afu(*afuManager,0);
    afuManager->printInfoAFUManager();
    afuManager->printStatics();

    delete afuManager;
    return 0;
}
void testLB_afu(AFUManager &afuManager,afuid_t id){
    
    
    AFU *afu = afuManager.getAFU(id);
    size_t numCL = 16;
    size_t numdados = (CL(1)/sizeof(data_t))*numCL;
    data_t dataIn[numdados];
    for(short j = 0;j < afu->getNumInputBuffer();j++){
       for (short i = 0; i < numdados; ++i) {
          dataIn[i] = static_cast<data_t>(i%sizeof(data_t) + 1);
       }
       afu->createInputBufferSW(j, numdados*sizeof(data_t), dataIn);
       afu->createOutputBufferSW(j, numdados*sizeof(data_t));
    }
    
    afu->start();
    afu->waitDone(0);
    if(numdados <= (CL(1)/sizeof(data_t))*128){
       for(short j = 0;j < afu->getNumInputBuffer();j++){
          MSG("AFU "<< afu->getID() <<": INPUT QUEUE "<<j<<":");
          cout << "  [APP]  ";
          auto  *dataInpt = (data_t *) afu->getInputBuffer(j);
          for (short i = 0; i < numdados; ++i) {
              cout << dataInpt[i] << " ";
          }
          cout << endl;
          MSG("AFU "<< afu->getID() <<": OUTPUT QUEUE "<<j<<":");
          cout << "  [APP]  ";
          auto  *dataOut = (data_t *) afu->getOutputBuffer(j);
          for (short i = 0; i < numdados; ++i) {
             cout << dataOut[i] << " ";
          }
          cout << endl;
          if(memcmp(afu->getInputBuffer(j),afu->getOutputBuffer(j),numdados*sizeof(data_t)) == 0){
             BEGIN_COLOR(GREEN);
             MSG("Success!");
             END_COLOR();
          }else{
             BEGIN_COLOR(RED);
             MSG("Error!");
             END_COLOR();
          }
       }
    }
    if (afu->isDone()) {
        MSG("AFU " << afu->getID() << " DONE!");
    } else {
        MSG("AFU "<< afu->getID()  << " NOT DONE!");
    }

    afu->printDSM();
    
}
