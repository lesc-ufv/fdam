#include <main.h>

int main(int argc, char *argv[]) {
    
    high_resolution_clock::time_point s;
    duration<double> diff{};
    s = high_resolution_clock::now();
    
    int n = 100000;
    
    if(argc > 1 ){
       n = atoi(argv[1])*16;
    } 
    else{
        cout << "invalid args!!!"<<endl;
        cout << "usage: <num lines>" << endl;
        exit(255);
    }
   auto *accMgr = new AccManagement();

   Accelerator &acc = accMgr->getAccelerator(0);
   
   auto in = (int *) malloc(sizeof(int)*n);
   auto out = (int *) malloc(sizeof(int)*n);

   for(int i = 0;i < n;i++){
       in[i] = static_cast<int>(i);
       out[i] = 0;
   }
   
   acc.createInputQueue(0,sizeof(int)*n);
   acc.createOutputQueue(0,sizeof(int)*n);
   acc.copyToInputQueue(0,in,n);
   
   high_resolution_clock::time_point s1;
   duration<double> diff1{};
   s1 = high_resolution_clock::now();
   
   acc.start();
   acc.waitDone(0);
   
   diff1 = high_resolution_clock::now() - s1;
   
   acc.copyFromOutputQueue(0,out,n);
   acc.printHwInfo();  
   accMgr->printHwInfo();
   
   for(int i = 0;i < n;i++){
       if(in[i] != out[i]){
          MSG("Loopback error at " << i << " position, expect "<< in[i] << " found "<<out[i]);
          break;   
       }
   }
   
   diff = high_resolution_clock::now() - s;
   double timeExec  = diff.count();
   double timeExec1 = diff1.count();
   double nBytes = (n+n) * sizeof(int);
   double nGbytes = nBytes / (1 << 30);
   double thpt = nGbytes / timeExec1;
   MSG("Total Time: " << timeExec * 1000 << "ms");
   MSG("Accelerator Time: " << timeExec1 * 1000 << "ms");
   MSG("Throughput: " << thpt << "GB/s");
   
   delete accMgr;
}
