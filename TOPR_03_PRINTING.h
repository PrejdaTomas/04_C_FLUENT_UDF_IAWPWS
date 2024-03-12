#ifndef TOPR_04_PRINTING
#define TOPR_04_PRINTING
    #include "TOPR_00_BASEHEADERS.h"
    #include "TOPR_01_CONSTANTS.h"
    #include "TOPR_02_ENUMS.h"
    void fprintfWrapper(const char* format, va_list args);
    void fprintfWrapper_newline(const char* format, ...);
    void fprintfWrapper_newline_head(const char* format, ...);
    void fprintfWrapper_newline_mid_upper(const char* format, ...);
    void fprintfWrapper_newline_mid_lower(const char* format, ...);
    void fprintfWrapper_newline_tail(const char* format, ...);
    void fprintfWrapper_newline_MAIN(const char* format, ...);
    void fprintfWrapper_comma(const char* format, ...);
    void newline(int count);

    void printArrayNative1D_string(char **arrayIpt, int length);
    void printArrayNative1D_double(double *arrayIpt, int length);
    int debugLevel = MAIN;
#endif // TOPR_04_PRINTING