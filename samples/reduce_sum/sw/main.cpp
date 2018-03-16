#include <iostream>
#include <string.h>
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
    int num_constants = 1;
    int num_data_in = aux;
    int num_data_out = 1;

    auto ** data_in = (unsigned short **)malloc(sizeof(unsigned short *)*num_copies);
    auto ** data_out = (unsigned short **)malloc(sizeof(unsigned short *)*num_copies);
    auto ** reduce_sum_constants = (int **)malloc(sizeof(int *)*num_copies);

    for (int i = 0; i < num_copies; i++){
        data_in[i] = (unsigned short *)malloc(sizeof(unsigned short)*num_data_in);
        data_out[i] = (unsigned short *)malloc(sizeof(unsigned short)*num_data_out);
        reduce_sum_constants[i] = (int *)malloc(sizeof(int)*num_constants);
    }

    bool flag_error = false;

    int constants[1] = {((num_data_in/32)<<8)|0x0000001};
    long sum[num_copies];
    for (int k = 0; k < num_copies; ++k) {
        sum[k] = 0;
        for (int j = 0; j < num_data_in; j++){
            if(j%(num_data_in/32)==0)
                data_in[k][j] =  1;
            else
                data_in[k][j] =  0;

            sum[k] +=  data_in[k][j];
        }
        for (int j = 0; j < num_constants; j++){
            reduce_sum_constants[k][j] = constants[j];
        }
    }
    high_resolution_clock::time_point s;
    duration<double> diff{};
    s = high_resolution_clock::now();

    dataflow_exec(reduce_sum_constants,num_constants,data_in,num_data_in,data_out,num_data_out,num_copies,true);

    diff = high_resolution_clock::now() - s;
    MSG("Execution Time: " << diff.count() * 1000<< "ms");

    for (int i = 0; i < num_copies; i++){
        if(data_out[i][0] != sum[i]){
            MSG("AFU " << i << ": REDUCE SUM error: "<< data_out[i][0] << " != "<< sum[i]);
        }
        else{
            MSG("AFU " << i << ": REDUCE SUM success!");
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
