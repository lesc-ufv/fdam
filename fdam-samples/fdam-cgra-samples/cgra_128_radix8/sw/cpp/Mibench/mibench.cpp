#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <omp.h>

using namespace std;
using namespace std::chrono;

#define DATA_SIZE (1 << 24)
#define NUM_THREAD 8

int mibench(int idx);
int mibench_openmp(int idx);

int main(int argc, char *argv[]){
      
     int idx = 0;
     if(argc > 1)
        idx = atoi(argv[1]);
   
     mibench(idx);
     mibench_openmp(idx);
     
     return 0;
}

int mibench(int idx){
   
   auto A = new unsigned short[DATA_SIZE];
   auto B = new unsigned short[DATA_SIZE];
   auto C = new unsigned short[DATA_SIZE];
   auto OUT = new unsigned short[DATA_SIZE];

   for (int k = 0; k < DATA_SIZE; ++k) {
      A[k] = k;
      B[k] = k;
      C[k] = k;
      OUT[k] = 0;
   }   
   high_resolution_clock::time_point s;
   duration<double> diff = {};
   for(int i =0; i < 100;i++){
   	s = high_resolution_clock::now();
   	for (int k = 0; k < DATA_SIZE; ++k) {
      		OUT[k] = (unsigned short) (A[k]*(6*A[k]+43+B[k])+C[k]*(6*A[k]+43+2*B[k]+C[k])+A[k]*(9*A[k]+1));
   	}	
      diff += high_resolution_clock::now() - s;
   }
   double cpuExecTime = (diff.count() * 1000)/100; 
   printf("Time(ms) 1 Thread: %5.2lf\n",cpuExecTime);
   
   int v = OUT[idx];  

   delete A;
   delete B;
   delete C;
   delete OUT;

  return v;
  
}

int mibench_openmp(int idx){ 
   auto A = new unsigned short[DATA_SIZE];
   auto B = new unsigned short[DATA_SIZE];
   auto C = new unsigned short[DATA_SIZE];
   auto OUT = new unsigned short[DATA_SIZE];

   for (int k = 0; k < DATA_SIZE; ++k) {
      A[k] = k;
      B[k] = k;
      C[k] = k;
      OUT[k] = 0;
   }   
   omp_set_num_threads(NUM_THREAD);
   high_resolution_clock::time_point s;
   duration<double> diff = {};
   for(int i =0; i < 100;i++){
   	s = high_resolution_clock::now();
#pragma omp parallel
#pragma omp for 
   	for (int k = 0; k < DATA_SIZE; ++k) {
      		OUT[k] = (unsigned short) (A[k]*(6*A[k]+43+B[k])+C[k]*(6*A[k]+43+2*B[k]+C[k])+A[k]*(9*A[k]+1));
   	}
	
      diff += high_resolution_clock::now() - s;
   }
   double cpuExecTime = (diff.count() * 1000)/100; 
   printf("Time(ms) %d Thread: %5.2lf\n",NUM_THREAD,cpuExecTime);
   
   int v = OUT[idx];  

   delete A;
   delete B;
   delete C;
   delete OUT;

  return v;




}


