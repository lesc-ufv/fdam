#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <omp.h>

using namespace std;
using namespace std::chrono;

#define DATA_SIZE (25165824)
#define NUM_THREAD 8

int sgfilter(int idx);
int sgfilter_openmp(int idx);

int main(int argc, char *argv[]){
      
     int idx = 0;
     if(argc > 1)
        idx = atoi(argv[1]);
   
     sgfilter(idx);
     sgfilter_openmp(idx);
     
     return 0;
}

int sgfilter(int idx){
   
   auto a = new unsigned short[DATA_SIZE];
   auto b = new unsigned short[DATA_SIZE];
   auto OUT = new unsigned short[DATA_SIZE];

   for (int k = 0; k < DATA_SIZE; ++k) {
      a[k] = k % 1024;
      b[k] = k % 1024;
      OUT[k] = 0;
   }   
   high_resolution_clock::time_point s;
   duration<double> diff = {};
   for(int i =0; i < 100;i++){
   	s = high_resolution_clock::now();
   	for (int k = 0; k < DATA_SIZE; ++k) {
      		OUT[k] = ((-984*a[k]-46)*a[k]-46)*a[k]+((92*a[k]-39)*a[k]+(-76*a[k]+7*b[k]+7)*b[k]+7)*b[k]-75;
   	}	
      diff += high_resolution_clock::now() - s;
   }
   double cpuExecTime = (diff.count() * 1000)/100; 
   printf("Time(ms) 1 Thread: %5.2lf\n",cpuExecTime);
   
   int v = OUT[idx];  

   delete a;
   delete b;
   delete OUT;

  return v;
  
}

int sgfilter_openmp(int idx){ 
   auto a = new unsigned short[DATA_SIZE];
   auto b = new unsigned short[DATA_SIZE];
   auto OUT = new unsigned short[DATA_SIZE];

   for (int k = 0; k < DATA_SIZE; ++k) {
      a[k] = k % 1024;
      b[k] = k % 1024;
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
      		OUT[k] = ((-984*a[k]-46)*a[k]-46)*a[k]+((92*a[k]-39)*a[k]+(-76*a[k]+7*b[k]+7)*b[k]+7)*b[k]-75;
   	}
	
      diff += high_resolution_clock::now() - s;
   }
   double cpuExecTime = (diff.count() * 1000)/100; 
   printf("Time(ms) %d Thread: %5.2lf\n",NUM_THREAD,cpuExecTime);
   
   int v = OUT[idx];  

   delete a;
   delete b;
   delete OUT;

  return v;




}


