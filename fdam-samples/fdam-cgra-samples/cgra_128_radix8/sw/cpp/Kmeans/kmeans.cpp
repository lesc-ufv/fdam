#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <omp.h>

using namespace std;
using namespace std::chrono;

#define DATA_SIZE (1 << 26)
#define NUM_THREAD 8

int kmeans(int idx, int num_clusters, int num_dim);
int kmeans_openmp(int idx, int num_clusters, int num_dim);

int main(int argc, char *argv[]){
      
     int idx = 0;
     if(argc > 1)
        idx = atoi(argv[1]);
   
     kmeans(idx,5,8);
     kmeans_openmp(idx,5,8);
     
     return 0;
}

int kmeans(int idx, int num_clusters, int num_dim){
   
   unsigned short *data_in, *data_out, *centroids;
   
   data_in  =  new unsigned short[DATA_SIZE*num_dim];
   data_out =  new unsigned short[DATA_SIZE];
   centroids = new unsigned short[num_clusters*num_dim];
   
   for(int i=0;i < DATA_SIZE*num_dim;i++){
     data_in[i] = i;
     data_out[i] = 0;
   }
   for(int i=0;i < num_clusters*num_dim;i++){
     centroids[i] = i;
   }
   
   high_resolution_clock::time_point s;
   duration<double> diff = {};
   
   for(int i = 0; i < 100;i++){
        s = high_resolution_clock::now();
        for (int i = 0; i < DATA_SIZE; i++) {
                    unsigned short min = UINT16_MAX, min_id = 0;
                    for (unsigned short c = 0; c < num_clusters; c++) {
                        unsigned short sum = 0;
                        for (int j = 0; j < num_dim; j++) {
                            sum += data_in[j*num_dim + i] - centroids[c * num_dim + j];
                        }
                        if (sum <= min) {
                            min = sum;
                            min_id = c;
                        }
                    }
                    data_out[i] = min_id;
        }
        diff += high_resolution_clock::now() - s;
   }
   
   double cpuExecTime = (diff.count() * 1000)/100; 

   printf("Time(ms) 1 Thread: %5.2lf\n",cpuExecTime);
   
   int v = data_out[idx];  
   
   delete data_in;
   delete data_out;
   delete centroids;
  
   return v;
}

int kmeans_openmp(int idx, int num_clusters, int num_dim){
   unsigned short *data_in, *data_out, *centroids;
   
   data_in  =  new unsigned short[DATA_SIZE*num_dim];
   data_out =  new unsigned short[DATA_SIZE];
   centroids = new unsigned short[num_clusters*num_dim];
   
   for(int i=0;i < DATA_SIZE*num_dim;i++){
     data_in[i] = i;
     data_out[i] = 0;
   }
   for(int i=0;i < num_clusters*num_dim;i++){
     centroids[i] = i;
   }
   
   omp_set_num_threads(NUM_THREAD);
   
   high_resolution_clock::time_point s;
   duration<double> diff = {};
   
   for(int i = 0; i < 100;i++){
        s = high_resolution_clock::now();
        #pragma omp parallel
        #pragma omp for 
        for (int i = 0; i < DATA_SIZE; i++) {
                    unsigned short min = UINT16_MAX, min_id = 0;
                    for (unsigned short c = 0; c < num_clusters; c++) {
                        unsigned short sum = 0;
                        for (int j = 0; j < num_dim; j++) {
                            sum += data_in[j*num_dim + i] - centroids[c * num_dim + j];
                        }
                        if (sum <= min) {
                            min = sum;
                            min_id = c;
                        }
                    }
                    data_out[i] = min_id;
        }
        diff += high_resolution_clock::now() - s;
   }
   
   double cpuExecTime = (diff.count() * 1000)/100; 

   printf("Time(ms) %d Thread: %5.2lf\n",NUM_THREAD,cpuExecTime);
   
   int v = data_out[idx];  
   
   delete data_in;
   delete data_out;
   delete centroids;
  
   return v;
}


