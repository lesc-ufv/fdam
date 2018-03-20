#include <iostream>
#include <cstring>
#include <chrono>
#include <AFUManager.h>

using namespace std;
using namespace std::chrono;

void dataflow_exec(int ** constants, int *num_constants ,unsigned short **data_in, int *num_data_in,unsigned short **data_out, int *num_data_out, int num_copies, bool printAFUStatus);

int main(int argc, char *argv[]){

    int aux[4];
    if(argc > 4 ){
        if(atoi(argv[1]) >= 16){
           aux[0] = atoi(argv[1]);
           aux[1] = 32*atoi(argv[2]);
           aux[2] = 32*atoi(argv[3]);
           aux[3] = 32*atoi(argv[4]);            
        }else{
           cout << "invalid args!!!"<<endl;
           cout << "Minimum number of lines to fir16 is 16 found "<< atoi(argv[1]) << "." << endl;
           exit(255);
        }
    }
    else{
        cout << "invalid args!!!"<<endl;
        cout << "usage: <num cache lines for fir16> <num cache lines for gourand> <num cache lines for paeth> <num cache lines for reduce sum>" << endl;
        exit(255);
    }
    
    int num_copies = 4;
    int num_constants[] = {16,6,2,1};
    int num_data_in[]= {32*aux[0],32,aux[2],aux[3]};
    int num_data_out[] = {32*(aux[0]-16),aux[1],aux[2]/2,1};
    bool flag_error = false;

    auto ** data_in = (unsigned short **)malloc(sizeof(unsigned short *)*num_copies);
    auto ** data_out = (unsigned short **)malloc(sizeof(unsigned short *)*num_copies);
    auto ** mixed_const = (int **)malloc(sizeof(int *)*num_copies);

    for (int i = 0; i < num_copies; i++){
        data_in[i] = (unsigned short *)malloc(sizeof(unsigned short)*num_data_in[i]);
        data_out[i] = (unsigned short *)malloc(sizeof(unsigned short)*num_data_out[i]);
        mixed_const[i] = (int *)malloc(sizeof(int)*num_constants[i]);
    }
    //FIR16 DATA SETTINGS:
    int fir16_constants[] = { 0x1010, 0xf0f, 0xe0e, 0xd0d, 0xc0c, 0xb0b, 0xa0a, 0x909, 0x808, 0x707, 0x606, 0x505, 0x404, 0x303, 0x202, 0x101};

    for (int j = 0; j < num_data_in[0]; j++){
        data_in[0][j] = 2;
    }
    for (int j = 0; j < num_constants[0]; j++){
        mixed_const[0][j] = fir16_constants[j];
    }
    
    //GOURAND DATA SETTINGS:
    int gourand_constants[] = { 0x101, 0x202, 0x303, 0xf80004, 0x505, 0xa06};

    for (int j = 0; j < num_data_in[1]; j++){
        data_in[1][j] =  1 << 12;
    }
    for (int j = 0; j < num_constants[1]; j++){
        mixed_const[1][j] = gourand_constants[j];
    }
    
    //PAETH DATA SETTINGS
    int paeth_constants[] = { 0x202, 0x001};
    
    for (int j = 0; j < num_data_in[2]; j++){
        data_in[2][j] =  1;
    }
    for (int j = 0; j < num_constants[2]; j++){
        mixed_const[2][j] = paeth_constants[j];
    }
    
    //REDUCE SUM DATA SETTINGS
    int reduce_sum_constants[1] = {((num_data_in[3]/32)<<8)|0x0000001};
    long sum = 0;
    for (int j = 0; j < num_data_in[3]; j++){
        if(j%(num_data_in[3]/32)==0)
            data_in[3][j] =  1;
        else
            data_in[3][j] =  0;
        
        sum +=  data_in[3][j];
    }
    for (int j = 0; j < num_constants[3]; j++){
        mixed_const[3][j] = reduce_sum_constants[j];
    }    
    
    high_resolution_clock::time_point s;
    duration<double> diff{};
    s = high_resolution_clock::now();

    dataflow_exec(mixed_const,num_constants,data_in,num_data_in,data_out,num_data_out,num_copies,true);

    diff = high_resolution_clock::now() - s;
    MSG("Execution Time: " << diff.count() * 1000<< "ms");
    //FIR16 CHECK OUTPUT
    for (int j = 0; j < num_data_out[0]; j++){
       if(data_out[0][j] != 272){
            MSG("Fir16 error: "<< data_out[0][j] << " != 272");
            flag_error = true;
       }
    }
    if(!flag_error){
        MSG("Fir16 success!");
    }
    //GOURAND CHECK OUTPUT
    flag_error = false;
    for (int j = 0; j < num_data_out[1]-1; j+=2){
        if(data_out[1][j] != 4228 || data_out[1][j+1] != 4096){
            cout << data_out[1][j] << " "<< data_out[1][j+1] << " ";
            MSG("Gourand error: "<< data_out[1][j] << " != 4228 || " << data_out[1][j+1] << " != 4096" );
            flag_error = true;
            break;
        }
    }
    if(!flag_error){
        MSG("Gourand success!");
    }
    //PAETH CHECK OUTPUT
    flag_error = false;    
    for (int j = 0; j < num_data_out[2]; j++){
        if(data_out[2][j] != 1){
            MSG("PAETH error: "<< data_out[2][j] << " != 1");
            flag_error = true;
        }
    }
    if(!flag_error){
        MSG("Paeth success!");
    }
   
    if(data_out[3][0] != sum){
        MSG("Reduce sum error: "<< data_out[3][0] << " != "<< sum );
    }
    else{
        MSG("Reduce sum success!");
    }     
    
    return 0;
}


