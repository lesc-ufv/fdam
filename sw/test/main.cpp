#include <iostream>
#include "../AFUManagerLIB/AFUManager.h"

using namespace std;

void testLB();

int main(int argc, char *argv[]) {

    testLB();
    return 0;
}

void testLB(){

    AFUManager *afuManager = new AFUManager("9f81ba12-1d38-4cc7-953a-dafeef45065b");
    AFU *afu;
    map<afu_id, AFU *>::const_iterator begin,end,it;
    begin =  afuManager->getAFUs().begin();
    end = afuManager->getAFUs().end();

    short dataIn[32];

    for (it = begin; it != end; ++it) {
        afu = it->second;
        for (short i = 0; i < 32; ++i) {
            dataIn[i] = (short) afu->getID();
        }
        afu->createInputBufferSW(0, 64, dataIn);
        afu->createOutputBufferSW(0, 64);
    }

    uint64_t  mask_afus = (uint64_t)((1L << afuManager->getNumAFUs()) - 1L);
    afuManager->startAFUs(mask_afus);
    afuManager->waitAllDone(2000);



    for (it = begin; it != end; ++it) {
        MSG("AFU "<< it->first <<": INPUT BUFFER 0:");
        cout << "  [APP]  ";
        short  *dataInpt = (short *) it->second->getInputBuffer(0);
        for (short i = 0; i < 32; ++i) {
            cout << dataInpt[i] << " ";
        }
        cout << endl;
        MSG("AFU "<< it->first <<": OUTPUT BUFFER 0:");
        cout << "  [APP]  ";
        short  *dataOut = (short *) it->second->getOutputBuffer(0);
        for (short i = 0; i < 32; ++i) {
            cout << dataOut[i] << " ";
        }
        cout << endl;
    }

    for (it = begin; it != end; ++it) {
        if (it->second->isDone()) {
            MSG("AFU " << it->first << " DONE!");
        } else {
            MSG("AFU "<< it->first << " NOT DONE!");
        }
    }

    afuManager->printInfoAFUManager();
    afuManager->printStatics();
    afuManager->printDSM();

    delete afuManager;
}