#ifndef TOPR_06_ARRAY
#define TOPR_06_ARRAY
    #include "TOPR_00_BASEHEADERS.h"
    #include "TOPR_01_CONSTANTS.h"
    #include "TOPR_02_ENUMS.h"
    #include "TOPR_03_PRINTING.h"
    #include "TOPR_04_CONVENIENCE.h"
    #include "TOPR_05_FILEINFO.h"
    typedef struct
    {
        double	*array;
        int		count;
        int		last;
    } Array1D;

    typedef struct
    {
        double	**array;
        char	**headers;
        int		rows;
        int 	lastRow;
        int 	columns;
        int 	lastColumn;
        int 	count;
    } Array2D;


    Array1D createArrayStruct1D(int ROWS);
    Array2D createArrayStruct2D(int ROWS, int COLUMNS);

#endif // TOPR_06_ARRAY   
