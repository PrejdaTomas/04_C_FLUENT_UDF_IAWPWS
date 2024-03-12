#include "TOPR_16A_FLUENT_MATERIALS.h"

DEFINE_PROPERTY (IAPWS_density, c, t)
{
    real T = C_T(c,t);
    real P = Pa_MPa(C_P(c,t) + RP_Get_Real("operating-pressure"));
    return (real)interpolation2D_bilinear(readTable, T, P, RHO, temperatureTable, pressureTable);
}

DEFINE_PROPERTY (IAPWS_thermalCapacity, c, t)
{
    real T = C_T(c,t);
    real P = Pa_MPa(C_P(c,t) + RP_Get_Real("operating-pressure"));
    return (real)interpolation2D_bilinear(readTable, T, P, CP, temperatureTable, pressureTable);   
}


DEFINE_PROPERTY (IAPWS_dynamicViscosity, c, t)
{
    real T = C_T(c,t);
    real P = Pa_MPa(C_P(c,t) + RP_Get_Real("operating-pressure"));
    return (real)interpolation2D_bilinear(readTable, T, P, MU, temperatureTable, pressureTable);
}


DEFINE_PROPERTY (IAPWS_IAPWS_thermalConductivity, c, t)
{
    real T = C_T(c,t);
    real P = Pa_MPa(C_P(c,t) + RP_Get_Real("operating-pressure"));
    return (real)interpolation2D_bilinear(readTable, T, P, K, temperatureTable, pressureTable);
}
