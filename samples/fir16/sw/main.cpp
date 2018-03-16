#include <iostream>
#include <cstring>
#include <chrono>
#include <AFUManager.h>

using namespace std;
using namespace std::chrono;

void dataflow_exec(int ** constants, int num_constants ,unsigned short **data_in, int num_data_in,unsigned short **data_out, int num_data_out, int num_copies, bool printAFUStatus);

int main(int argc, char *argv[]){
    
    int auxx = 0;
    int aux = 0;
    if(argc > 2 ){
       auxx = atoi(argv[1]); 
       aux = 32*atoi(argv[2]);
    } 
    else{
        cout << "invalid args!!!"<<endl;
        cout << "usage: <num copies> <num cache lines>" << endl;
        exit(255);
    }
    
    
    int num_copies = auxx;
    int num_constants = 16;
    int num_data_in = aux;
    int num_data_out = num_data_in/16;

    auto ** data_in = (unsigned short **)malloc(sizeof(unsigned short *)*num_copies);
    auto ** data_out = (unsigned short **)malloc(sizeof(unsigned short *)*num_copies);
    auto ** fir_const = (int **)malloc(sizeof(int *)*num_copies);

    for (int i = 0; i < num_copies; i++){
        data_in[i] = (unsigned short *)malloc(sizeof(unsigned short)*num_data_in);
        data_out[i] = (unsigned short *)malloc(sizeof(unsigned short)*num_data_out);
        fir_const[i] = (int *)malloc(sizeof(int)*num_constants);
    }

    bool flag_error = false;

    int constants[] = { 0x1010, 0xf0f, 0xe0e, 0xd0d, 0xc0c, 0xb0b, 0xa0a, 0x909, 0x808, 0x707, 0x606, 0x505, 0x404, 0x303, 0x202, 0x101};


    for (int i = 0; i < num_copies; i++){
        for (int j = 0; j < num_data_in; j++){
            data_in[i][j] = 2;
        }
        for (int j = 0; j < num_constants; j++){
            fir_const[i][j] = constants[j];
        }
    }
    high_resolution_clock::time_point s;
    duration<double> diff{};
    s = high_resolution_clock::now();

    dataflow_exec(fir_const,num_constants,data_in,num_data_in,data_out,num_data_out,num_copies,true);

    diff = high_resolution_clock::now() - s;
    MSG("Execution Time: " << diff.count() * 1000<< "ms");

    for (int i = 0; i < num_copies; i++){
        for (int j = 0; j < num_data_out; j++){
            if(data_out[i][j] != 272){
                MSG("AFU " << i << " Fir16 error: "<< data_out[i][j] << " != 272");
                flag_error = true;
            }
        }
        if(!flag_error){
            MSG("AFU "<< i << ": Fir16 success!");
        }

    }
    return 0;
}

void dataflow_exec(int ** constants, int num_constants ,unsigned short **data_in, int num_data_in,unsigned short **data_out, int num_data_out, int num_copies, bool printAFUStatus) {

    auto *afuManager = new AFUManager();

    for(int i=0;i < num_copies;i++){
        AFU *afu = afuManager->getAFU(static_cast<afuid_t>(i));
        int num_constants_align = 0;
        if(num_constants % 16 == 0)
            num_constants_align = num_constants;
        else
            num_constants_align = num_constants - (num_constants % 16) + 16;

        int *constants_align =  nullptr;

        size_t numBytes = num_constants_align*sizeof(int) + num_data_in*sizeof(unsigned short) + CL(1);

        afu->createInputBufferSW(0, numBytes, nullptr);
        afu->createOutputBufferSW(0, num_data_out*sizeof(unsigned short));

        int context[] = {num_constants_align,num_data_in,num_data_out,0,0,0,0,0,0,0,0,0,0,0,0,0};

        auto *inPtr = (uint8_t*)afu->getInputBuffer(0);

        memcpy(inPtr,context,CL(1));

        if(num_constants_align > 0){
            constants_align = (int *)malloc(sizeof(int)*num_constants_align);
            memset(constants_align,0,sizeof(int)*num_constants_align);
            for(int j = 0; j < num_constants; j++){
                constants_align[j] = constants[i][j];
            }
            memcpy(inPtr + CL(1),constants_align,num_constants_align*sizeof(int));
        }

        memcpy(inPtr + (CL(1) + num_constants_align*sizeof(int)),data_in[i],num_data_in*sizeof(unsigned short));

        delete constants_align;
    }
    auto afuMask = static_cast<uint64_t>((1 << num_copies) - 1);
    afuManager->startAFUs(afuMask);
    afuManager->waitAllDone(0);

    for(int i=0;i < num_copies;i++){
        AFU *afu = afuManager->getAFU(static_cast<afuid_t>(i));
        afu->copyFromOutputBuffer(0,data_out[i],num_data_out*sizeof(unsigned short));
        afu->printDSM();
    }
    if(printAFUStatus){
        afuManager->printInfoAFUManager();
        afuManager->printStatics();
    }
    delete afuManager;
}
