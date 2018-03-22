#include <iostream>
#include <string.h>
#include <chrono>
#include <AFUManager.h>

using namespace std;
using namespace std::chrono;

int main(int argc, char *argv[]){

    int aux = 0;
    if(argc > 1 ){
       aux = 16*atoi(argv[1]);
    } 
    else{
        cout << "invalid args!!!"<<endl;
        cout << "usage: <num cache lines>" << endl;
        exit(255);
    }

    int num_data_out = aux;

    auto *data_out = (unsigned int *)malloc(sizeof(unsigned int)*num_data_out);
    
    memset(data_out,0x00,sizeof(unsigned int)*num_data_out);
    
    bool flag_error = false;
    
    auto *afuManager = new AFUManager();
    AFU *afu = afuManager->getAFU(0);
    
    afu->createOutputBufferSW(0, num_data_out*sizeof(unsigned int));

    high_resolution_clock::time_point s;
    duration<double> diff{};
    s = high_resolution_clock::now();
    
    afu->start();
    afu->waitDone(0);    
    
    diff = high_resolution_clock::now() - s;
    MSG("Execution Time: " << diff.count() * 1000<< "ms");

    afu->copyFromOutputBuffer(0,data_out,num_data_out*sizeof(unsigned int));
   
    for (int i = 0; i < num_data_out; i++){
        if(data_out[i] != 0x01){
            flag_error = true;
            break;
        }
    }
    if(flag_error){
        MSG("AFU 0: read error: ");
    }else{
         MSG("AFU 0: read success!");
    }
    
    afu->printDSM();
    afuManager->printInfoAFUManager();
    afuManager->printStatics();
    delete afuManager;
    
    return 0;
}
