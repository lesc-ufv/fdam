#include <main.h>

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
        cout << "usage: <num words>" << endl;
        exit(255);
    }
   auto accMgr = new AccManagement();

   Accelerator &acc = accMgr->getAccelerator(0);
   
   auto a = (int *) malloc(sizeof(int)*n);
   auto b = (int *) malloc(sizeof(int)*n);
   auto c = (int *) malloc(sizeof(int)*n);

   for(int i = 0;i < n;i++){
       a[i] = static_cast<int>(i);
       b[i] = static_cast<int>(i);
       c[i] = 0;
   }

   acc.createInputQueue(0,sizeof(int)*n);
   acc.createInputQueue(1,sizeof(int)*n);
   acc.createOutputQueue(0,sizeof(int)*n);
   
   acc.copyToInputQueue(0,a,n);
   acc.copyToInputQueue(1,b,n);

   
   high_resolution_clock::time_point s1;
   duration<double> diff1{};
   s1 = high_resolution_clock::now();
   
   acc.start();
   acc.waitDone(0);
   
   diff1 = high_resolution_clock::now() - s1;
   
   acc.copyFromOutputQueue(0,c,n);
   
   acc.printHwInfo();  
   accMgr->printHwInfo();
   
   long idx_error = -1;
   for(int i = 0;i < n;i++){
       if(a[i]+b[i] != c[i]){
         idx_error = i;
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
   if(idx_error >= 0){
      MSG("Sum error at " << idx_error<< " position, expect "<< a[idx_error]+b[idx_error] << " found "<<c[idx_error]);
   }else{
      MSG("Sum pass!");       
   }
   
   free(a);
   free(b);
   free(c);
   delete accMgr;
}
