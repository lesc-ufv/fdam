#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <omp.h>

using namespace std;
using namespace std::chrono;

#define DATA_SIZE (1 << 24)
#define NUM_THREAD 8

int poly5(int idx);
int poly5_openmp(int idx);

int main(int argc, char *argv[]){
      
     int idx = 0;
     if(argc > 1)
        idx = atoi(argv[1]);
   
     poly5(idx);
     poly5_openmp(idx);
     
     return 0;
}

int poly5(int idx){

   unsigned short *a, *b, *c, *out;
   
   a = new unsigned short[DATA_SIZE];
   b = new unsigned short[DATA_SIZE];
   c = new unsigned short[DATA_SIZE];
   out = new unsigned short[DATA_SIZE];

   for (int k = 0; k < DATA_SIZE; ++k) {
      a[k] = k;
      b[k] = k;
      c[k] = k;
      out[k] = 0;
   }   

   high_resolution_clock::time_point s;
   duration<double> diff = {};

   for(int i=0;i<100;i++){
   	s = high_resolution_clock::now();
   	for (int k = 0; k < DATA_SIZE; ++k) {
      	    out[k] = ((a[k]*(a[k]*(a[k-432])+62208)-2985984)*b[k]*(a[k]*(78*a[k]-9504)+b[k]*(288*a[k]-5184))+c[k]*b[k]*(a[k]*(a[k]-207)+3456-b[k]*(2*a[k]-c[k]+144)))*c[k];
  	}
   	diff += high_resolution_clock::now() - s;
   }
   double cpuExecTime = (diff.count() * 1000)/100; 

   printf("Time(ms) 1 Thread: %5.2lf\n",cpuExecTime);
   
   int v = out[idx];  

   delete a;
   delete b;
   delete c;
   delete out;

  return v;
  
}

int poly5_openmp(int idx){

   unsigned short *a, *b, *c, *out;
   
   a = new unsigned short[DATA_SIZE];
   b = new unsigned short[DATA_SIZE];
   c = new unsigned short[DATA_SIZE];
   out = new unsigned short[DATA_SIZE];

   for (int k = 0; k < DATA_SIZE; ++k) {
      a[k] = k;
      b[k] = k;
      c[k] = k;
      out[k] = 0;
   }
   
   omp_set_num_threads(NUM_THREAD);
   high_resolution_clock::time_point s;
   duration<double> diff = {};

   for(int i=0;i<100;i++){
   	s = high_resolution_clock::now();
#pragma omp parallel
#pragma omp for 
   	for (int k = 0; k < DATA_SIZE; ++k) {
      	    out[k] = ((a[k]*(a[k]*(a[k-432])+62208)-2985984)*b[k]*(a[k]*(78*a[k]-9504)+b[k]*(288*a[k]-5184))+c[k]*b[k]*(a[k]*(a[k]-207)+3456-b[k]*(2*a[k]-c[k]+144)))*c[k];
  	}
   	diff += high_resolution_clock::now() - s;
   }
   double cpuExecTime = (diff.count() * 1000)/100; 

   printf("Time(ms) %d Thread: %5.2lf\n",NUM_THREAD,cpuExecTime);

   int v = out[idx];  

   delete a;
   delete b;
   delete c;
   delete out;

  return v;
}


