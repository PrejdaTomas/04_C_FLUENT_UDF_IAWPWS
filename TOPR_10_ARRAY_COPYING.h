#ifndef TOPR_10_ARRAY_COPYING
#define TOPR_10_ARRAY_COPYING
    #include "TOPR_06_ARRAY.h"    
    void copyArray1DNative_toArray1DStruct(double *source, Array1D *target); /*mohu tez poslat &pointer a pak target->count atp*/
    void copyArray1Dstruct_toArray1DNative(Array1D *source, double *target);
#endif // TOPR_10_ARRAY_COPYING
