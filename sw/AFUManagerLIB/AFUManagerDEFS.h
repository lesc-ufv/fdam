//
// Created by lucas on 12/28/17.
//

#ifndef SW_AFU_MANAGER_DEFS_H
#define SW_AFU_MANAGER_DEFS_H

#define AFU_INF_SIZE 64

#define RED          31
#define GREEN        32
#define YELLOW       33
#define BLUE         34

#define MSG(x) cout << "  [APP]  " << x << endl
#define BEGIN_COLOR(c) cout << "\033[1;"<<c<<"m";
#define END_COLOR()  cout << "\033[0;m";

#define GET_INDEX(ROW, COL, NUMCOL) (((ROW)*(NUMCOL))+ (COL))
#define MB(x) (x << 20) // the same as x*1024*1024
#define GB(x) (x << 30) // the same as x*1024*1024*1024

#define AFU_CONTROLLER_START (1L << 0)
#define AFU_CONTROLLER_STOP 0L
#define AFU_CONTROLLER_RESET (1L << 1)
#define AFU_CONTROLLER_UPDATE_WKP (1L << 2)

typedef unsigned int afu_id;
typedef class AFUManager AFUManager_t;

typedef enum CSR_WR {
    REG_CFG,
    REG_ADDR_WORKSPACE_BASE,
    REG_WORKSPACE_SIZE,
    REG_START_AFUs,
    REG_STOP_AFUs,
    REG_RESET_AFUs,
    REG_INDEX_BUFFER_RESET
} CSR_WR;
typedef enum CSR_RD {
    REG_CLOCK_COUNT,
    REG_CL_WR_COUNT,
    REG_CL_RD_COUNT,
    REG_AFU_CONTROLLER_STATUS,
    REG_INF_1,
    REG_INF_2,
    REG_INF_3,
    REG_INF_4,
    REG_INF_5,
    REG_INF_6,
    REG_INF_7,
    REG_INF_8
} CSR_RD;

#endif //SW_AFU_MANAGER_DEFS_H
