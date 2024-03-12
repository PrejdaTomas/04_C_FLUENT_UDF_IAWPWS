#ifndef TOPR_3_CONVENIENCE
#define TOPR_3_CONVENIENCE
    #include "TOPR_00_BASEHEADERS.h"
    #include "TOPR_02_ENUMS.h"
    #include "TOPR_03_PRINTING.h"

    double  timer(time_t start, time_t end);
    char *strConcat(const char *s1, const char *s2);
    double absDiff(double value1, double value2);
    double absError(double value1, double value2);
    double drand(double loBound, double hiBound);

    double *ardrand(double loBound, double hiBound, int Count);

    bool compare(double compared, double reference);
    void copyArray1DNative_toArray1DNative(double *source, double *target, int count);
#endif // TOPR_3_CONVENIENCE