#include <main.h>
#include <algorithm>

using namespace std;


int main(int argc, char *argv[]) {

    Cgra cgra;
    CgraArch cgraArch(0, 128, 8, 8, 8, 1, 2);

//    LoopBack loopBack(&cgra, &cgraArch);
//    loopBack.benchmarking(8);
//    loopBack.printStatistics();
//
    SobelFilter sf(&cgra, &cgraArch);
    sf.benchmarking(8);
    sf.printStatistics();

//    Kmeans km(&cgra, &cgraArch,5,8);
//    km.benchmarking(8);
//    km.printStatistics();

//    FIR fir(&cgra, &cgraArch, 48);
//    fir.benchmarking(8);
//    fir.printStatistics();
//
//    Paeth paeth(&cgra, &cgraArch);
//    paeth.benchmarking(8);
//    paeth.printStatistics();

    return 0;
}
