#include <iostream>
#include "../AFUManagerLIB/AFUManager.h"

using namespace std;

int main(int argc, char *argv[]) {

    AFUManager myAFU("9f81ba12-1d38-4cc7-953a-dafeef45065b");
    myAFU.printInfoAFU();

    short dataIn[32];
    for (short i = 0; i < 32; ++i) {
        dataIn[i] = (short) (i + 1);
    }

    AFU *subAFU_0 = myAFU.getAFU(0);
    subAFU_0->createInputBufferSW(0,64,dataIn);
    short * dataOut = (short *) subAFU_0->createOutputBufferSW(0, 64);
    AFU *subAFU_1 = myAFU.getAFU(1);
    subAFU_1->createInputBufferSW(0,64,dataIn);
    short * dataOut1 = (short *) subAFU_1->createOutputBufferSW(0, 64);

    myAFU.commitWorkspace();
    subAFU_0->start();
    subAFU_1->start();
    subAFU_0->waitDone(2000);
    subAFU_1->waitDone(2000);
    BEGIN_COLOR(YELLOW);
    MSG("INPUT BUFFER 0:");
    cout <<"  [APP]  ";
    for (short i = 0; i < 32; ++i) {
        cout << dataIn[i] << " ";
    }
    cout << endl;
    END_COLOR();
    BEGIN_COLOR(YELLOW);
    MSG("AFU 0: OUTPUT BUFFER 0:");
    cout <<"  [APP]  ";
    for (short i = 0; i < 32; ++i) {
       cout << dataOut[i] << " ";
    }
    cout << endl;
    MSG("AFU 1: OUTPUT BUFFER 0:");
    cout <<"  [APP]  ";
    for (short i = 0; i < 32; ++i) {
        cout << dataOut1[i] << " ";
    }
    cout << endl;
    END_COLOR();
    if(subAFU_0->isDone()){
        MSG("AFU 0 DONE!");
    }else{
        MSG("AFU 0 NOT DONE!");
    }
    if(subAFU_1->isDone()){
        MSG("AFU 1 DONE!");
    }else{
        MSG("AFU 1 NOT DONE!");
    }
    myAFU.printStatics();
    myAFU.printDSM();
    return 0;
}
