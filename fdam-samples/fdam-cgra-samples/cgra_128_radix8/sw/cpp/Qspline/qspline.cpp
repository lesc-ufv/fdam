#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <omp.h>

using namespace std;
using namespace std::chrono;

#define DATA_SIZE (1 << 23)
#define NUM_THREAD 8

int qspline(int idx);
int qspline_openmp(int idx);

int main(int argc, char *argv[]){
      
     int idx = 0;
     if(argc > 1)
        idx = atoi(argv[1]);
   
     qspline(idx);
     qspline_openmp(idx);
     
     return 0;
}

int qspline(int idx){

   unsigned short *a, *b, *c, *d, *e, *f, *g, *out;
   
   a = new unsigned short[DATA_SIZE];
   b = new unsigned short[DATA_SIZE];
   c = new unsigned short[DATA_SIZE];
   d = new unsigned short[DATA_SIZE];
   e = new unsigned short[DATA_SIZE];
   f = new unsigned short[DATA_SIZE];
   g = new unsigned short[DATA_SIZE];
   out = new unsigned short[DATA_SIZE];

   for (int k = 0; k < DATA_SIZE; ++k) {
      a[k] = k % 1024;
      b[k] = k % 1024;
      c[k] = k % 1024;
      d[k] = k % 1024;
      e[k] = k % 1024;
      f[k] = k % 1024;
      g[k] = k % 1024;
      out[k] = 0;
   }   

   high_resolution_clock::time_point s;
   duration<double> diff = {};
   for(int i=0;i<100;i++){
      s = high_resolution_clock::now();
      for (int k = 0; k < DATA_SIZE; ++k) {
         out[k] = b[k]*a[k]*b[k]*b[k]*b[k]+b[k]*b[k]+b[k]*f[k]*6*c[k]*b[k]*f[k]+e[k]*f[k]*f[k]*f[k]+b[k]*f[k]*f[k]*4*g[k]*f[k];
      }
      diff += high_resolution_clock::now() - s;
   }
   double cpuExecTime = (diff.count() * 1000)/100; 

   printf("Time(ms) 1 Thread: %5.2lf\n",cpuExecTime);
   
   int v = out[idx];  

   delete a;
   delete b;
   delete c;
   delete d;
   delete e;
   delete f;
   delete g;
   delete out;

  return v;
  
}

int qspline_openmp(int idx){

   unsigned short *a, *b, *c, *d, *e, *f, *g, *out;
   
   a = new unsigned short[DATA_SIZE];
   b = new unsigned short[DATA_SIZE];
   c = new unsigned short[DATA_SIZE];
   d = new unsigned short[DATA_SIZE];
   e = new unsigned short[DATA_SIZE];
   f = new unsigned short[DATA_SIZE];
   g = new unsigned short[DATA_SIZE];
   out = new unsigned short[DATA_SIZE];

   for (int k = 0; k < DATA_SIZE; ++k) {
      a[k] = k % 1024;
      b[k] = k % 1024;
      c[k] = k % 1024;
      d[k] = k % 1024;
      e[k] = k % 1024;
      f[k] = k % 1024;
      g[k] = k % 1024;
      out[k] = 0;
   }    
   
   high_resolution_clock::time_point s;
   duration<double> diff = {};
   omp_set_num_threads(NUM_THREAD);

   for(int i=0;i<100;i++){
      s = high_resolution_clock::now();
#pragma omp parallel
#pragma omp for 
      for (int k = 0; k < DATA_SIZE; ++k) {
         out[k] = b[k]*a[k]*b[k]*b[k]*b[k]+b[k]*b[k]+b[k]*f[k]*6*c[k]*b[k]*f[k]+e[k]*f[k]*f[k]*f[k]+b[k]*f[k]*f[k]*4*g[k]*f[k];
      }
      diff += high_resolution_clock::now() - s;
   }
   double cpuExecTime = (diff.count() * 1000)/100; 

   printf("Time(ms) %d Thread: %5.2lf\n",NUM_THREAD,cpuExecTime);

   int v = out[idx];  

   delete a;
   delete b;
   delete c;
   delete d;
   delete e;
   delete f;
   delete g;
   delete out;

  return v;
}


