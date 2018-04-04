//
// Created by lucas on 12/28/17.
//
#ifndef DATAFLOW_EXEC_H
#define DATAFLOW_EXEC_H

#include <chrono>
#include <AccManagement.h>

using namespace std;
using namespace std::chrono;

double dataflow_exec(int ** constants, int num_constants ,uint16_t **data_in, int num_data_in,uint16_t **data_out, int num_data_out, int num_copies, bool printAccStatus);



#endif //DATAFLOW_EXEC_H
