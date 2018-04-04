#include <iostream>
#include <string.h>
#include <dataflow_exec.h>

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
    int num_constants = 2;
    int num_data_in = aux;
    int num_data_out = num_data_in/2;

    auto ** data_in = (unsigned short **)malloc(sizeof(unsigned short *)*num_copies);
    auto ** data_out = (unsigned short **)malloc(sizeof(unsigned short *)*num_copies);
    auto ** paeth_const = (int **)malloc(sizeof(int *)*num_copies);

    for (int i = 0; i < num_copies; i++){
        data_in[i] = (unsigned short *)malloc(sizeof(unsigned short)*num_data_in);
        data_out[i] = (unsigned short *)malloc(sizeof(unsigned short)*num_data_out);
        paeth_const[i] = (int *)malloc(sizeof(int)*num_constants);
    }

    bool flag_error = false;

    int constants[] = { 0x202, 0x001};

    for (int i = 0; i < num_copies; i++){
        for (int j = 0; j < num_data_in; j++){
            data_in[i][j] = 1;
        }
        for (int j = 0; j < num_constants; j++){
            paeth_const[i][j] = constants[j];
        }
    }

    double timeExec = dataflow_exec(paeth_const,num_constants,data_in,num_data_in,data_out,num_data_out,num_copies,true);
    double nBytes = (num_data_in + num_data_out) * num_copies * sizeof(uint16_t);
    double nGbytes = nBytes / (1 << 30);
    double thpt = nGbytes / timeExec;
    MSG("Execution Time: " << timeExec * 1000 << "ms");
    MSG("Throughput: " << thpt << "GB/s");

    for (int i = 0; i < num_copies; i++){
        for (int j = 0; j < num_data_out; j++){
            if(data_out[i][j] != 1){
                MSG("AFU " << i << ": Paeth error: "<< data_out[i][j] << " != 1");
                flag_error = true;
            }
        }
        if(!flag_error){
            MSG("AFU " << i << ": Paeth success!");
        }
    }
    return 0;
}
