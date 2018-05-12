public class Main{

   public static void main(String args[]){
   
       if(args.length < 1){
          System.out.println("invalid args!!!");
          System.out.println("usage: <num elements>");
          return;
       }
       
       AccManagement accMgr = new AccManagement();
       Accelerator acc = accMgr.getAccelerator(0);
              
       int n = Integer.parseInt(args[0])*16;        
       int in[] = new int[n];
       int out[] = new int[n];
       
       for(int i = 0;i < n;i++){
          in[i] = i;
          out[i] = 0;
       }
       acc.createInputQueue((short)0,in.length*4);
       acc.createOutputQueue((short)0,out.length*4);          
       acc.copyToInputQueue((short)0,in,n);      
       
       acc.start();
       acc.waitDone(0);       
       
       acc.copyFromOutputQueue((short)0,out,n);
       
       for(int i = 0;i < n;i++){
          if(in[i] != out[i]){
             System.out.println("[APP]   Loopback error at " + i + " position, expect " + in[i] + " found " + out[i]);
             break;
         }
       }
       
       acc.printHwInfo();
       accMgr.printHwInfo();
       accMgr.delete();          
   }

}
