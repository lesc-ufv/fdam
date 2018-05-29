#include <main.h>
//#define DEBUG

int main(int argc, char *argv[]) {
    
  uint32_t* data_in;
  uint32_t* data_out_sw;
  uint32_t* data_out_hw;
  bool flag_error=false;
  
  data_in  = (uint32_t *) malloc (NI*sizeof(uint32_t));
  data_out_sw = (uint32_t *) malloc (NJ*sizeof(uint32_t));
  data_out_hw = (uint32_t *) malloc (NJ*sizeof(uint32_t));
  
  for (uint64_t i = 0; i < NI; i++)
  {
    data_in[i] = i;
  }
  
  high_resolution_clock::time_point s;
  duration<double> diff{};
  s = high_resolution_clock::now();    
  
  md5_sw(data_in,NI,data_out_sw,NJ);   
  
  diff = high_resolution_clock::now() - s;
  double timeExecSw  = diff.count();
  
  s = high_resolution_clock::now(); 
  
  md5_hw(data_in,NI,data_out_hw,NJ); 
  
  diff = high_resolution_clock::now() - s;
  double timeExecHw  = diff.count();
  
  for (uint64_t i = 0; i < NJ; i += 4) {
      
        if((data_out_sw[i + 0] & 0xff) != (data_out_hw[i + 0] & 0xff)){
            flag_error = true;
            break;
        }
        if((data_out_sw[i + 1] & 0xff) != (data_out_hw[i + 1] & 0xff)){
            flag_error = true;
            break;
        }
        if((data_out_sw[i + 2] & 0xff) != (data_out_hw[i + 2] & 0xff)){
            flag_error = true;
            break;
        }
        if((data_out_sw[i + 3] & 0xff) != (data_out_hw[i + 3] & 0xff)){
            flag_error = true;
            break;
        }
#ifdef DEBUG
        printf("md5_sw[%02lu] = %08x %08x %08x %08x\n", i/4, 
            data_out_sw[i + 3] & 0xff, 
            data_out_sw[i + 2] & 0xff, 
            data_out_sw[i + 1] & 0xff, 
            data_out_sw[i + 0] & 0xff);
        printf("md5_hw[%02lu] = %08x %08x %08x %08x\n", i/4, 
            data_out_hw[i + 3] & 0xff, 
            data_out_hw[i + 2] & 0xff, 
            data_out_hw[i + 1] & 0xff, 
            data_out_hw[i + 0] & 0xff);
#endif // DEBUG
  }
  
  if(flag_error){
     MSG("MD5 ERROR!");
  }else{
    MSG("MD5 PASS!");    
  }
  
  MSG("MD5 software time: " << timeExecSw * 1000 << "ms");
  MSG("MD5 hardware time: " << timeExecHw * 1000 << "ms");

  free(data_in);
  free(data_out_sw);
  free(data_out_hw);
  
  return 0;

}
void md5_sw(const WORD32 *message, long len_message, WORD32 *output,long len_output){

  #pragma omp target device(HARPSIM) map(to: message[:len_message]) map(from: output[:len_output])
  #pragma omp parallel for use(hrw) module(md5)
  for (uint64_t i = 0; i < len_message; i += 16)
  {
    output[i/4] = message[i];
    md5((const uint32_t*) message + i, 64, (uint32_t *) output + i/4);
  }

}
void md5_hw(const WORD32 *message, long len_message, WORD32 *output,long len_output){

   auto *accMgr = new AccManagement();
   Accelerator &acc = accMgr->getAccelerator(0);   
   acc.createInputQueue(0,sizeof(WORD32)*len_message);
   acc.createOutputQueue(0,sizeof(WORD32)*len_output);
   acc.copyToInputQueue(0,(int*)message,len_message);
   acc.start();
   acc.waitDone(0);
   acc.copyFromOutputQueue(0,(int *)output,len_output);
   acc.printHwInfo();  
   accMgr->printHwInfo();   
   delete accMgr;
    
}

