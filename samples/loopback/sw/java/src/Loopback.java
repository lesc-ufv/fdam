public class Loopback{

   public static void main(String argv[]){
   
       AccManagement accMgr = new AccManagement();
       Accelerator acc = accMgr.getAccelerator(0);
/*       
       int n = 10000;
       
       int in[] = new int[n];
       int out[] = new int[n];
       
       for(int i = 0;i < n;i++){
          in[i] = i;
          out[i] = 0;
       }
       acc.createInputQueue(0,sizeof(int)*n,in);
       acc.createOutputQueue(0,sizeof(int)*n);
       
       acc.start();
       acc.waitDone(0);*/
       
       acc.printInfo();
//        acc.copyFromOutputQueue(0,out,sizeof(int)*n);
//        
//        for(int i = 0;i < n;i++){
//           if(in[i] != out[i]){
//              System.out.println("Loopback error at " + i + " position, expect " + in[i] + " found " + out[i]);
//              break;
//          }
//        }
//     
       accMgr.printHwInfo();
       accMgr.delete();          
   }

}
