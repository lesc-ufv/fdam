//
// Created by lucas on 12/28/17.
//

#ifndef SW_AFU_DEFS_H
#define SW_AFU_DEFS_H

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

typedef int subafu_id;

typedef class AFU Afu;

typedef enum CSR_WR {
    CFG_REG,
    ADDR_WORKSPACE_BASE,
    WORKSPACE_SIZE,
    START_INTERFACES,
    STOP_INTERFACES,
    RST_INTERFACES,
    RST_BUFFER_INDEX
} CSR_WR;
typedef enum CSR_RD {
    CLOCK_COUNT,
    CL_WR_COUNT,
    CL_RD_COUNT,
    AFU_CONTROLLER_STATUS,
    INF_1,
    INF_2,
    INF_3,
    INF_4,
    INF_5,
    INF_6,
    INF_7,
    INF_8
} CSR_RD;

#endif //SW_AFU_DEFS_H
