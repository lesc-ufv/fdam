#include <iostream>
#include <string.h>
#include <chrono>
#include <AFUManager.h>

using namespace std;
using namespace std::chrono;

void dataflow_exec(int * constants, int num_constants ,unsigned short *data_in, int num_data_in,unsigned short *data_out, int num_data_out, bool printAFUStatus);

int main(int argc, char *argv[]){
    
    int num_data_in = 32;
    int num_data_out = 256;
    unsigned short * data_in = (unsigned short *) malloc(sizeof(unsigned short) * num_data_in);
    unsigned short * data_out = (unsigned short*) malloc(sizeof(unsigned short) * num_data_out);
    bool flag_error = false;
    
    int constants[6] = { 0x101, 0x202, 0x303, 0xf80004, 0x505, 0xa06};
   
    for (int i = 0; i < num_data_in; i++){
        data_in[i] = 1 << 12;
    }  
    
    high_resolution_clock::time_point s;
    duration<double> diff;
    s = high_resolution_clock::now();
    
    dataflow_exec(constants,6,data_in,num_data_in,data_out,num_data_out,true);
    
    diff = high_resolution_clock::now() - s;
    MSG("Execution Time: " << diff.count() * 1000<< "ms");
    
    for (int i = 0; i < num_data_out; i+=2){
        if(data_out[i] != 4228 || data_out[i+1] != 4096){
            cout << data_out[i] << " "<< data_out[i+1] << " ";
            MSG("Error: "<< data_out[i] << " != 4228 || " << data_out[i+1] << " != 4096" );
            flag_error = true; 
            break;
        }
    } 
    if(!flag_error){
        MSG("Sucess!");
    } 
    
    return 0;
}
void  dataflow_exec(int * constants, int num_constants ,unsigned short *data_in, int num_data_in,unsigned short *data_out, int num_data_out, bool printAFUStatus) {

    auto *afuManager = new AFUManager();
    
    AFU *afu = afuManager->getAFU(0);
    
    int num_constants_align = 0;
    if(num_constants % 16 == 0)
        num_constants_align = num_constants;
    else
        num_constants_align = num_constants - (num_constants % 16) + 16;
        
    int *constants_align =  nullptr;
    
    int numBytes = num_constants_align*sizeof(int) + num_data_in*sizeof(unsigned short) + CL(1);
    
    afu->createInputBufferSW(0, numBytes, nullptr);
    afu->createOutputBufferSW(0, num_data_out*sizeof(unsigned short));
    
    int context[] = {num_constants_align,num_data_in,num_data_out,0,0,0,0,0,0,0,0,0,0,0,0,0};
    
    uint8_t *inPtr = (uint8_t*)afu->getInputBuffer(0);
    
    memcpy(inPtr,context,CL(1)); 
    
    if(num_constants_align > 0){
        constants_align = (int *) malloc(sizeof(int) * num_constants_align);
        memset(constants_align,0,sizeof(int) * num_constants_align);
        for(int i = 0; i < num_constants; i++){
            constants_align[i] = constants[i];
        }
        memcpy(inPtr + CL(1),constants_align,num_constants_align*sizeof(int));
    }
        
    memcpy(inPtr + (CL(1) + num_constants_align*sizeof(int)),data_in,num_data_in*sizeof(unsigned short));
    
    afu->start();
    
    afu->waitDone(0);
      
    afu->copyFromOutputBuffer(0,data_out,num_data_out*sizeof(unsigned short)); 
    
    if(printAFUStatus){
        afu->printDSM();
        afuManager->printInfoAFUManager();
        afuManager->printStatics();
    }
    
    if(constants_align != nullptr){
        delete constants_align;
    }
    
    delete afuManager;
}
