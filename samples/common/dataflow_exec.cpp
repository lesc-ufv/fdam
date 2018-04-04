//
// Created by lucas on 12/28/17.
//

#include <dataflow_exec.h>

double dataflow_exec(int ** constants, int num_constants ,uint16_t  **data_in, int num_data_in,uint16_t **data_out, int num_data_out, int num_copies, bool printAccStatus) {
   
    auto *accMgr = new AccManagement();
    int num_constants_align = 0;
    if(num_constants % 16 == 0)
       num_constants_align = num_constants;
    else
       num_constants_align = num_constants - (num_constants % 16) + 16;

    int *constants_align =  nullptr;
    size_t numBytesIn = num_constants_align*sizeof(int) + num_data_in*sizeof(unsigned short) + CL(1);
    size_t numBytesOut = num_data_out * sizeof(uint16_t);
    for (auto &it:accMgr->getAccelerators()) {
        auto &acc = it.second;
        auto nIn = acc->getNumInputQueue();
        auto nOut = acc->getNumOutputQueue();
        acc->createInputQueue(0, numBytesIn,nullptr); 
        acc->createOutputQueue(0, numBytesOut);
        int context[] = {num_constants_align,num_data_in,num_data_out,0,0,0,0,0,0,0,0,0,0,0,0,0};
        auto *inPtr = (uint8_t*)acc->getInputQueue(0);
        memcpy(inPtr,context,CL(1));
        if(num_constants_align > 0){
            constants_align = (int *)malloc(sizeof(int)*num_constants_align);
            memset(constants_align,0,sizeof(int)*num_constants_align);
            for(int j = 0; j < num_constants; j++){
                constants_align[j] = constants[acc->getId()][j];
            }
            memcpy(inPtr + CL(1),constants_align,num_constants_align*sizeof(int));
        }
        memcpy(inPtr + (CL(1) + num_constants_align*sizeof(int)),data_in[acc->getId()],num_data_in*sizeof(unsigned short));
        delete constants_align;        
    }
    
    auto accMask = static_cast<uint64_t>((1 << accMgr->getNumAccelerators()) - 1L);
    high_resolution_clock::time_point s;
    duration<double> diff{};
    s = high_resolution_clock::now();

    accMgr->startAccelerators(accMask);
    accMgr->waitDone(accMask,0);

    diff = high_resolution_clock::now() - s;

    for (auto &it:accMgr->getAccelerators()) {
        auto &acc = it.second;
        acc->copyFromOutputQueue(0, data_out[acc->getId()], numBytesOut);
        if(printAccStatus){
            acc->printInfo();
        }
    }

    accMgr->printHwInfo();

    delete accMgr;

    return diff.count();
    
}
