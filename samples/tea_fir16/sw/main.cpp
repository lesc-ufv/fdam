#include <iostream>
#include <cstring>
#include <chrono>
#include <AccManagement.h>

using namespace std;
using namespace std::chrono;

double tea(AccManagement &accMgr,uint32_t ***data_in, uint32_t ***data_out, int num_data_in, int num_data_out);
double fir16(AccManagement &accMgr,int ** constants, int num_constants ,uint16_t  **data_in, int num_data_in,uint16_t **data_out, int num_data_out, int num_copies, bool printAccStatus);
int fir16Call(AccManagement &accMgr, int lines);
int teaCall(AccManagement &accMgr,int lines);

int main(int argc, char *argv[]){
    
    int lines = 0;
    if(argc > 1 ){
       lines = atoi(argv[1]); 
    } 
    else{
        cout << "invalid args!!!"<<endl;
        cout << "usage: <num cache lines>" << endl;
        exit(255);
    }
    auto *accMgr = new AccManagement();

    teaCall(*accMgr,lines);
    fir16Call(*accMgr,lines);

    delete accMgr;

}

int fir16Call(AccManagement &accMgr,int lines){
    
    int num_copies = 2;
    int num_constants = 16;
    int num_data_in = 32*lines;
    int num_data_out = 32*(lines-15);

    auto ** data_in = (unsigned short **)malloc(sizeof(unsigned short *)*num_copies);
    auto ** data_out = (unsigned short **)malloc(sizeof(unsigned short *)*num_copies);
    auto ** fir_const = (int **)malloc(sizeof(int *)*num_copies);

    for (int i = 0; i < num_copies; i++){
        data_in[i] = (unsigned short *)malloc(sizeof(unsigned short)*num_data_in);
        data_out[i] = (unsigned short *)malloc(sizeof(unsigned short)*num_data_out);
        fir_const[i] = (int *)malloc(sizeof(int)*num_constants);
    }

    bool flag_error = false;

    int constants[] = {0x1010, 0xf0f, 0xe0e, 0xd0d, 0xc0c, 0xb0b, 0xa0a, 0x909, 0x808, 0x707, 0x606, 0x505, 0x404, 0x303, 0x202, 0x101};

    for (int i = 0; i < num_copies; i++){
        for (int j = 0; j < num_data_in; j++){
            data_in[i][j] = 2;
        }
        for (int j = 0; j < num_constants; j++){
            fir_const[i][j] = constants[j];
        }
   }

   double timeExec =  fir16(accMgr,fir_const,num_constants,data_in,num_data_in,data_out,num_data_out,num_copies,true);
   double nBytes = (num_data_in + num_data_out) * num_copies * sizeof(uint16_t);
   double nGbytes = nBytes / (1 << 30);
   double thpt = nGbytes / timeExec;
   MSG("Execution Time: " << timeExec * 1000 << "ms");
   MSG("Throughput: " << thpt << "GB/s");
    
   for (int i = 0; i < num_copies; i++){
        for (int j = 0; j < num_data_out; j++){
            if(data_out[i][j] != 272){
                MSG("Acc " << i << " Fir16 error: "<< data_out[i][j] << " != 272");
                flag_error = true;
            }
        }
        if(!flag_error){
            MSG("Acc "<< i << ": Fir16 success!");
        }

    }
    return 0;
}
int teaCall(AccManagement &accMgr,int lines) {
    
    int num_data_in = lines*16;
    int num_data_out = lines*16;
    int num_copies = 2;
    
    auto ***data_in = new uint32_t **[num_copies];
    auto ***data_out = new uint32_t **[num_copies];
    for (int k = 0; k < num_copies; ++k) {
        data_in[k] = new uint32_t *[num_copies];
        data_out[k] = new uint32_t *[num_copies];
        for (int i = 0; i < 2; ++i) {
            data_in[k][i] = new uint32_t[num_data_in];
            data_out[k][i] = new uint32_t[num_data_out];
            for (int j = 0; j < num_data_in; j ++) {
                if(i %2==0){
                    data_in[k][i][j] = static_cast<uint32_t>(0x2885842e);
                }else{
                    data_in[k][i][j] = static_cast<uint32_t>(0xa3e6999b);
                }
            }
            for (int j = 0; j < num_data_out; ++j) {
                data_out[k][i][j] = 0;
            }
        }
    }
    
    double timeExec = tea(accMgr,data_in, data_out, num_data_in, num_data_out);
    double nBytes = (num_data_in + num_data_out) * sizeof(uint32_t);
    double nGbytes = nBytes / (1 << 30);
    double thpt = nGbytes / timeExec;
    MSG("Execution Time: " << timeExec * 1000 << "ms");
    MSG("Throughput: " << thpt << "GB/s");
    
    /*
     for (int k = 0; k < num_copies; ++k) {
        MSG("ACC " << k << ":");
        for (int i = 0; i < 2; ++i) {
            cout << "         OUT "<<i<<":";
            cout << " [ ";
            for (int j = 0; j < num_data_out; ++j) {
                cout << std::hex << data_out[k][i][j] << " ";
            }
            cout <<"]"<< endl;
        }
    }
    */
    
    delete[](data_in);
    delete[](data_out);

    return 0;
}

