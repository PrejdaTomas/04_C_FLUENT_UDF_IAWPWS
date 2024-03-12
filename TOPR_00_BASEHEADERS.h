#ifndef TOPR_00_BASEHEADERS
#define TOPR_00_BASEHEADERS
    #ifndef    _INC_STDIO
        #include <stdio.h>
    #endif

    #ifndef _INC_STDLIB
        #include <stdlib.h>
    #endif

    #ifndef _INC_STDARG
        #include <stdarg.h>
    #endif

    #ifndef _STDBOOL
        #include <stdbool.h>
    #endif

    #ifndef _INC_STRING
        #include <string.h>
    #endif

    #ifndef _USE_MATH_DEFINES
        #include <math.h>
    #endif

    #ifndef _INC_TIME
        #include <time.h>
    #endif

    #define BUFFER_SIZE 65536 //65536
    #define BUFFER_SIZE_STRING 512 //512
    #define ITER_PRINT_FREQUENCY 50
    #define COMPARE_THRESHOLD 0.000001
    #define SEARCH_THRESHOLD 0.01
    #define BENCHMARK_TRIES 1000

#endif // TOPR_00_BASEHEADERS  