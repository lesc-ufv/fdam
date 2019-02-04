#include <main.h>
#include <algorithm>

using namespace std;


int main(int argc, char *argv[]) {

    Cgra cgra;
    CgraArch cgraArch(0, 128, 8, 8, 8, 1, 2);

    if(argc < 2){
        printf("usage: ./main <benchmark name>\n");
        printf("./main \"loopback\"\n");
        printf("./main \"sobel\"\n");
        printf("./main \"kmeans\"\n");
        printf("./main \"fir\"\n");
        printf("./main \"paeth\"\n");
        exit(0);
    }

    if(std::strcmp("loopback",argv[1]) == 0){
        LoopBack loopBack(&cgra, &cgraArch);
        loopBack.benchmarking(8);
        loopBack.printStatistics();
    }else if(std::strcmp("sobel",argv[1]) == 0){
        SobelFilter sf(&cgra, &cgraArch);
        sf.benchmarking(8);
        sf.printStatistics();
    }if(std::strcmp("kmeans",argv[1]) == 0) {
        Kmeans km(&cgra, &cgraArch,5,8);
        km.benchmarking(8);
        km.printStatistics();
    }else if(std::strcmp("fir",argv[1]) == 0){
        FIR fir(&cgra, &cgraArch, 48);
        fir.benchmarking(8);
        fir.printStatistics();
    } else if(std::strcmp("paeth",argv[1]) == 0){
        Paeth paeth(&cgra, &cgraArch);
        paeth.benchmarking(8);
        paeth.printStatistics();
    }

    return 0;
}
