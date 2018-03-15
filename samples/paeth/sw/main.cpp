#include <iostream>
#include <string.h>
#include <chrono>
#include <AFUManager.h>

using namespace std;
using namespace std::chrono;

void dataflow_exec(int * constants, int num_constants ,short *data_in, int num_data_in, short *data_out, int num_data_out, bool printAFUStatus);

int main(int argc, char *argv[]){
    
    short num_dados_in = 99;
    short num_data_out = num_dados_in/3;
    short data_in[num_dados_in];
    short data_out[num_data_out];
    bool flag_error = false;
    
    int constants[2] = { 0x202, 0x001};
   
    for (int i = 0; i < num_dados_in; i++){
        data_in[i] = 1;
    }  
    
    high_resolution_clock::time_point s;
    duration<double> diff;
    s = high_resolution_clock::now();
    
    dataflow_exec(constants,2,data_in,num_dados_in,data_out,num_data_out,true);
    
    diff = high_resolution_clock::now() - s;
    MSG("Execution Time: " << diff.count() * 1000<< "ms");
    
    for (int i = 0; i < num_data_out; i++){
        if(data_out[i] != 1){
            MSG("Error: "<< data_out[i] << " != 1" );
            flag_error = true; 
            break;
        }
    } 
    if(!flag_error){
        MSG("Sucess!");
    } 
    
    return 0;
}
void  dataflow_exec(int * constants, int num_constants ,short *data_in, int num_data_in, short *data_out, int num_data_out, bool printAFUStatus) {

    auto *afuManager = new AFUManager();
    
    AFU *afu = afuManager->getAFU(0);
    
    int num_constants_align = 0;
    if(num_constants % 16 == 0)
        num_constants_align = num_constants;
    else
        num_constants_align = num_constants - (num_constants % 16) + 16;
        
    int *constants_align =  nullptr;
    
    int numBytes = num_constants_align*sizeof(int) + num_data_in*sizeof(short) + CL(1);
    
    afu->createInputBufferSW(0, numBytes, nullptr);
    afu->createOutputBufferSW(0, num_data_out*sizeof(short));
    
    int context[] = {num_constants_align,num_data_in,num_data_out,0,0,0,0,0,0,0,0,0,0,0,0,0};
    
    uint8_t *inPtr = (uint8_t*)afu->getInputBuffer(0);
    
    memcpy(inPtr,context,CL(1)); 
    
    if(num_constants_align > 0){
        constants_align = new int [num_constants_align];
        for(int i = 0; i < num_constants; i++){
            constants_align[i] = constants[i];
        }
        memcpy(inPtr + CL(1),constants_align,num_constants_align*sizeof(int));
    }
        
    memcpy(inPtr + (CL(1) + num_constants_align*sizeof(int)),data_in,num_data_in*sizeof(short));
    
    afu->start();
    
    afu->waitDone(0);
      
    afu->copyFromOutputBuffer(0,data_out,num_data_out*sizeof(short)); 
    
    if(printAFUStatus){
        afu->printDSM();
        afuManager->printInfoAFUManager();
        afuManager->printStatics();
    }
    delete afuManager;
}
