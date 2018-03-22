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

    double time_exec = dataflow_exec(reduce_sum_constants,num_constants,data_in,num_data_in,data_out,num_data_out,num_copies,true);

    MSG("Execution Time: " << time_exec << "ms");

    for (int i = 0; i < num_copies; i++){
        if(data_out[i][0] != sum[i]){
            MSG("AFU " << i << ": Reduce sum error: "<< data_out[i][0] << " != "<< sum[i]);
        }
        else{
            MSG("AFU " << i << ": Reduce sum success!");
        }
    }
    return 0;
}