void dataflow_exec(int ** constants, int *num_constants ,unsigned short **data_in, int *num_data_in,unsigned short **data_out, int *num_data_out, int num_copies, bool printAFUStatus) {
    
    auto *afuManager = new AFUManager(); 
    
    for(int i=0;i < num_copies;i++){
        AFU *afu = afuManager->getAFU(static_cast<afuid_t>(i));
        int num_constants_align = 0;
        if(num_constants[i] % 16 == 0)
            num_constants_align = num_constants[i];
        else
            num_constants_align = num_constants[i] - (num_constants[i] % 16) + 16;

        int *constants_align =  nullptr;

        size_t numBytes = num_constants_align*sizeof(int) + num_data_in[i]*sizeof(unsigned short) + CL(1);

        afu->createInputBufferSW(0, numBytes, nullptr);
        afu->createOutputBufferSW(0, num_data_out[i]*sizeof(unsigned short));

        int context[] = {num_constants_align,num_data_in[i],num_data_out[i],0,0,0,0,0,0,0,0,0,0,0,0,0};

        auto *inPtr = (uint8_t*)afu->getInputBuffer(0);

        memcpy(inPtr,context,CL(1));

        if(num_constants_align > 0){
            constants_align = (int *)malloc(sizeof(int)*num_constants_align);
            memset(constants_align,0,sizeof(int)*num_constants_align);
            for(int j = 0; j < num_constants[i]; j++){
                constants_align[j] = constants[i][j];
            }
            memcpy(inPtr + CL(1),constants_align,num_constants_align*sizeof(int));
        }

        memcpy(inPtr + (CL(1) + num_constants_align*sizeof(int)),data_in[i],num_data_in[i]*sizeof(unsigned short));

        delete constants_align;
    }
    
    auto afuMask = static_cast<uint64_t>((1 << num_copies) - 1);
    afuManager->startAFUs(afuMask);
    afuManager->waitAllDone(0);

    for(int i=0;i < num_copies;i++){
        AFU *afu = afuManager->getAFU(static_cast<afuid_t>(i));
        afu->copyFromOutputBuffer(0,data_out[i],num_data_out[i]*sizeof(unsigned short));
        if(printAFUStatus)
            afu->printDSM();
    }
    if(printAFUStatus){
        afuManager->printInfoAFUManager();
        afuManager->printStatics();
    }
    delete afuManager;
}
