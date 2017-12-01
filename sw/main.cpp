#include <iostream>
#include "AFU.h"

using namespace std;

int main(int argc, char *argv[]) {

    AFU myAFU("9f81ba12-1d38-4cc7-953a-dafeef45065b");
    char a[128] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    myAFU.addInputBuffer(0, 128, a);
    myAFU.addOutputBuffer(0, 128);
    myAFU.run();
    myAFU.printBuffer(0, AFU::INPUT_BUFFER);
    myAFU.printBuffer(0, AFU::OUTPUT_BUFFER);
    myAFU.printStatics();

}
