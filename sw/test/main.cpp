#include <iostream>
#include <string.h>
#include <chrono>
#include "../AFUManagerLIB/include/AFUManager.h"

using namespace std;
using namespace std::chrono;

typedef uint64_t data_t;

void loopback(data_t *data_in, data_t *data_out, int num_dados);

int main(int argc, char *argv[]) {
    
    int num_dados = 1 << 12;
    data_t * data_in = new data_t[num_dados];
    data_t * data_out = new data_t[num_dados];
    
    for(int i = 0; i < num_dados; i++){
        data_in[i] = i+1;
    }
    
    high_resolution_clock::time_point s;
    duration<double> diff;
    s = high_resolution_clock::now();
    
    loopback(data_in,data_out,num_dados);
    
    diff = high_resolution_clock::now() - s;
    MSG("Execution Time: " << diff.count() * 1000<< "ms");

    return 0;
}

void loopback(data_t *data_in, data_t *data_out, int num_dados){
    
   auto *afuManager = new AFUManager();
   AFU *afu = afuManager->getAFU(0);
   size_t numBytes = num_dados*sizeof(data_t);
   for(short j = 0;j < afu->getNumInputBuffer();j++){
      afu->createInputBufferSW(j,numBytes, data_in);
      afu->createOutputBufferSW(j,numBytes);
   }    
   afu->start();
   afu->waitDone(0);
   for(short j = 0;j < afu->getNumInputBuffer();j++){
       if(num_dados <= (CL(1)/sizeof(data_t))*128){
            MSG("AFU "<< afu->getID() <<": INPUT QUEUE "<<j<<":");
            cout << "  [APP]  ";
            auto  *dataInpt = (data_t *) afu->getInputBuffer(j);
            for (short i = 0; i < num_dados; ++i) {
                cout << dataInpt[i] << " ";
            }
            cout << endl;
            MSG("AFU "<< afu->getID() <<": OUTPUT QUEUE "<<j<<":");
            cout << "  [APP]  ";
            auto  *dataOut = (data_t *) afu->getOutputBuffer(j);
            for (short i = 0; i < num_dados; ++i) {
                cout << dataOut[i] << " ";
            }
            cout << endl;
       }
       if(memcmp(afu->getInputBuffer(j),afu->getOutputBuffer(j),numBytes) == 0){
          BEGIN_COLOR(GREEN);
          MSG("Success!");
          END_COLOR();
       }else{
          BEGIN_COLOR(RED);
          MSG("Error!");
          END_COLOR();
       }
   }
   
   if (afu->isDone()) {
       MSG("AFU " << afu->getID() << " DONE!");
   } else {
       MSG("AFU "<< afu->getID()  << " NOT DONE!");
   }
   
   afu->printDSM();
   afuManager->printInfoAFUManager();
   afuManager->printStatics();

   delete afuManager;
}
