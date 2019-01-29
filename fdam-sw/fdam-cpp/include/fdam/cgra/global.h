//
// Created by lucas on 23/01/19.
//

#ifndef MAIN_GLOBAL_H
#define MAIN_GLOBAL_H

#include <vector>
#include <cstdlib>

class Global {
private:
    static int idx;
public:
    static int getNextIdx();

    static void resetIndex();

    static int bitRotateLeft(int val, int r_bits, int max_bits);

    static int bitRotateRigth(int val, int r_bits, int max_bits);

};


#endif //MAIN_GLOBAL_H
