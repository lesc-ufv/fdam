#include <main.h>

int main(int argc, char *argv[]) {
    
   auto *accMgr = new AccManagement();
   accMgr->printHwInfo();

   delete accMgr;

}

