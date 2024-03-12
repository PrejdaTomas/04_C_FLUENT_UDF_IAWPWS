#include "TOPR_08_ARRAY_PRINTING.h"
void printArrayStruct1D(Array1D arrayIpt)
{
	if (debugLevel >= MainAndFuncboundsAndUpper)
	{
		fprintfWrapper_newline_head("printArrayStruct1D: start!");
		unsigned int index;
		printf("[");
		for (index = 0; index < arrayIpt.count; index++)
		{
			printf("%lf, ", arrayIpt.array[index]);
		}
		printf("]\n");
		fflush(stdout);
		fprintfWrapper_newline_tail("printArrayStruct1D: success!");
	}
}

void printArrayStruct2D(Array2D arrayIpt)
{
	if (debugLevel >= MainAndFuncboundsAndUpper)
	{
		fprintfWrapper_newline_head("printArrayStruct2D: start!");
		unsigned int indexRow, indexCol;
		printf("[\n");
		for (indexRow = 0; indexRow < arrayIpt.rows; indexRow++)
		{
			printf("\t[");
			for (indexCol = 0; indexCol < arrayIpt.columns; indexCol++)
			{
				printf("%lf, ", arrayIpt.array[indexRow][indexCol]);
			}
			printf("]\n");
		}
		printf("]\n");
		fprintfWrapper_newline_tail("printfArray2D: success!");
		fflush(stdout);
	}
}