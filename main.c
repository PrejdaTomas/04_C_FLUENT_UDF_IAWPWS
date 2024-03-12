#include "TOPR_13_ARRAY_INTERPOLATION.h"

int Main()
{
    debugLevel = MAIN;
    Array2D readTable = readFromCSV("table.csv");
    Array1D inputs = createRandom1D(450.0, 550.0, 100);
    Array1D temps = createRandom1D(400.0, 600.0, 100);
    Array1D press = createRandom1D(4.5, 6.0, 100);

    sortAscendingStruct1D(inputs);
    sortAscendingStruct1D(temps);
    sortAscendingStruct1D(press);

    Array1D lerped1D = lerp1D(inputs, temps, press);
    return 1;
}