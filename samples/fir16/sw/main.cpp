#include <iostream>
#include <cstring>
#include <dataflow_exec.h>

using namespace std;

int main(int argc, char *argv[]){
    
    int auxx = 0;
    int aux = 0;
    if(argc > 2 ){
       auxx = atoi(argv[1]); 
       aux = atoi(argv[2]);
       if(aux < 16){
           cout << "invalid args!!!"<<endl;
           cout << "Minimum number of lines to fir16 is 16 found "<< atoi(argv[1]) << "." << endl;
           exit(255);
       }
    } 
    else{
        cout << "invalid args!!!"<<endl;
        cout << "usage: <num copies> <num cache lines>" << endl;
        exit(255);
    }
        
    int num_copies = auxx;
    int num_constants = 16;
    int num_data_in = 32*aux;
    int num_data_out = 32*(aux-15);

    auto ** data_in = (unsigned short **)malloc(sizeof(unsigned short *)*num_copies);
    auto ** data_out = (unsigned short **)malloc(sizeof(unsigned short *)*num_copies);
    auto ** fir_const = (int **)malloc(sizeof(int *)*num_copies);

    for (int i = 0; i < num_copies; i++){
        data_in[i] = (unsigned short *)malloc(sizeof(unsigned short)*num_data_in);
        data_out[i] = (unsigned short *)malloc(sizeof(unsigned short)*num_data_out);
        fir_const[i] = (int *)malloc(sizeof(int)*num_constants);
    }

    bool flag_error = false;

    int constants[] = { 0x1010, 0xf0f, 0xe0e, 0xd0d, 0xc0c, 0xb0b, 0xa0a, 0x909, 0x808, 0x707, 0x606, 0x505, 0x404, 0x303, 0x202, 0x101};


    for (int i = 0; i < num_copies; i++){
        for (int j = 0; j < num_data_in; j++){
            data_in[i][j] = 2;
        }
        for (int j = 0; j < num_constants; j++){
            fir_const[i][j] = constants[j];
        }
   }

   double timeExec =  dataflow_exec(fir_const,num_constants,data_in,num_data_in,data_out,num_data_out,num_copies,true);
   double nBytes = (num_data_in + num_data_out) * num_copies * sizeof(uint16_t);
   double nGbytes = nBytes / (1 << 30);
   double thpt = nGbytes / timeExec;
   MSG("Execution Time: " << timeExec * 1000 << "ms");
   MSG("Throughput: " << thpt << "GB/s");
    
   for (int i = 0; i < num_copies; i++){
        for (int j = 0; j < num_data_out; j++){
            if(data_out[i][j] != 272){
                MSG("Acc " << i << " Fir16 error: "<< data_out[i][j] << " != 272");
                flag_error = true;
            }
        }
        if(!flag_error){
            MSG("Acc "<< i << ": Fir16 success!");
        }

    }
    return 0;
}
