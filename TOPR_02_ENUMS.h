#ifndef TOPR_03_ENUMS
#define TOPR_03_ENUMS
    enum debugLevelSelected
    {
        NONE = 0,
        MAIN = 1,
        MainAndFuncbounds = 2,
        MainAndFuncboundsAndUpper = 3,
        MainAndFuncboundsAndUpperAndLower = 4
    };

    enum tableColumn
    {
        T = 0,
        P = 1,
        RHO = 2,
        MU = 3,
        CP = 4,
        K = 5,
        HREL = 6,
        SREL = 7
    };

    enum rectPosition
    {
        TL = 0,
        TR = 1,
        BL = 2,
        BR = 3
    };

#endif // TOPR_03_ENUMS