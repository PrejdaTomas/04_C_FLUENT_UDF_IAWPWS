#ifndef TOPR_13_ARRAY_INTERPOLATION
#define TOPR_13_ARRAY_INTERPOLATION
    #include "TOPR_12_ARRAY_SPECCREATION.h"
    double interpolation1D_linear(Array1D xValues, Array1D yValues, double x);
    double interpolation2D_bilinear(Array2D arrayIpt, double x, double y, enum tableColumn selectedColumn, Array1D uniqueX, Array1D uniqueY); 
#endif // TOPR_12_ARRAY_INTERPOLATION
