#include "fir.h"


int main(int argc, char *argv[]){
      
     int idx = 0;
     int test = 0;
     
    if(argc > 1)
        test = atoi(argv[1]);
     
     if(argc > 2)
        idx = atoi(argv[2]);
     
     unsigned short *coef;
     coef = new unsigned short[TAPS];
     
     for (int k = 0; k < TAPS; ++k){
      coef[k] = k+1;
     } 
     if(test & 1)
        fir(idx,coef,TAPS);
     if(test & 2)
        fir_openmp(idx,coef,TAPS);
     if(test & 4)
        fir_cgra(idx,1,coef,TAPS);
     
     delete coef;
     
     return 0;
}

int fir(int idx, unsigned short *coef, int taps){

   unsigned short *data_in, *data_out;

   data_in = new unsigned short[DATA_SIZE+taps];
   data_out = new unsigned short[DATA_SIZE+taps];


   for (int k = 0; k < DATA_SIZE; ++k){
      data_in[k] = k;
      data_out[k] = 0;
   }
   
   high_resolution_clock::time_point s;
   duration<double> diff = {};
   
   for(int i = 0; i < SAMPLES;i++){
      s = high_resolution_clock::now();
      for (int j = 0; j < DATA_SIZE; j++){
         unsigned short fir = 0;
         for (int k = 0; k < taps; ++k) {
             fir += data_in[j + k] * coef[taps - 1 - k];
         }
         data_out[j] = fir;
      }
      diff += high_resolution_clock::now() - s;
   }
   
   double cpuExecTime = (diff.count() * 1000)/SAMPLES;

   printf("Time(ms) CPU 1 Thread: %5.2lf\n",cpuExecTime);
   
   int v = data_out[idx];  

   delete data_in;
   delete data_out;

   return v;

}

int fir_openmp(int idx, unsigned short *coef, int taps){
    
   unsigned short *data_in, *data_out;
   
   data_in = new unsigned short[DATA_SIZE + taps];
   data_out = new unsigned short[DATA_SIZE + taps];
 
   for (int k = 0; k < DATA_SIZE; ++k){
      data_in[k] = k;
      data_out[k] = 0;
   }   
   
   omp_set_num_threads(NUM_THREAD);
   
   high_resolution_clock::time_point s;
   duration<double> diff = {};
   
   for(int i = 0; i < SAMPLES;i++){
      s = high_resolution_clock::now();
      #pragma omp parallel
      #pragma omp for 
      for (int j = 0; j < DATA_SIZE; j++){
         unsigned short fir = 0;
         for (int k = 0; k < taps; ++k) {
             fir += data_in[j + k] * coef[taps - 1 - k];
         }
         data_out[j] = fir;
      }
      diff += high_resolution_clock::now() - s;
   }
   
   double cpuExecTime = (diff.count() * 1000)/SAMPLES;

   printf("Time(ms) CPU %d Thread: %5.2lf\n",NUM_THREAD,cpuExecTime);

   int v = data_out[idx];  

   delete data_in;
   delete data_out;
   
   return v;
}

int fir_cgra(int idx, int copies, unsigned short *coef,int taps) {

    auto cgraArch = new CgraArch(0, 128, 8, 8, 8, 1, 2);
    auto cgraHw = new Cgra();
    Scheduler scheduler(cgraArch);
    std::vector<DataFlow *> dfs;

    unsigned short *data_in, *data_out;
    data_in = new unsigned short[DATA_SIZE];
    data_out = new unsigned short[DATA_SIZE];
    int r = 0, v = 0, tries = 0;

    for (int k = 0; k < DATA_SIZE; ++k) {
        data_in[k] = k;
        data_out[k] = 0;
    }

    for (int i = 0; i < NUM_THREAD; ++i) {
        dfs.push_back(createDataFlow(i, copies,coef,taps));
        scheduler.addDataFlow(dfs[i], i, 0);
        cgraArch->getNetBranch(i)->createRouteTable();
        cgraArch->getNet(i)->createRouteTable();
    }

    do {
        r = scheduler.scheduling();
        tries++;
    } while (r != SCHEDULE_SUCCESS && tries < 1000);

    if (r == SCHEDULE_SUCCESS) {

        cgraHw->loadCgraProgram(cgraArch->getCgraProgram());

        auto data_size = (size_t) (DATA_SIZE / ((NUM_THREAD) * copies));
        auto sf = (sizeof(unsigned short));

        int k = 0;
        for (int i = 0; i < NUM_THREAD; ++i) {
            for (int j = 0, c = 0; j < copies; ++j) {
                cgraHw->setCgraProgramInputStreamByID(i, c, &data_in[k * data_size], sf*data_size);
                cgraHw->setCgraProgramOutputStreamByID(i, c + 1, &data_out[k * data_size], sf*data_size);
                c = c + 2;
                k++;
            }
        }

        double cgraExecTime = 0;
        for (int i = 0; i < SAMPLES; i++){
            cgraHw->syncExecute(0);
            cgraExecTime += cgraHw->getTimeExec();
        }
        cgraExecTime /= SAMPLES;
        printf("Time(ms) CGRA: %5.2lf\n", cgraExecTime);
        v = data_out[idx];

    } else {
        printf("Scheduler Error: %d\n", r);
    }

    delete cgraArch;
    delete cgraHw;
    delete data_in;
    delete data_out;
    for (auto df:dfs) {
        delete df;
    }
    return v;
}

DataFlow *createDataFlow(int id, int copies, unsigned short *coef, int taps) {
    auto df = new DataFlow(id, "fir");
    int idx = 0;
    std::vector<Operator *> in_cp;
    std::vector<Operator *> out_cp;

    in_cp.reserve(copies);
    for (int j = 0; j < copies; ++j) {
        in_cp.push_back(new InputStream(idx++));
    }
    out_cp.reserve(copies);
    for (int j = 0; j < copies; ++j) {
        out_cp.push_back(new OutputStream(idx++));
    }
    for (int j = 0; j < copies; ++j) {
        Operator *op, *op1, *op2;
        std::vector<Operator *> add;
        add.reserve((unsigned long) taps - 1);
        for (int i = 0; i < taps; ++i) {
            auto m = new Multi(idx++, coef[taps - i - 1]);
            if (i == 0) {
                op = new PassA(idx++);
            } else {
                op = new Add(idx++);
            }
            add.push_back(op);
            df->connect(in_cp[j], m, m->getPortA());
            df->connect(m, op, m->getPortA());
        }
        for (int i = 0; i < taps - 1; ++i) {
            op1 = add[i];
            op2 = add[i + 1];
            df->connect(op1, op2, op2->getPortB());
        }
        op1 = add[taps - 1];
        df->connect(op1, out_cp[j], out_cp[j]->getPortA());
    }

    return df;

}
