// file = 0; split type = patterns; threshold = 100000; total count = 0.
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "rmapats.h"

scalar dummyScalar;
scalar fScalarIsForced=0;
scalar fScalarIsReleased=0;
scalar fScalarHasChanged=0;
scalar fForceFromNonRoot=0;
scalar fNettypeIsForced=0;
scalar fNettypeIsReleased=0;
void  hsG_0 (struct dummyq_struct * I1094, EBLK  * I1095, U  I651);
void  hsG_0 (struct dummyq_struct * I1094, EBLK  * I1095, U  I651)
{
    U  I1338;
    U  I1339;
    U  I1340;
    struct futq * I1341;
    I1338 = ((U )vcs_clocks) + I651;
    I1340 = I1338 & ((1 << fHashTableSize) - 1);
    I1095->I697 = (EBLK  *)(-1);
    I1095->I701 = I1338;
    if (I1338 < (U )vcs_clocks) {
        I1339 = ((U  *)&vcs_clocks)[1];
        sched_millenium(I1094, I1095, I1339 + 1, I1338);
    }
    else if ((peblkFutQ1Head != ((void *)0)) && (I651 == 1)) {
        I1095->I702 = (struct eblk *)peblkFutQ1Tail;
        peblkFutQ1Tail->I697 = I1095;
        peblkFutQ1Tail = I1095;
    }
    else if ((I1341 = I1094->I1054[I1340].I714)) {
        I1095->I702 = (struct eblk *)I1341->I713;
        I1341->I713->I697 = (RP )I1095;
        I1341->I713 = (RmaEblk  *)I1095;
    }
    else {
        sched_hsopt(I1094, I1095, I1338);
    }
}
#ifdef __cplusplus
extern "C" {
#endif
void SinitHsimPats(void);
#ifdef __cplusplus
}
#endif
