#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <omp.h>

using namespace std;
using namespace std::chrono;

#define DATA_SIZE (1 << 24)
#define NUM_THREAD 8

int poly6(int idx);
int poly6_openmp(int idx);

int main(int argc, char *argv[]){
      
     int idx = 0;
     if(argc > 1)
        idx = atoi(argv[1]);
   
     poly6(idx);
     poly6_openmp(idx);
     
     return 0;
}

int poly6(int idx){

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
      	out[k] = b[k]*(8*c[k]*(b[k]+216)+124416*c[k])+2985984*c[k]-a[k]*(b[k]*b[k]*c[k]*(6*b[k]+4*b[k]-432)-c[k]*(414*c[k]-20736)-c[k]*(3456*c[k]-1492992)-a[k]*((b[k]*b[k]*(b[k]*(c[k]-32)+c[k]*(c[k]-72))-c[k]*(87*c[k]+2592))+c[k]*(c[k]*(c[k]-864)-186624)));
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

int poly6_openmp(int idx){

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
      	out[k] = b[k]*(8*c[k]*(b[k]+216)+124416*c[k])+2985984*c[k]-a[k]*(b[k]*b[k]*c[k]*(6*b[k]+4*b[k]-432)-c[k]*(414*c[k]-20736)-c[k]*(3456*c[k]-1492992)-a[k]*((b[k]*b[k]*(b[k]*(c[k]-32)+c[k]*(c[k]-72))-c[k]*(87*c[k]+2592))+c[k]*(c[k]*(c[k]-864)-186624)));
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

