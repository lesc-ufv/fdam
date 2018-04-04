#include <iostream>
#include <cstring>
#include<dataflow_exec.h>

using namespace std;

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
    int num_constants = 6;
    int num_data_in = 32;
    int num_data_out = aux;

    auto ** data_in = (unsigned short **)malloc(sizeof(unsigned short *)*num_copies);
    auto ** data_out = (unsigned short **)malloc(sizeof(unsigned short *)*num_copies);
    auto ** gourand_const = (int **)malloc(sizeof(int *)*num_copies);

    for (int i = 0; i < num_copies; i++){
        data_in[i] = (unsigned short *)malloc(sizeof(unsigned short)*num_data_in);
        data_out[i] = (unsigned short *)malloc(sizeof(unsigned short)*num_data_out);
        gourand_const[i] = (int *)malloc(sizeof(int)*num_constants);
    }

    bool flag_error = false;

    int constants[6] = { 0x101, 0x202, 0x303, 0xf80004, 0x505, 0xa06};

    for (int i = 0; i < num_copies; i++){
        for (int j = 0; j < num_data_in; j++){
            data_in[i][j] = 4096;
        }
        for (int j = 0; j < num_constants; j++){
            gourand_const[i][j] = constants[j];
        }
    }

    double timeExec = dataflow_exec(gourand_const,num_constants,data_in,num_data_in,data_out,num_data_out,num_copies,true);
    double nBytes = (num_data_in + num_data_out) * num_copies * sizeof(uint16_t);
    double nGbytes = nBytes / (1 << 30);
    double thpt = nGbytes / timeExec;
    MSG("Execution Time: " << timeExec * 1000 << "ms");
    MSG("Throughput: " << thpt << "GB/s");

    for (int i = 0; i < num_copies; i++){
        flag_error = false;
        for (int j = 0; j < num_data_out-1; j+=2){
            if(data_out[i][j] != 4228 || data_out[i][j+1] != 4096){
                MSG("Acc "<< i << ": Gourand error: "<< data_out[i][j] << " != 4228 || " << data_out[i][j+1] << " != 4096" );
                flag_error = true;
                break;
            }
        }
        if(!flag_error){
            MSG("Acc " << i << ": Gourand success!");
        }
    }
    return 0;
}
