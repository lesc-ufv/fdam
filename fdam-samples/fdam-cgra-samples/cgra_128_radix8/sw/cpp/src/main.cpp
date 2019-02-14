#include <main.h>
#include <algorithm>

using namespace std;

int main(int argc, char *argv[]) {

    Cgra *cgra = new Cgra();
    auto cgraArch = new CgraArch(0, 128, 8, 8, 8, 1, 2);

    int num_thread = atoi(argv[2]);
    int data_size = atoi(argv[3]);

    if (std::strcmp("loopback", argv[1]) == 0) {
        LoopBack loopBack(cgra, cgraArch);
        loopBack.benchmarking(num_thread, data_size);
        loopBack.printStatistics();
    } else if (std::strcmp("sobel", argv[1]) == 0) {
        int img_width = atoi(argv[3]);
        int img_height = atoi(argv[4]);
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
    }

    delete cgra;
    delete cgraArch;

    return 0;
}
