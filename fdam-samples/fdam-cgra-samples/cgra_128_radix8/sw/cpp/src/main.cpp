#include <main.h>
#include <algorithm>

using namespace std;

int main(int argc, char *argv[]) {

    int num_thread = 1;
    int data_size = 32;
    if(argc > 3) {
        num_thread = atoi(argv[2]);
        data_size = atoi(argv[3]);
    }
    if (std::strcmp("gen_dot", argv[1]) == 0) {
        generate_dataflows();

    } else if (std::strcmp("loopback", argv[1]) == 0) {
        auto cgra = new Cgra();
        auto cgraArch = new CgraArch(0, 128, 8, 8, 8, 1, 2);
        LoopBack loopBack(cgra, cgraArch);
        loopBack.benchmarking(num_thread, data_size);
        loopBack.printStatistics();
        delete cgra;
        delete cgraArch;
    } else if (std::strcmp("sobel", argv[1]) == 0) {
        auto cgra = new Cgra();
        auto cgraArch = new CgraArch(0, 128, 8, 8, 8, 1, 2);
        int img_width = atoi(argv[3]);
        int img_height = atoi(argv[4]);
        SobelFilter sf(cgra, cgraArch);
        sf.benchmarking(num_thread, img_width, img_height);
        sf.printStatistics();
        delete cgra;
        delete cgraArch;
    } else if (std::strcmp("kmeans", argv[1]) == 0) {
        auto cgra = new Cgra();
        auto cgraArch = new CgraArch(0, 128, 8, 8, 8, 1, 2);
        Kmeans km(cgra, cgraArch, 5, 8);
        km.benchmarking(num_thread, data_size);
        km.printStatistics();
        delete cgra;
        delete cgraArch;
    } else if (std::strcmp("fir", argv[1]) == 0) {
        auto cgra = new Cgra();
        auto cgraArch = new CgraArch(0, 128, 8, 8, 8, 1, 2);
        FIR fir(cgra, cgraArch, 63);
        fir.benchmarking(num_thread, data_size);
        fir.printStatistics();
        delete cgra;
        delete cgraArch;
    } else if (std::strcmp("paeth", argv[1]) == 0) {
        auto cgra = new Cgra();
        auto cgraArch = new CgraArch(0, 128, 8, 8, 8, 1, 2);
        Paeth paeth(cgra, cgraArch);
        paeth.benchmarking(num_thread, data_size);
        paeth.printStatistics();
        delete cgra;
        delete cgraArch;
    }


    return 0;
}

void generate_dataflows(){

    auto cgraArch = new CgraArch(0, 128, 8, 8, 8, 1, 2);

    LoopBack loopBack(nullptr, cgraArch);
    loopBack.createDataFlow(0)->toDot("../dot_dataflows/loopBack.dot");

    FIR fir64(nullptr, cgraArch, 64);
    fir64.createDataFlow(0)->toDot("../dot_dataflows/fir64.dot");

    Kmeans kmeans(nullptr, cgraArch, 5, 8);
    kmeans.createDataFlow(0)->toDot("../dot_dataflows/kmeans.dot");

    Paeth paeth(nullptr, cgraArch);
    paeth.createDataFlow(0)->toDot("../dot_dataflows/paeth.dot");

    SobelFilter sobel_filter(nullptr, cgraArch);
    sobel_filter.createDataFlow(0)->toDot("../dot_dataflows/sobel_filter.dot");

    Chebyshev chebyshev(nullptr, cgraArch);
    chebyshev.createDataFlow(0)->toDot("../dot_dataflows/chebyshev.dot");

    Mibench mibench(nullptr, cgraArch);
    mibench.createDataFlow(0)->toDot("../dot_dataflows/mibench.dot");

    Poly5 poly5(nullptr,cgraArch);
    poly5.createDataFlow(0)->toDot("../dot_dataflows/poly.dot");

    Qspline qspline(nullptr,cgraArch);
    qspline.createDataFlow(0)->toDot("../dot_dataflows/qspline.dot");

    Sgfilter sgfilter(nullptr,cgraArch);
    sgfilter.createDataFlow(0)->toDot("../dot_dataflows/sgfilter.dot");

    delete cgraArch;

}