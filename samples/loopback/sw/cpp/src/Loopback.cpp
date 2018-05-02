#include <AccManagement.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

int main(int argc, char *argv[]) {
    
    high_resolution_clock::time_point s;
    duration<double> diff{};
    s = high_resolution_clock::now();
    
    int n = 100000;
    
    if(argc > 1 ){
       n = atoi(argv[1]);
    } 
    else{
        cout << "invalid args!!!"<<endl;
        cout << "usage: <num elements>" << endl;
        exit(255);
    }
   auto *accMgr = new AccManagement();

   Accelerator &acc = accMgr->getAccelerator(0);
   
   auto in = (uint32_t *) malloc(sizeof(uint32_t)*n);
   auto out = (uint32_t *) malloc(sizeof(uint32_t)*n);

   for(int i = 0;i < n;i++){
       in[i] = static_cast<uint32_t>(i);
       out[i] = 0;
   }
   
   acc.createInputQueue(0,sizeof(uint32_t)*n,in);
   acc.createOutputQueue(0,sizeof(uint32_t)*n);

   high_resolution_clock::time_point s1;
   duration<double> diff1{};
   s1 = high_resolution_clock::now();
   
   acc.start();
   acc.waitDone(0);
   
   diff1 = high_resolution_clock::now() - s1;
   
   acc.printInfo();

   acc.copyFromOutputQueue(0,out,sizeof(uint32_t)*n);
 
   for(int i = 0;i < n;i++){
       if(in[i] != out[i]){
          MSG("Loopback error at " << i << " position, expect "<< in[i] << " found "<<out[i]);
          break;   
       }
   }
    
   accMgr->printHwInfo();
   
   diff = high_resolution_clock::now() - s;
   double timeExec  = diff.count();
   double timeExec1 = diff1.count();
   double nBytes = (n+n) * sizeof(uint32_t);
   double nGbytes = nBytes / (1 << 30);
   double thpt = nGbytes / timeExec1;
   MSG("Total Time: " << timeExec * 1000 << "ms");
   MSG("Accelerator Time: " << timeExec1 * 1000 << "ms");
   MSG("Throughput: " << thpt << "GB/s");
   

   delete accMgr;

}

