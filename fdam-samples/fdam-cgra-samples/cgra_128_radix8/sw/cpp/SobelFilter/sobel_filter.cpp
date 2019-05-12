#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <omp.h>

using namespace std;
using namespace std::chrono;

#define DATA_SIZE (1 << 24)
#define NUM_THREAD 8

int sobel_filter(int idx);
int sobel_filter_openmp(int idx);

int main(int argc, char *argv[]){
      
     int idx = 0;
     if(argc > 1)
        idx = atoi(argv[1]);
   
     sobel_filter(idx);
     sobel_filter_openmp(idx);
     
     return 0;
}

int sobel_filter(int idx){
    
   short sobel_h[] = {-1, 0, 1, -2, 2, -1, 0, 1};
   short sobel_v[] = {1, 2, 1, 0, 0, -1, -2, -1};
   
   short **data_in, *data_out;
   
   data_in = new  short *[8];
   data_out = new short[DATA_SIZE];
   
   for(int i = 0;i < 8;i++){
       data_in[i] = new short[DATA_SIZE];   
       for (int k = 0; k < DATA_SIZE; ++k) {
            data_in[i][k] = k;
        }       
   }
   
   for (int k = 0; k < DATA_SIZE; ++k) {
       data_out[k] = 0;
   }   
   
   high_resolution_clock::time_point s;
   duration<double> diff = {};
   
   for(int i = 0; i < 100;i++){
        s = high_resolution_clock::now();
        for (int k = 0; k < DATA_SIZE; ++k) {
            short sum_h = 0,sum_v = 0;
            for(int j = 0;j < 8;j++){
                sum_h += data_in[j][k] * sobel_h[j]; 
                sum_v += data_in[j][k] * sobel_v[j]; 
            }
            data_out[k] = (sum_h*sum_h) + (sum_v*sum_v);
        }
        diff += high_resolution_clock::now() - s;
   }
   
   double cpuExecTime = (diff.count() * 1000)/100; 

   printf("Time(ms) 1 Thread: %5.2lf\n",cpuExecTime);
   
   int v = data_out[idx];  
   
   for(int i = 0;i < 8;i++){
     delete data_in[i];   
   }
   
   delete data_in;
   delete data_out;

  return v;
    
}

int sobel_filter_openmp(int idx){
    
   short sobel_h[] = {-1, 0, 1, -2, 2, -1, 0, 1};
   short sobel_v[] = {1, 2, 1, 0, 0, -1, -2, -1};
   
   short **data_in, *data_out;
   
   data_in = new  short *[8];
   data_out = new short[DATA_SIZE];
   
   for(int i = 0;i < 8;i++){
       data_in[i] = new short[DATA_SIZE];   
       for (int k = 0; k < DATA_SIZE; ++k) {
            data_in[i][k] = k;
        }       
   } 

   for (int k = 0; k < DATA_SIZE; ++k) {
       data_out[k] = 0;
   } 
 
   omp_set_num_threads(NUM_THREAD);
    
   high_resolution_clock::time_point s;
   duration<double> diff = {};
   
   for(int i = 0; i < 100;i++){
        s = high_resolution_clock::now();
        #pragma omp parallel
        #pragma omp for 
        for (int k = 0; k < DATA_SIZE; ++k) {
            short sum_h = 0,sum_v = 0;
            for(int j = 0;j < 8;j++){
                sum_h += data_in[j][k] * sobel_h[j]; 
                sum_v += data_in[j][k] * sobel_v[j]; 
            }
            data_out[k] = (sum_h*sum_h) + (sum_v*sum_v);
            
        }
        diff += high_resolution_clock::now() - s;
   }
    
   double cpuExecTime = (diff.count() * 1000)/100; 

   printf("Time(ms) %d Thread: %5.2lf\n",NUM_THREAD,cpuExecTime);

   int v = data_out[idx];  
   
   for(int i = 0;i < 8;i++){
     delete data_in[i];   
   }
   
   delete data_in;
   delete data_out;

  return v;
}