double fir16(AccManagement &accMgr,int ** constants, int num_constants ,uint16_t  **data_in, int num_data_in,uint16_t **data_out, int num_data_out, int num_copies, bool printAccStatus) {
   

    int num_constants_align = 0;
    if(num_constants % 16 == 0)
       num_constants_align = num_constants;
    else
       num_constants_align = num_constants - (num_constants % 16) + 16;

    int *constants_align =  nullptr;
    size_t numBytesIn = num_constants_align*sizeof(int) + num_data_in*sizeof(unsigned short) + CL(1);
    size_t numBytesOut = num_data_out * sizeof(uint16_t);
    for (int i=2;i < 4;i++) {
        auto &acc = accMgr.getAccelerator(static_cast<accid_t>(i));
        acc.createInputQueue(0, numBytesIn,nullptr); 
        acc.createOutputQueue(0, numBytesOut);
        int context[] = {num_constants_align,num_data_in,num_data_out,0,0,0,0,0,0,0,0,0,0,0,0,0};
        auto *inPtr = (uint8_t*)acc.getInputQueue(0);
        memcpy(inPtr,context,CL(1));
        if(num_constants_align > 0){
            constants_align = (int *)malloc(sizeof(int)*num_constants_align);
            memset(constants_align,0,sizeof(int)*num_constants_align);
            for(int j = 0; j < num_constants; j++){
                constants_align[j] = constants[i-2][j];
            }
            memcpy(inPtr + CL(1),constants_align,num_constants_align*sizeof(int));
        }
        memcpy(inPtr + (CL(1) + num_constants_align*sizeof(int)),data_in[i-2],num_data_in*sizeof(unsigned short));
        delete constants_align;        
    }
    
    uint64_t accMask = 12;
    high_resolution_clock::time_point s;
    duration<double> diff{};
    s = high_resolution_clock::now();

    accMgr.startAccelerators(accMask);
    accMgr.waitDone(accMask,0);

    diff = high_resolution_clock::now() - s;

    for (int i=2;i < 4;i++) {
        auto &acc = accMgr.getAccelerator(static_cast<accid_t>(i));
        acc.copyFromOutputQueue(0, data_out[i-2], numBytesOut);
        if(printAccStatus){
            acc.printInfo();
        }
    }

    accMgr.printHwInfo();


    return diff.count();
    
}

double tea(AccManagement &accMgr,uint32_t ***data_in, uint32_t ***data_out, int num_data_in, int num_data_out) {

    size_t numBytesIn = num_data_in * sizeof(uint32_t);
    size_t numBytesOut = num_data_out * sizeof(uint32_t);
    for (int i = 0; i < 2;i++) {
        auto &acc = accMgr.getAccelerator(static_cast<accid_t>(i));
        auto nIn = acc.getNumInputQueue();
        auto nOut = acc.getNumOutputQueue();
        for (short j = 0; j < nIn; ++j) {
            acc.createInputQueue(static_cast<uint8_t>(j), numBytesIn, data_in[acc.getId()][j]);
        }
        for (short j = 0; j < nOut; ++j) {
            acc.createOutputQueue(static_cast<uint8_t>(j), numBytesOut);
        }
    }

    uint64_t accMask = 3;
    high_resolution_clock::time_point s;
    duration<double> diff{};
    s = high_resolution_clock::now();

    accMgr.startAccelerators(accMask);
    accMgr.waitDone(accMask, 0);

    diff = high_resolution_clock::now() - s;

    for (int i = 0; i < 2;i++) {
        auto &acc = accMgr.getAccelerator(static_cast<accid_t>(i));
        for (short j = 0; j < acc.getNumOutputQueue(); j++) {
            acc.copyFromOutputQueue(static_cast<uint8_t>(j), data_out[acc.getId()][j], numBytesOut);
        }
        if (acc.isDone()) {
            MSG("ACC " << acc.getId() << " DONE!");
        } else {
            MSG("ACC " << acc.getId() << " NOT DONE!");
        }
        acc.printInfo();
    }

    accMgr.printHwInfo();

    return diff.count();
}
