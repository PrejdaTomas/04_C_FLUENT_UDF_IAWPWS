#include "TOPR_09_ARRAY_SORTING.h"

void sortAscendingStruct1D(Array1D arrayInput)
{
	fprintfWrapper_newline_head("sortAscendingStruct1D: start!");
	double *arrayNativeTmp = (double*)malloc(sizeof(double)*arrayInput.count);
	copyArray1Dstruct_toArray1DNative(&arrayInput, arrayNativeTmp);
	if (debugLevel >= MainAndFuncboundsAndUpper) {printArrayNative1D_double(arrayNativeTmp, arrayInput.count);}

	quickSort(arrayNativeTmp, arrayInput.count);

	copyArray1DNative_toArray1DStruct(arrayNativeTmp, &arrayInput);
	if (debugLevel >= MainAndFuncboundsAndUpper) {printArrayStruct1D(arrayInput);}
	/*free(arrayNativeTmp);
	fprintfWrapper_newline_mid_upper("sourtAscendingStruct1D: freed the temporary array");*/
	fprintfWrapper_newline_tail("sortAscendingStruct1D: success!");
}