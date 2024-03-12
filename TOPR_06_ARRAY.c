#include "TOPR_06_ARRAY.h"

Array1D createArrayStruct1D(int ROWS)
{
	fprintfWrapper_newline_head("createArrayStruct1D: start -> %d elements!", ROWS);
	Array1D arrStruct;
	int index;
	arrStruct.array = (double*)malloc(ROWS * sizeof(double));
	arrStruct.count = ROWS;
	arrStruct.last 	= arrStruct.count - 1;
	for (index = 0; index < arrStruct.count; index ++) arrStruct.array[index] = -3.14159;
	fprintfWrapper_newline_tail("createArrayStruct1D: success!");
	return arrStruct;
}

Array2D createArrayStruct2D(int ROWS, int COLUMNS)
{
	fprintfWrapper_newline_head("createArrayStruct2D: start -> %d ROWS and %d COLUMNS!", ROWS, COLUMNS);
	Array2D arrStruct;
	int index1, index2;
	arrStruct.array			= (double**)malloc(ROWS * sizeof(double*));
	arrStruct.rows			= ROWS;
	arrStruct.lastRow		= ROWS-1;
	arrStruct.columns		= COLUMNS;
	arrStruct.lastColumn	= COLUMNS-1;
	arrStruct.count			= ROWS;
	arrStruct.headers 		= NULL;

	fprintfWrapper_newline_mid_upper("createArrayStruct2D: filling the array with -pis!");
	for (index1 = 0; index1 < arrStruct.rows; index1++)
	{
		arrStruct.array[index1] = (double*)malloc(arrStruct.columns*sizeof(double));
		for (index2 = 0; index2 < arrStruct.columns; index2++ ) arrStruct.array[index1][index2] = -3.14159;
	}
	fprintfWrapper_newline_tail("createArrayStruct2D: success! -> rows: %d, columns: %d, lastRow: %d, lastColumn: %d, count: %d", 
	arrStruct.rows, arrStruct.columns,
	arrStruct.lastRow, arrStruct.lastColumn,
	arrStruct.count	
	);
	return arrStruct;
}