#include <main.h>

using namespace std;

int main(int argc, char *argv[]) {

    if (argc > 1) {
        if (std::strcmp("gen_dot", argv[1]) == 0) {
            generate_dataflows();

        } else if (argc > 3) {
            auto cgra = new Cgra();
            auto cgraArch = new CgraArch(0, 128, 8, 8, 8, 1, 2);
            int num_thread = atoi(argv[2]);
            int data_size = atoi(argv[3]);
            int img_width = atoi(argv[3]);
            int img_height = img_width;
            if (argc > 4)
                img_height = atoi(argv[4]);

            if (std::strcmp("loopback", argv[1]) == 0) {
                LoopBack loopBack(cgra, cgraArch);
                loopBack.benchmarking(num_thread, data_size);
                loopBack.printStatistics();
            } else if (std::strcmp("sobel", argv[1]) == 0) {
                SobelFilter sf(cgra, cgraArch);
                sf.benchmarking(num_thread, img_width, img_height);
                sf.printStatistics();
            } else if (std::strcmp("kmeans", argv[1]) == 0) {
                Kmeans km(cgra, cgraArch, 5, 8);
                km.benchmarking(num_thread, data_size);
                km.printStatistics();
            } else if (std::strcmp("fir", argv[1]) == 0) {
                FIR fir(cgra, cgraArch, 63);
                fir.benchmarking(num_thread, data_size);
                fir.printStatistics();
            } else if (std::strcmp("paeth", argv[1]) == 0) {
                Paeth paeth(cgra, cgraArch);
                paeth.benchmarking(num_thread, data_size);
                paeth.printStatistics();
            } else if (std::strcmp("chebyshev", argv[1]) == 0) {
                Chebyshev chebyshev(cgra, cgraArch);
                chebyshev.benchmarking(num_thread, data_size);
                chebyshev.printStatistics();
            } else if (std::strcmp("mibench", argv[1]) == 0) {
                Mibench mibench(cgra, cgraArch);
                mibench.benchmarking(num_thread, data_size);
                mibench.printStatistics();
            } else if (std::strcmp("poly5", argv[1]) == 0) {
                Poly5 poly5(cgra, cgraArch);
                poly5.benchmarking(num_thread, data_size);
                poly5.printStatistics();
            } else if (std::strcmp("poly6", argv[1]) == 0) {
                Poly6 poly6(cgra, cgraArch);
                poly6.benchmarking(num_thread, data_size);
                poly6.printStatistics();
            } else if (std::strcmp("poly8", argv[1]) == 0) {
                Poly8 poly8(cgra, cgraArch);
                poly8.benchmarking(num_thread, data_size);
                poly8.printStatistics();
            } else if (std::strcmp("sgfilter", argv[1]) == 0) {
                Sgfilter sgfilter(cgra, cgraArch);
                sgfilter.benchmarking(num_thread, data_size);
                sgfilter.printStatistics();
            } else if (std::strcmp("qspline", argv[1]) == 0) {
                Qspline qspline(cgra, cgraArch);
                qspline.benchmarking(num_thread, data_size);
                qspline.printStatistics();
            }
            delete cgra;
            delete cgraArch;
        }
    } else {
        printf("Falta argumentos!\n");
    }
    return 0;
}

void generate_dataflows() {

    auto cgraArch = new CgraArch(0, 128, 8, 8, 8, 1, 2);

    printf("Loopback...\n");
    LoopBack loopBack(nullptr, cgraArch);
    loopBack.createDataFlow(0)->toDot("../dot_dataflows/loopback.dot");
    loopBack.compile(8);

    printf("FIR...\n");
    FIR fir63(nullptr, cgraArch, 63);
    fir63.createDataFlow(0)->toDot("../dot_dataflows/fir.dot");
    fir63.compile(8);

    printf("Kmeans...\n");
    Kmeans kmeans(nullptr, cgraArch, 5, 8);
    kmeans.createDataFlow(0)->toDot("../dot_dataflows/kmeans.dot");
    kmeans.compile(8);

    printf("Paeth...\n");
    Paeth paeth(nullptr, cgraArch);
    paeth.createDataFlow(0, 1)->toDot("../dot_dataflows/paeth.dot");
    paeth.compile(8,1);

    printf("Sobel Filter...\n");
    SobelFilter sobel_filter(nullptr, cgraArch);
    sobel_filter.createDataFlow(0)->toDot("../dot_dataflows/sobel_filter.dot");
    sobel_filter.compile(8);

    printf("Chebyshev...\n");
    Chebyshev chebyshev(nullptr, cgraArch);
    chebyshev.createDataFlow(0, 1)->toDot("../dot_dataflows/chebyshev.dot");
    chebyshev.compile(8,1);

    printf("Mibench...\n");
    Mibench mibench(nullptr, cgraArch);
    mibench.createDataFlow(0, 1)->toDot("../dot_dataflows/mibench.dot");
    mibench.compile(8,1);

    printf("Poly5...\n");
    Poly5 poly5(nullptr, cgraArch);
    poly5.createDataFlow(0, 1)->toDot("../dot_dataflows/poly5.dot");
    poly5.compile(8,1);

    printf("Poly6...\n");
    Poly6 poly6(nullptr, cgraArch);
    poly6.createDataFlow(0, 1)->toDot("../dot_dataflows/poly6.dot");
    poly6.compile(8,1);

    printf("Poly8...\n");
    Poly8 poly8(nullptr, cgraArch);
    poly8.createDataFlow(0, 1)->toDot("../dot_dataflows/poly8.dot");
    poly8.compile(8,1);

    printf("Qspline...\n");
    Qspline qspline(nullptr, cgraArch);
    qspline.createDataFlow(0, 1)->toDot("../dot_dataflows/qspline.dot");
    qspline.compile(8,1);

    printf("Sgfilter...\n");
    Sgfilter sgfilter(nullptr, cgraArch);
    sgfilter.createDataFlow(0, 1)->toDot("../dot_dataflows/sgfilter.dot");
    sgfilter.compile(8,1);

    delete cgraArch;

}
