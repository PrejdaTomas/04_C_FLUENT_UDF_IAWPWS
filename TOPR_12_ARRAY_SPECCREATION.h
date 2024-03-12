#ifndef TOPR_12_ARRAY_SPECCREATION
#define TOPR_12_ARRAY_SPECCREATION
    #include "TOPR_11_ARRAY_SEARCHING.h"
    Array1D extractColumnFromTable(Array2D arrayIpt, int selectedColumn);
    Array1D createUniqueValuesArray1D(Array1D arrayIpt);
    Array1D differences1D(double value, Array1D arrayIpt);
    Array1D createRandom1D(double hiBound, double loBound, int Count);
    Array1D lerp1D(Array1D inputVals, Array1D independentVals, Array1D dependentVals);

    Array2D getRectangle(int index, Array2D arrayIpt, int yUnique);
    Array2D readFromCSV(char *filePath); 
#endif // TOPR_12_ARRAY_SPECCREATION

