#ifndef TOPR_16_FLUENT_UDRGM
#define TOPR_16_FLUENT_UDRGM
    #include "TOPR_15_FLEUNT_SETUP.h"
    UDF_EXPORT RGAS_Functions RealGasFunctionList =
    {
        IAPWS_INITIALIZER,                 /* initialize           */
        IAPWS_density,               /* density              */
        IAPWS_enthalpy,              /* enthalpy             */
        IAPWS_entropy,               /* entropy              */
        IAPWS_specific_heat,         /* specific_heat        */
        IAPWS_mw,                    /* molecular_weight     */
        IAPWS_speed_of_sound,        /* speed_of_sound       */
        IAPWS_viscosity,             /* viscosity            */
        IAPWS_thermal_conductivity,  /* thermal_conductivity */
        IAPWS_rho_t,                 /* drho/dT |const p     */
        IAPWS_rho_p,                 /* drho/dp |const T     */
        IAPWS_enthalpy_t,            /* dh/dT |const p       */
        IAPWS_enthalpy_p             /* dh/dp |const T       */
    };
#endif // TOPR_16_FLUENT_UDRGM
