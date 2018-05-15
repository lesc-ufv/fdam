#include <fam/AccManagement.h>

using namespace std;

int main(int argc, char *argv[]) {
    
   auto *accMgr = new AccManagement();
   accMgr->printHwInfo();

   delete accMgr;

}

