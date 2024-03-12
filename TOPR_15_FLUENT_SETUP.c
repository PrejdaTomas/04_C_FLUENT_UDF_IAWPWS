#include "TOPR_15_FLUENT_SETUP.h"
DEFINE_EXECUTE_ON_LOADING (read_IAPWS_table, libname)
{
    readTable = readFromCSV("table.csv");
    Array1D temperatures = extractColumnFromTable(readTable, T);
    temperatureTable  = createUniqueValuesArray1D(temperatures);
    free(temperatures.array);
    free(temperatures.count);
    free(temperatures.last);
    free(temperatures);
    
    Array1D pressures = extractColumnFromTable(readTable, P);
    pressureTable = createUniqueValuesArray1D(pressures); 
    free(pressures.array);
    free(pressures.count)
    free(pressures.last)
    free(pressures)
}