//
// Created by lucas on 12/28/17.
//

#include <dataflow_exec.h>

double dataflow_exec(int ** constants, int num_constants ,unsigned short **data_in, int num_data_in,unsigned short **data_out, int num_data_out, int num_copies, bool printAFUStatus) {

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
    
    high_resolution_clock::time_point s;
    duration<double> diff{};
    s = high_resolution_clock::now();
    
    afuManager->startAFUs(afuMask);
    afuManager->waitAllDone(0);
    
    diff = high_resolution_clock::now() - s;

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
    
    return diff.count() * 1000;
}
