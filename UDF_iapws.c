#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "udf.h"


#define BUFFER_SIZE			65536 //65536
#define BUFFER_SIZE_STRING		512 //512
#define ITER_PRINT_FREQUENCY		50
#define COMPARE_THRESHOLD		0.000001
#define SEARCH_THRESHOLD 		0.01
#define BENCHMARK_TRIES		 1000

enum debugLevelSelected {
	NONE = 0,
	Main = 1,
	MainAndFuncbounds = 2,
	MainAndFuncboundsAndUpper = 3,
	MainAndFuncboundsAndUpperAndLower = 4
};

enum tableColumn{
	T = 0,
	P = 1,
	RHO = 2,
	MU = 3,
	CP = 4,
	K = 5,
	HREL = 6,
	SREL = 7
};

enum rectPosition{
	TL = 0,
	TR = 1,
	BL = 2,
	BR = 3
};

typedef struct
{
	double	*array;
	int		count;
	int		last;
} Array1D;

typedef struct
{
	double	**array;
	char	**headers;
	int		rows;
	int 	lastRow;
	int 	columns;
	int 	lastColumn;
	int 	count;
} Array2D;


Array1D createArrayStruct1D(int ROWS);
Array1D extractColumnFromTable(Array2D arrayIpt, int selectedColumn);
Array1D createUniqueValuesArray1D(Array1D arrayIpt);
Array1D differences1D(double value, Array1D arrayIpt);

Array2D createArrayStruct2D(int ROWS, int COLUMNS);
Array2D getRectangle(int index, Array2D arrayIpt, int yUnique);
Array2D readFromCSV(char *filePath);


void fprintfWrapper(const char* format, va_list args);
void fprintfWrapper_newline(const char* format, ...);
void fprintfWrapper_newline_head(const char* format, ...);
void fprintfWrapper_newline_mid_upper(const char* format, ...);
void fprintfWrapper_newline_mid_lower(const char* format, ...);
void fprintfWrapper_newline_tail(const char* format, ...);
void fprintfWrapper_newline_MAIN(const char* format, ...);
void fprintfWrapper_comma(const char* format, ...);


void	swap(double* a, double* b);
int	partition(double arr[], int low, int high);
void	quickSort_inner(double arr[], int low, int high);
void	quickSort(double arr[], int count);

double interpolation2D_bilinear(Array2D arrayIpt, double x, double y, enum tableColumn selectedColumn, Array1D uniqueX, Array1D uniqueY);

int countLinesFile(char *filePath);
int countLinesFile_noHeader(char *filePath);
int countColumnsFile(char *filePath);
int countUniqueVals1D(Array1D arrayIpt);
int countNonUniqueVals1D(Array1D arrayIpt);


double	minimumStruct1D_value(Array1D arrayIpt);
int	minimumStruct1D_index(Array1D arrayIpt);
double	maximumStruct1D_value(Array1D arrayIpt);
int	maximumStruct1D_index(Array1D arrayIpt);
double	sumStruct1D_value(Array1D arrayIpt);
double	averageStruct1D_value(Array1D arrayIpt);
double  timer(time_t start, time_t end);


int findClosest1D_linear(double variable, Array1D arrayIpt);
int findClosest1D_efficient(double variable, Array1D arrayIpt);
int findClosest2D(double independentVariable1, double independentVariable2, Array2D arrayIpt);
int findClosest2D_efficient(double independentVariable1, double independentVariable2, Array1D uniqueList1, Array1D uniqueList2);

void printArrayNative1D_string(char **arrayIpt, int length);
void printArrayNative1D_double(double *arrayIpt, int length);

void printArrayStruct1D(Array1D arrayIpt);
void printArrayStruct2D(Array2D arrayIpt);
void newline(int count);
void copyArray1DNative_toArray1DNative(double *source, double *target, int count);
void copyArray1DNative_toArray1DStruct(double *source, Array1D *target); /*mohu tez poslat &pointer a pak target->count atp*/
void copyArray1Dstruct_toArray1DNative(Array1D *source, double *target);

void sortAscendingStruct1D(Array1D arrayInput);

/*
void sin2D(int countRow, int countCol, double **arrayIpt);
*/

char *strConcat(const char *s1, const char *s2);

bool compare(double compared, double reference);
bool valueInArray(double compared, Array1D arrayIpt);

double absDiff(double value1, double value2);
double absError(double value1, double value2);
double drand(double loBound, double hiBound);

const int debugLevel 	= Main;




int main()
{
	double total;
	int attempt;
	double tempK, pressMPa, bilerp;

	clock_t start, end;
	fprintfWrapper_newline_MAIN("start -> debugLevel = %d", debugLevel);

	char *tablefilePath		= "table.csv";

	total = 0;
	start = clock();
	Array2D readTable		= readFromCSV(tablefilePath);
	end = clock();
	fprintfWrapper_newline_MAIN("Finished reading the csv in the main loop: \t%lf!", timer(start, end));
	
	total = 0;
	start = clock();
	Array1D temperatures		= extractColumnFromTable(readTable, 0);
	sortAscendingStruct1D(temperatures);
	Array1D uniqueTemperatures	= createUniqueValuesArray1D(temperatures);
	end = clock();;
	fprintfWrapper_newline_MAIN("Finished creating the unique temperatures in the main loop: \t%lf!", timer(start, end));
	
	total = 0;
	start = clock();
	Array1D pressures		= extractColumnFromTable(readTable, 1);
	sortAscendingStruct1D(pressures);
	Array1D uniquePressures = createUniqueValuesArray1D(pressures);	
	end = clock();
	fprintfWrapper_newline_MAIN("Finished creating the unique pressures in the main loop: \t%lf!", timer(start, end));
	


	attempt = 0;
	total = 0;
	while (attempt < BENCHMARK_TRIES)
	{
		tempK = drand(uniqueTemperatures.array[0], uniqueTemperatures.array[uniqueTemperatures.last]);
		pressMPa = drand(uniquePressures.array[0], uniquePressures.array[uniquePressures.last]);
        	if (attempt % 50 == 0) {fprintfWrapper_newline_MAIN("%d: [%lf K, %lf MPa]", attempt, tempK, pressMPa);}

		start = clock();
		bilerp = interpolation2D_bilinear(readTable, tempK, pressMPa, RHO, uniqueTemperatures, uniquePressures);
		end = clock();
		total += timer(start, end);
		attempt ++;
	}
	total = total/(double)(BENCHMARK_TRIES);
	fprintfWrapper_newline_MAIN("Bilerp [%lf, %lf]: %lf\t%lfs", tempK, pressMPa, bilerp, total);

	fprintfWrapper_newline_MAIN("end");
	return 1;
}


#pragma region RGEOS
#pragma region CONVENIENCE
double absDiff(double value1, double value2)
{
	//abs: int
	//fabs: double
	return fabs(value1 - value2);
}

double absError(double value1, double value2)
{
	//abs: int
	//fabs: double
	return absDiff(value1, value2)/value2;

}

void newline(int count)
{
	int position = 0;
	do
	{	fprintfWrapper_newline(".");
		position++;
	}
	while (position <= count) ;
}

char* strConcat(const char *s1, const char *s2)
{
	const size_t len1 = strlen(s1);
	const size_t len2 = strlen(s2);

	char *result = malloc(len1 + len2 + 1); 

	memcpy(result, s1, len1);
	memcpy(result + len1, s2, len2 + 1);
	return result;
}

double timer(clock_t start, clock_t end)
{
	return ((double)(end-start))/ CLOCKS_PER_SEC;
}

double drand (double low, double high)
{
	return ((double)rand() * ( high - low )) / (double)RAND_MAX + low;
}
#pragma endregion CONVENIENCE

#pragma region FPRINTF
void fprintfWrapper(const char* format, va_list args)
{
	
	{	
		vfprintf(stdout, format, args);		/* vytiskne zformtovany text do stdout bufferu */
		fflush(stdout);				/* vyprzdní stdout buffer */
	}
}

void fprintfWrapper_newline(const char* format, ...)
{
	if (debugLevel >= MainAndFuncbounds)
	{
		va_list args;				/* načte argumenty do listu*/		
		va_start(args, format);			/* vloží argumenty do textu s %d %f %s... */
		fprintfWrapper(format, args);
		fprintf(stdout, "\n");
		va_end(args);				/* pročistí array s argumenty */
	}
}

void fprintfWrapper_newline_tail(const char* format, ...)
{
	if (debugLevel >= MainAndFuncbounds)
	{
		va_list args;
		va_start(args, format);	
		fprintf(stdout, "\t");
		fprintfWrapper(format, args);
		fprintf(stdout, "\n");
		va_end(args);	
	}
}

void fprintfWrapper_newline_mid_upper(const char* format, ...)
{
	if (debugLevel >= MainAndFuncboundsAndUpper)
	{
		va_list args;
		va_start(args, format);	
		fprintf(stdout, "\t\t");
		fprintfWrapper(format, args);
		fprintf(stdout, "\n");
		va_end(args);
	}
}

void fprintfWrapper_newline_mid_lower(const char* format, ...)
{
	if (debugLevel >= MainAndFuncboundsAndUpperAndLower)
	{
		va_list args;
		va_start(args, format);	
		fprintf(stdout, "\t\t\t");
		fprintfWrapper(format, args);
		fprintf(stdout, "\n");
		va_end(args);
	}
}

void fprintfWrapper_newline_head(const char* format, ...)
{
	if (debugLevel >= MainAndFuncbounds)
	{
		va_list args;
		va_start(args, format);	
		fprintf(stdout, "\n");
		fprintfWrapper(format, args);
		fprintf(stdout, "\n");
		va_end(args);	
	}
}

void fprintfWrapper_newline_MAIN(const char* format, ...)
{
	if (debugLevel >= Main)
	{
		va_list args;				/* načte argumenty do listu*/		
		va_start(args, format);			/* vloží argumenty do textu s %d %f %s... */
		fprintf(stdout, "MAIN:\t");
		fprintfWrapper(format, args);
		fprintf(stdout, "\n");
		va_end(args);				/* pročistí array s argumenty */
	}
}

void fprintfWrapper_comma(const char* format, ...)
{
	if (debugLevel >= MainAndFuncboundsAndUpper)
	{
		va_list args;
		va_start(args, format);	
		fprintf(stdout, "\t");
		fprintfWrapper(format, args);
		fprintf(stdout, ", ");
		va_end(args);	
	}
}

#pragma endregion FPRINTF


#pragma region COPYING
void copyArray1DNative_toArray1DNative(double *source, double *target, int count)
{
	fprintfWrapper_newline_head("copyArray1DNative_toArray1DNative: start -> %d elements!", count);
	if (target == NULL || source == NULL)
	{
		fprintfWrapper_newline_tail("copyArray1DNative_toArray1DNative: failure -> Memory allocation failed!");
		exit(1); // Handle allocation failure
	}
	int index;

	free(target);
	fprintfWrapper_newline_mid_upper("copyArray1DNative_toArray1DNative: freed the target array!");

	target = (double*)malloc(sizeof(double) * count);
	fprintfWrapper_newline_mid_upper("copyArray1DNative_toArray1DNative: re-mallocated the target array!");

	fprintfWrapper_newline_mid_upper("copyArray1DNative_toArray1DNative: copying the values!");
	for (index = 0; index < count; index++)
	{
		fprintfWrapper_newline_mid_lower("copyArray1DNative_toArray1DNative: index %d", index);
		target[index] = source[index];
	}
	fprintfWrapper_newline_tail("copyArray1DNative_toArray1DNative: success!");
}

void copyArray1DNative_toArray1DStruct(double *source, Array1D *target)
{
	fprintfWrapper_newline_head("copyArray1DNative_toArray1DStruct: start -> %d elements!", target->count);
	int index;
	free(target->array);
	fprintfWrapper_newline_mid_upper("copyArray1DNative_toArray1DStruct: freed the target array!");

	target->array = (double*)malloc(sizeof(double) * (target->count));
	fprintfWrapper_newline_mid_upper("copyArray1DNative_toArray1DStruct: re-mallocated the target array!");

	if (target->array == NULL || source == NULL)
	{
		fprintfWrapper_newline_tail("copyArray1DNative_toArray1DStruct: failure -> Memory allocation failed!");
		exit(1); // Handle allocation failure
	}

	fprintfWrapper_newline_mid_upper("copyArray1DNative_toArray1DStruct: copying the values!");
	for (index = 0; index < target->count; index++)
	{
		fprintfWrapper_newline_mid_lower("copyArray1DNative_toArray1DStruct: index %d", index);
		target->array[index] = source[index];
	}
	fprintfWrapper_newline_tail("copyArray1DNative_toArray1DStruct: success!");
}

void copyArray1Dstruct_toArray1DNative(Array1D *source, double *target)
{
	fprintfWrapper_newline_head("copyArray1Dstruct_toArray1DNative: start -> %d elements!", source->count);
	if (target == NULL || source->array == NULL)
	{
		fprintfWrapper_newline_tail("copyArray1Dstruct_toArray1DNative: failure ->Memory allocation failed!");
		exit(1); // Handle allocation failure
	}
	int index;
	free(target);
	fprintfWrapper_newline_mid_upper("copyArray1Dstruct_toArray1DNative: freed the target array!");
	target = (double*)malloc(sizeof(double) *(source->count));
	fprintfWrapper_newline_mid_upper("copyArray1Dstruct_toArray1DNative: re-mallocated the target array!");



	fprintfWrapper_newline_mid_upper("copyArray1Dstruct_toArray1DNative: copying the values!");
	for (index = 0; index < source->count; index++)
	{
		fprintfWrapper_newline_mid_lower("copyArray1Dstruct_toArray1DNative: index %d", index);
		target[index] = source->array[index];
	}
	fprintfWrapper_newline_tail("copyArray1Dstruct_toArray1DNative: success!");
}
#pragma endregion COPYING


#pragma region ARRAY_CREATION
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

Array1D createUniqueValuesArray1D(Array1D arrayIpt)
{
	fprintfWrapper_newline_head("createUniqueValuesArray1D: start!");

	int position_1 = 0;
	int position_2 = 1;
	int uniqueVals = countUniqueVals1D(arrayIpt);

	fprintfWrapper_newline_mid_upper("createUniqueValuesArray1D: reached unique value -> Breaking at %d unique values!", uniqueVals);
	

	Array1D nuArray = createArrayStruct1D(uniqueVals);
	fprintfWrapper_newline_mid_upper("createUniqueValuesArray1D: creating unique 1D array!");

	nuArray.array[position_1] = arrayIpt.array[position_1];
	for (position_1 = 0; position_1 < arrayIpt.count; position_1++)
	{
		if (!valueInArray(arrayIpt.array[position_1], nuArray))
		{
			nuArray.array[position_2] = arrayIpt.array[position_1];
			fprintfWrapper_newline_mid_lower("nuArray[%d]: %lf \t arrayIpt[%d]: %lf",
				position_2,
				nuArray.array[position_2],
				position_1,
				arrayIpt.array[position_1]
			);
			position_2++;
		}
	}
	printArrayStruct1D(nuArray);
	fprintfWrapper_newline_tail("createUniqueValuesArray1D: success! -> %d values", nuArray.count);
	return nuArray;
}

Array1D extractColumnFromTable(Array2D arrayIpt, int selectedColumn)
{
	fprintfWrapper_newline_head("extractColumnFromTable: start!");
	int rowIndex_arrayIpt;
	fprintfWrapper_newline_mid_upper("extractColumnFromTable: getting the numLines!");

	fprintfWrapper_newline_mid_upper("extractColumnFromTable: extracted column will have %d lines!", arrayIpt.rows);
	Array1D nuArray = createArrayStruct1D(arrayIpt.rows);

	fprintfWrapper_newline_mid_upper("extractColumnFromTable: initialized the column!");
	fprintfWrapper_newline_mid_upper("extractColumnFromTable: filling the column!");
	for (rowIndex_arrayIpt = 0; rowIndex_arrayIpt < arrayIpt.rows; rowIndex_arrayIpt++)
	{
		nuArray.array[rowIndex_arrayIpt] = arrayIpt.array[rowIndex_arrayIpt][selectedColumn];
		fprintfWrapper_newline_mid_lower("nuArray[%d]: %lf \t arrayIpt[%d][%d]: %lf",
			rowIndex_arrayIpt,
			nuArray.array[rowIndex_arrayIpt],
			rowIndex_arrayIpt,
			selectedColumn,
			arrayIpt.array[rowIndex_arrayIpt][selectedColumn]
		);
	}
	printArrayStruct1D(nuArray);
	fprintfWrapper_newline_tail("extractColumnFromTable: success!");
	return nuArray;
}

Array1D differences1D(double value, Array1D arrayIpt)
{
	fprintfWrapper_newline_head("differences1D: start -> value: %lf", value);
	Array1D nuArray = createArrayStruct1D(arrayIpt.count);
	int index;

	fprintfWrapper_newline_mid_upper("differences1D: filling the array with abs. value of differences!");
	for (index = 0; index < nuArray.count; index++)
	{
		nuArray.array[index] = absDiff(arrayIpt.array[index], value);
		fprintfWrapper_newline_mid_lower("nuArray.array[%d] = %lf",index, nuArray.array[index]);
	}
	fprintfWrapper_newline_head("differences1D: success!");
	return nuArray;
}

Array2D getRectangle(int index, Array2D arrayIpt, int yUnique)
{
	fprintfWrapper_newline_head("getRectangle: start -> target index: %d [%lf, %lf], array length: %d, array offset: %d",
	index, arrayIpt.array[index][T], arrayIpt.array[index][P],
	arrayIpt.count, yUnique
	);

	fprintfWrapper_newline_mid_upper("getRectangle: index MOD yUnique -> %d MOD yUnique %d = %d", index, yUnique, index%yUnique);

	if (index < 0 || index >= arrayIpt.lastRow)
	{
		fprintfWrapper_newline_tail("getRectangle: failure -> target index: %d < 0 or %d > %d", index, index, arrayIpt.lastRow);
		exit(1);
	}




	int TOP_LEFT, TOP_RIGHT, BOT_LEFT, BOT_RIGHT;
	Array2D nuArray = createArrayStruct2D(4, arrayIpt.columns);
	fprintfWrapper_newline_mid_upper("getRectangle: rectangle initialized!");

	if (index % yUnique == 0) //special case: first pressure value
	{
		fprintfWrapper_newline_mid_upper("getRectangle: index MOD yUnique == 0 -> %d MOD %d -> index+= (yUnique+1)", index, yUnique);
		index = index + yUnique + 1;
		
	}

	else if (index % yUnique == yUnique-1) // special case: last pressure value
	{	
		if (index != arrayIpt.lastRow) // only last temperature
			{
			fprintfWrapper_newline_mid_upper("getRectangle: index MOD yUnique == 0 -> %d MOD %d -> index+= (yUnique-1)", index, yUnique);
			index = index + yUnique - 1;
		}

		else // special case: both last temperature and last pressure
		{
			fprintfWrapper_newline_mid_upper("getRectangle: index MOD yUnique == 0 -> %d MOD %d -> index+= -1", index, yUnique);
			index = index - 1;
	
		}
		
	}



	// left edge value
	if ((index < yUnique))
	{
		fprintfWrapper_newline_mid_upper("getRectangle: v1 -> x in (0, yUnique)\t %d in (0, %d)", index, yUnique);
		TOP_LEFT	= index;
		TOP_RIGHT	= TOP_LEFT	+ 1;
		BOT_LEFT	= TOP_LEFT	+ yUnique;
		BOT_RIGHT	= TOP_RIGHT	+ yUnique;
	}

	// right edge value
	else if (	(index >= (arrayIpt.lastRow - yUnique))	)
	{	

		// have not surpassed the bounds yet
		if (index <= arrayIpt.lastRow)
		{
			fprintfWrapper_newline_mid_upper("getRectangle: v2A -> x in (lastRow - yUnique, lastRow)\t %d in (%d, %d)",
			index, (arrayIpt.lastRow-yUnique), arrayIpt.lastRow
			);
			TOP_LEFT	= index		- yUnique;
			TOP_RIGHT	= TOP_LEFT	+ 1;
			BOT_LEFT	= TOP_LEFT	+ yUnique;
			BOT_RIGHT	= TOP_RIGHT	+ yUnique;
		}
		
		// the bounds have been surpassed
		else
		{
			fprintfWrapper_newline_mid_upper("getRectangle: v2B -> x > lastRow\t %d > %d",
			index, arrayIpt.lastRow
			);
			TOP_LEFT	= index		- 2*yUnique;
			TOP_RIGHT	= TOP_LEFT	+ 1;
			BOT_LEFT	= index 	- 1*yUnique;
			BOT_RIGHT	= TOP_RIGHT	+ yUnique;
		}
	}

	//middle value
	else
	{		
		fprintfWrapper_newline_mid_upper("getRectangle: v3 -> x in (yUnique, lastRow - yUnique)\t %d in (%d, %d)",
		index, yUnique, (arrayIpt.lastRow-yUnique)
		);
		TOP_LEFT	= index - yUnique - 1;
		TOP_RIGHT	= TOP_LEFT	+ 2;
		BOT_LEFT	= TOP_LEFT	+ yUnique;
		BOT_RIGHT	= TOP_RIGHT	+ yUnique;
	}
	
	nuArray.array[TL] = arrayIpt.array[TOP_LEFT];
	nuArray.array[BL] = arrayIpt.array[BOT_LEFT];
	nuArray.array[TR] = arrayIpt.array[TOP_RIGHT];
	nuArray.array[BR] = arrayIpt.array[BOT_RIGHT];
	printArrayStruct2D(nuArray);
	fprintfWrapper_newline_tail("getRectangle: success! -> [[%d, %d], [%d, %d]]", TOP_LEFT, TOP_RIGHT, BOT_LEFT, BOT_RIGHT);
	return nuArray;
}

Array2D readFromCSV(char *filePath)
{
	fprintfWrapper_newline_head("readFromCSV: start -> from %s!", filePath);
	double stringToNumber;
	int position	= 0;
	int lineNum	= 0; 
	int rowCount 	= countLinesFile_noHeader(filePath);
	int columnCount = countColumnsFile(filePath);

	fprintfWrapper_newline_mid_upper("readFromCSV: creating Array2D with %d rows and %d columns!", rowCount, columnCount);
	Array2D nuArray = createArrayStruct2D(rowCount, columnCount);
	char **headers = (char**)malloc(sizeof(char*)*columnCount); 
	char line[BUFFER_SIZE_STRING];
	char *data;

	fprintfWrapper_newline_mid_upper("readFromCSV: filling Array2D with empty arrays 1D");
	for (position = 0; position < rowCount; position++)
	{
		nuArray.array[position] = (double*)malloc(sizeof(double)*columnCount);
	}
	fprintfWrapper_newline_mid_upper("readFromCSV: filled Array2D with empty arrays 1D");
	FILE* file = fopen(filePath, "r");
	if (file != NULL)
	{
		fprintfWrapper_newline_mid_upper("readFromCSV: reading %s", filePath);
		while (fgets(line, sizeof(line), file))
		{

			fprintfWrapper_newline_mid_lower("line: %s", line);
			if (line[0] == '\n') {continue;}
			int columnIndex	= 0;
			data = strtok(line, ", ");


			while (data != NULL)
			{
				////free(data);
				//fprintfWrapper_newline_mid_upper("readFromCSV: data = %s", data);
				if (lineNum == 0)
				{
					//fprintfWrapper_newline_mid_upper("readFromCSV: debugLine1 -> data: %s", data);
					headers[columnIndex] = strdup(data); /* aby mi zustala data, porad odkazuji pointery, tak musim string v headeru duplikovat */
					//fprintfWrapper_newline_mid_upper("readFromCSV: debugLine2");					
					fprintfWrapper_comma("%s", headers[columnIndex]);
				}

				else if (lineNum > 0)
				{
					//fprintfWrapper_newline_mid_upper("readFromCSV: got through header!");
					sscanf(data, "%lf", &stringToNumber); /* na adresu d prevedu hodnotu ze stringu na double */
					if (lineNum % ITER_PRINT_FREQUENCY == 0)
					{
						fprintfWrapper_comma("%.6lf", stringToNumber);
					}
					nuArray.array[lineNum - 1][columnIndex] = stringToNumber;
				}
				//fprintfWrapper_newline_mid_upper("readFromCSV: inner while loop before strtok");
				data = strtok(NULL, ", ");
				//fprintfWrapper_newline_mid_upper("readFromCSV: next inner while loop iter!");
				columnIndex++;
			}

			if (lineNum % ITER_PRINT_FREQUENCY == 0) {newline(1);}
			if (lineNum > rowCount*columnCount)
			{
				fprintfWrapper_newline_mid_upper("readFromCSV: Exceeded the length of a file, how?");
				break;
			}
			//fprintfWrapper_newline_mid_upper("readFromCSV: next outer while loop iter!");
			lineNum++;
		}
		//if (lineNum < 50) {newline(1);}
		if (lineNum == ITER_PRINT_FREQUENCY) {fprintfWrapper_newline_mid_upper("readFromCSV: End of table preview!");}
		nuArray.headers = headers;

		fprintfWrapper_newline_mid_upper("readFromCSV: closing %s", filePath);
		fclose(file);

		printArrayStruct2D(nuArray);
		fprintfWrapper_newline_tail("readFromCSV: success -> read %s into a 2D array with %d rows and %d columns", filePath, nuArray.rows, nuArray.columns);
		return nuArray;
	}
	fprintfWrapper_newline_tail("readFromCSV: failure -> does not exist: %s", filePath);
	exit(1);
}
#pragma endregion ARRAY_CREATION


#pragma region QUICKSORT
void swap(double* a, double* b)
{
	double temp = *a;
	*a = *b;
	*b = temp;
}

int partition(double arr[], int low, int high)
{
	double pivot = arr[high];
	int i = low - 1;
	int j;

	for (j = low; j < high; j++)
	{
		if (arr[j] <= pivot)
		{
					i++;
					swap(&arr[i], &arr[j]);
			}
	}
	swap(&arr[i + 1], &arr[high]);
	return i + 1;
}

void quickSort(double arr[], int count)
{
	fprintfWrapper_newline_head("quickSort: start");
	if (debugLevel == 3) {printArrayNative1D_double(arr, count);}
	quickSort_inner(arr, 0, count-1);
	if (debugLevel == 3) {printArrayNative1D_double(arr, count);}
	fprintfWrapper_newline_tail("quickSort: success");
}

void quickSort_inner(double arr[], int low, int high)
{
	if (low < high)
	{
		int partitionIndex = partition(arr, low, high);
		quickSort_inner(arr, low, partitionIndex - 1);
		quickSort_inner(arr, partitionIndex + 1, high);
	}
}

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

#pragma endregion QUICKSORT


#pragma region COMPARISON
bool compare(double compared, double reference)
{
	double lower = reference*(1 - COMPARE_THRESHOLD);
	double upper = reference*(1 + COMPARE_THRESHOLD);

	if ((lower <= compared) && (compared <= upper)) { return true; } 
	else { return false; }
}

bool valueInArray(double compared, Array1D arrayIpt)
{
	int index	= 0;
	double value	= 0;
	for (index = 0; index < arrayIpt.count; index++)
	{
		value = arrayIpt.array[index];
		if (compare(compared, value))
		{
			fprintfWrapper_newline_mid_lower("valueInArray: YES (%lf )", compared);
			return true;
		}
	}
	fprintfWrapper_newline_mid_lower("valueInArray: NO (%lf )", compared);
	return false; 
}
#pragma endregion COMPARISON


#pragma region PRINT_ARRAY
void printArrayNative1D_string(char **arrayIpt, int length)
{
	if (debugLevel>= MainAndFuncboundsAndUpper)
	{
		fprintfWrapper_newline_head("printArrayNative1D_string: start!");
		int index;
		printf("[");
		for (index = 0; index < length; index++)
		{
			printf("%s, ", arrayIpt[index]);
		}
		printf("]\n");
		fflush(stdout);
		fprintfWrapper_newline_tail("printArrayNative1D_string: success!");
	}
}

void printArrayNative1D_double(double *arrayIpt, int length)
{
	if (debugLevel >= MainAndFuncboundsAndUpper)
	{
		fprintfWrapper_newline_head("printArrayNative1D_double: start!");
		int index;
		printf("[");
		for (index = 0; index < length; index++)
		{
			printf("%lf, ", arrayIpt[index]);
		}
		printf("]\n");
		fflush(stdout);
		fprintfWrapper_newline_tail("printArrayNative1D_double: success!");
	}
}

void printArrayStruct1D(Array1D arrayIpt)
{
	if (debugLevel >= MainAndFuncboundsAndUpper)
	{
		fprintfWrapper_newline_head("printArrayStruct1D: start!");
		int index;
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
		int indexRow, indexCol;
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
#pragma endregion PRINT_ARRAY




double interpolation2D_bilinear(Array2D arrayIpt, double x, double y, enum tableColumn selectedColumn, Array1D uniqueX, Array1D uniqueY)
{
	fprintfWrapper_newline_head("interpolation2D_bilinear: start -> [%lf %lf %d]", x, y, selectedColumn);
	if ((x < arrayIpt.array[0][0]) || (y > arrayIpt.array[arrayIpt.lastRow][0]))
	{
		fprintfWrapper_newline_tail("interpolation2D_bilinear: failure -> extrapolation (in 1. independent) variable not supported! %lf not in <%lf, %lf>",
		x, arrayIpt.array[0][0], arrayIpt.array[arrayIpt.lastRow][0]
		);
		exit(1);
	}

	if ((y < arrayIpt.array[0][1]) || (y > arrayIpt.array[arrayIpt.lastRow][1]))
	{
		fprintfWrapper_newline_tail("interpolation2D_bilinear: failure -> extrapolation (in 2. independent) variable not supported! %lf not in <%lf, %lf>",
		y, arrayIpt.array[0][1], arrayIpt.array[arrayIpt.lastRow][1]
		);
		exit(1);
	}

	fprintfWrapper_newline_mid_upper("interpolation2D_bilinear: passed the range checks!");
	double result;


	fprintfWrapper_newline_mid_upper("interpolation2D_bilinear: finding the closest value using the binary search in the read table!");
	int closest_eff	= findClosest2D_efficient(x, y, uniqueX, uniqueY);

	fprintfWrapper_newline_mid_upper("interpolation2D_bilinear: creating the bounding recangle to the closest value index: %d!", closest_eff);
	Array2D rect	= getRectangle(closest_eff, arrayIpt, uniqueY.count);

	fprintfWrapper_newline_mid_upper("interpolation2D_bilinear: finding the closest value in the read table!");

	double *point_11 = rect.array[TL]; //point11
	double *point_21 = rect.array[BL]; //point21
	double *point_12 = rect.array[TR]; //point12
	double *point_22 = rect.array[BR]; //point22
	
	double x1 = point_11[T]; //point_11.x
	double y1 = point_11[P]; //point_11.y
	double x2 = point_22[T]; //point_22.x
	double y2 = point_22[P]; //point_22.y

	if ((x1 == x2) || (y1 == y2))
	{
		fprintfWrapper_newline_tail("interpolation2D_bilinear: failure -> not a rectangle!");
		exit(1);
	}



	fprintfWrapper_newline_mid_upper("interpolation2D_bilinear: points:");
	fprintfWrapper_newline_mid_lower("point_11[%lf, %lf, %lf]", point_11[T], point_11[P], point_11[selectedColumn]);
	fprintfWrapper_newline_mid_lower("point_21[%lf, %lf, %lf]", point_21[T], point_21[P], point_21[selectedColumn]);
	fprintfWrapper_newline_mid_lower("point_12[%lf, %lf, %lf]", point_12[T], point_12[P], point_12[selectedColumn]);
	fprintfWrapper_newline_mid_lower("point_22[%lf, %lf, %lf]", point_22[T], point_22[P], point_22[selectedColumn]);
	fprintfWrapper_newline_mid_upper("interpolation2D_bilinear: x1: %lf, x2: %lf, y1: %lf, y2: %lf", x1, x2, y1, y2);
	


	double dxdy = (x2-x1)*(y2-y1);
	fprintfWrapper_newline_mid_upper("interpolation2D_bilinear: dxdy: %lf", dxdy);

	fprintfWrapper_newline_mid_upper("interpolation2D_bilinear: interpolation members"); 
	double left_top		= point_11[selectedColumn] * (x2 - x) * (y2 - y);
	fprintfWrapper_newline_mid_lower("left_top -> %lf * (%lf - %lf) * (%lf - %lf) -> %lf * %lf * %lf -> %lf",
	point_11[selectedColumn],x2,x,y2,y,
	point_11[selectedColumn], (x2 - x), (y2 - y),
	point_11[selectedColumn] * (x2 - x) * (y2 - y)
	);

	double right_top	= point_21[selectedColumn] * (x - x1) * (y2 - y);
	fprintfWrapper_newline_mid_lower("right_top -> %lf * (%lf - %lf) * (%lf - %lf) -> %lf * %lf * %lf -> %lf",
	point_21[selectedColumn],x,x1,y2,y,
	point_21[selectedColumn], (x - x1), (y2 - y),
	point_21[selectedColumn] * (x - x1) * (y2 - y)
	);

	double left_bot		= point_12[selectedColumn] * (x2 - x) * (y - y1);
	fprintfWrapper_newline_mid_lower("left_bot -> %lf * (%lf - %lf) * (%lf - %lf) -> %lf * %lf * %lf -> %lf",
	point_12[selectedColumn],x2,x,y,y1,
	point_12[selectedColumn], (x2 - x), (y - y1),
	point_12[selectedColumn] * (x2 - x) * (y - y1)
	);

	double right_bot	= point_22[selectedColumn] * (x - x1) * (y - y1);
	fprintfWrapper_newline_mid_lower("right_bot -> %lf * (%lf - %lf) * (%lf - %lf) -> %lf * %lf * %lf -> %lf",
	point_22[selectedColumn],x,x1,y,y1,
	point_22[selectedColumn], (x - x1), (y - y1),
	point_22[selectedColumn] * (x - x1) * (y - y1)
	);

	fprintfWrapper_newline_mid_upper("interpolation2D_bilinear: [[%lf, %lf], [%lf, %lf]]", left_top/dxdy, right_top/dxdy, left_bot/dxdy, right_bot/dxdy);
	result = (left_top + right_top + left_bot + right_bot)/dxdy;

	fprintfWrapper_newline_tail("interpolation2D_bilinear: success! -> %lf", result);
	return result;
}


#pragma region STATISTICS_ARRAY
double minimumStruct1D_value(Array1D arrayIpt)
{
	fprintfWrapper_newline_head("minimumStruct1D_value: start");
	double minValue = 1.23456789e11;
	int index = 0;
	for (index = 0; index < arrayIpt.count; index++)
	{
		if (arrayIpt.array[index] < minValue)
		{
			minValue = arrayIpt.array[index];
			fprintfWrapper_newline_mid_lower("index %d: %lf", index, minValue);
		}
	}
	fprintfWrapper_newline_tail("minimumStruct1D_value: success! -> minimum: %lf", minValue);
	return minValue;
}

int minimumStruct1D_index(Array1D arrayIpt)
{
	fprintfWrapper_newline_head("minimumStruct1D_index: start");
	double minValue = 1.23456789e11;
	int index = 0;
	int targetIndex = 0;
	for (index = 0; index < arrayIpt.count; index++)
	{
		if (arrayIpt.array[index] < minValue)
		{
			minValue = arrayIpt.array[index];
			fprintfWrapper_newline_mid_lower("index: %d -> value %lf: %d", index, minValue, targetIndex);
			targetIndex++;
		}
	}
	fprintfWrapper_newline_tail("minimumStruct1D_index: success! -> minimum index: %d", targetIndex);
	return targetIndex;
}

double maximumStruct1D_value(Array1D arrayIpt)
{
	fprintfWrapper_newline_head("maximumStruct1D_value: start");
	double maxValue = -1.23456789e11;
	int index = 0;
	for (index = 0; index > arrayIpt.count; index++)
	{
		if (arrayIpt.array[index] < maxValue)
		{
			maxValue = arrayIpt.array[index];
			fprintfWrapper_newline_mid_lower("index: %d -> value %lf: %d", index, maxValue);
		}
	}
	fprintfWrapper_newline_tail("maximumStruct1D_value: success! -> minimum: %lf", maxValue);
	return maxValue;
}

int maximumStruct1D_index(Array1D arrayIpt)
{
	fprintfWrapper_newline_head("maximumStruct1D_index: start");
	double maxValue = -1.23456789e11;
	int index = 0;
	int targetIndex = 0;
	for (index = 0; index > arrayIpt.count; index++)
	{
		if (arrayIpt.array[index] < maxValue)
		{
			maxValue = arrayIpt.array[index];
			targetIndex++;
			fprintfWrapper_newline_mid_lower("index: %d -> value %lf: %d", index, maxValue, targetIndex);
		}
	}
	fprintfWrapper_newline_tail("maximumStruct1D_index: success! -> maximum index: %d", targetIndex);
	return maxValue;
}

double sumStruct1D_value(Array1D arrayIpt)
{
	fprintfWrapper_newline_head("sumStruct1D_value: start");
	double sum = 0.0;
	int index = 0;
	for (index = 0; index < arrayIpt.count; index++)
	{
		sum = sum + arrayIpt.array[index];
		fprintfWrapper_newline_mid_lower("index %d: %lf", index, sum);
	}
	fprintfWrapper_newline_tail("sumStruct1D_value: success! -> sum: %lf", sum);
	return sum;
}

double averageStruct1D_value(Array1D arrayIpt)
{
	fprintfWrapper_newline_head("averageStruct1D_value: start");
	double average = sumStruct1D_value(arrayIpt)/arrayIpt.count;
	fprintfWrapper_newline_tail("averageStruct1D_value: success! -> ar. average: %lf", average);
	return average;
}
#pragma endregion STATISTICS_ARRAY

#pragma region CLOSEST_ARRAY
int findClosest1D_linear(double variable, Array1D arrayIpt)
{
	fprintfWrapper_newline_head("findClosest1D_linear: start -> variable: %lf", variable);
	Array1D diffs = differences1D(variable, arrayIpt);
	double minValue = minimumStruct1D_value(diffs);
	printArrayStruct1D(diffs);
	int index;

	fprintfWrapper_newline_mid_lower("findClosest1D_linear: searching!");
	for (index = 0; index < diffs.count; index++)
	{
		fprintfWrapper_newline_mid_lower("index: %d | dPhi: %lf", index, diffs.array[index]);
		if (compare(diffs.array[index], minValue)) break;
	}
	//free(diffs.array);
	
	fprintfWrapper_newline_tail("findClosest1D_linear: success! -> index: %d", index);
	return index;
}

int findClosest1D_efficient(double variable, Array1D arrayIpt)
{
	fprintfWrapper_newline_head("findClosest1D_efficient: start -> variable: %lf", variable);

	int low, mid, high;
	double error;
	low		= 0;
	high	= arrayIpt.last;
	fprintfWrapper_newline_mid_upper("findClosest1D_efficient: iterstart-> [low: %d, mid: %d, high: %d]", low, mid, high);
	while (low <= high)
	{
		mid = low + (int)((high - low) / 2);
		error = absError(arrayIpt.array[mid], variable);
		fprintfWrapper_newline_mid_lower("findClosest1D_efficient: itergo-> [low: %d, mid: %d, high: %d, compared: %lf, ref: %lf, error: %lf]",
		low, mid, high,
		arrayIpt.array[mid], variable, error
		);

		if (SEARCH_THRESHOLD > error)
		{
			fprintfWrapper_newline_tail("findClosest1D_efficient: success! -> index: %d", mid);
			return mid; 
		}
		if (arrayIpt.array[mid] < variable) low = mid + 1;
		else high = mid - 1;
	}
	fprintfWrapper_newline_tail("findClosest1D_efficient: failure! -> attempt at index: %d", mid);
	return mid;
}

int findClosest2D(double independentVariable1, double independentVariable2, Array2D arrayIpt)
{
	fprintfWrapper_newline_head("findClosest2D: start -> [%lf, %lf]", independentVariable1, independentVariable2);
	Array1D column_1 = extractColumnFromTable(arrayIpt, T);
	sortAscendingStruct1D(column_1);
	column_1 = createUniqueValuesArray1D(column_1);

	Array1D column_2 = extractColumnFromTable(arrayIpt, P);
	sortAscendingStruct1D(column_2);
	column_2 = createUniqueValuesArray1D(column_2);

	printArrayStruct1D(column_1);
	printArrayStruct1D(column_2);

	fprintfWrapper_newline_mid_upper("findClosest2D: Got the column1 offset: %d & column2 offset: %d", column_1.count, column_2.count);
	int closest_1		= findClosest1D_efficient(independentVariable1, column_1);
	int closest_2		= findClosest1D_efficient(independentVariable2, column_2);
	fprintfWrapper_newline_mid_upper("findClosest2D: closest_1 <index>: %d, closest_2 <index>: %d", closest_1, closest_2);
	fprintfWrapper_newline_mid_upper("findClosest2D: closest_1 <value>: %lf, closest_2 <value>: %lf", column_1.array[closest_1], column_2.array[closest_2]);
	int result			= closest_1*column_2.count + closest_2;

	fprintfWrapper_newline_mid_upper("findClosest2D: ivar1 <%d>: %lf, ivar2 <%d>: %lf",
	result, arrayIpt.array[result][T], result, arrayIpt.array[result][P]);

	/*fprintfWrapper_newline_mid_upper("findClosest2D: Freeing the array memory!");
	free(column_1.array);
	free(column_2.array);
	fprintfWrapper_newline_mid_upper("findClosest2D: memory cleanup done!");*/
	fprintfWrapper_newline_tail("findClosest2D: success! -> index: %d", result);
	return result;
}

int findClosest2D_efficient(double independentVariable1, double independentVariable2, Array1D uniqueList1, Array1D uniqueList2)
{
	fprintfWrapper_newline_head("findClosest2D_efficient: start -> [%lf, %lf]", independentVariable1, independentVariable2);
	int closest_1		= findClosest1D_linear(independentVariable1, uniqueList1);
	int closest_2		= findClosest1D_linear(independentVariable2, uniqueList2);
	int result			= closest_1*uniqueList2.count + closest_2;
	fprintfWrapper_newline_tail("findClosest2D_efficient: success! -> index: %d", result);
	return result;
}
#pragma endregion CLOSEST_ARRAY

#pragma region COUNTS
int countLinesFile(char *filePath)
{
	fprintfWrapper_newline_head("countLinesFile: start -> from %s!", filePath);
	FILE *file = fopen(filePath, "r");
	if (file != NULL)
	{
		fprintfWrapper_newline_mid_upper("countLinesFile: reading from %s", filePath);
		char buf[BUFFER_SIZE];
		int counter = 0;
		for(;;)
		{
				size_t res = fread(buf, 1, BUFFER_SIZE, file);
				if (ferror(file))
				{ 
					fprintfWrapper_newline_tail("countLinesFile: failure -> ferror(%s)", filePath) ;
					exit(1);
				}

				int i;
				for(i = 0; i < res; i++)
				{
					if (buf[i] == '\n') {counter++ ;}
				}
				if (feof(file)) {break; }
		}
		fprintfWrapper_newline_mid_upper("countLinesFile: closing %s", filePath);
		fclose(file);

		fprintfWrapper_newline_tail("countLinesFile: success -> lines in %s: %d", filePath, counter);
		return counter;
	}
	fprintfWrapper_newline_tail("countLinesFile: failure -> not exist: %s", filePath);
	exit(1);
}

int countLinesFile_noHeader(char *filePath)
{
	fprintfWrapper_newline_head("countLinesFile_noHeader: start -> from %s without a header!", filePath);
	int result = countLinesFile(filePath) - 1;
	fprintfWrapper_newline_tail("countLinesFile_noHeader: success -> %d lines!", result);
	return result;
}

int countColumnsFile(char *filePath)
{
	fprintfWrapper_newline_head("countColumnsFile: start -> from %s!", filePath);
	FILE *file = fopen(filePath, "r");
	if (file != NULL)
	{
		fprintfWrapper_newline_mid_upper("countColumnsFile: reading %s", filePath);
		char buf[BUFFER_SIZE];
		int counter = 0;
		for(;;)
		{
				size_t res = fread(buf, 1, BUFFER_SIZE, file);
				if (ferror(file)) { return -1; }

				int i;
				for(i = 0; i < res; i++)
			{
						if (buf[i] == '\n') {break ;}
						if (buf[i] == ',') {counter ++ ;}
			}

				if (feof(file)) {break; }
		}

		fclose(file);

		if (counter > 1) {counter++;}
		fprintfWrapper_newline_tail("countColumnsFile: success -> columns in %s: %d", filePath, counter);
		return counter;
	}
	fprintfWrapper_newline_tail("countColumnsFile: failure -> does not exist: %s", filePath);
	exit(1);
}

int countUniqueVals1D(Array1D arrayIpt)
{
	fprintfWrapper_newline_head("countUniqueVals1D: start!");
	int index = 0;
	int count = 0;
	fprintfWrapper_newline_mid_upper("countUniqueVals1D: counting!");
	for (index = 0; index < arrayIpt.count; index++)
	{
		if(compare(arrayIpt.array[index], arrayIpt.array[index + 1]) == false)
		{
			count++;
		fprintfWrapper_newline_mid_lower("index %d: %d", index, count);
		}
	}
	fprintfWrapper_newline_tail("countUniqueVals1D: success -> %d unique values!", count);
	return count;
}

int countNonUniqueVals1D(Array1D arrayIpt)
{
	fprintfWrapper_newline_head("countNonUniqueVals1D: start!");
	int index = 0;
	int count = 0;
	fprintfWrapper_newline_mid_upper("countNonUniqueVals1D: counting!");
	for (index = 0; index < arrayIpt.count; index++)
	{
		if(compare(arrayIpt.array[index], arrayIpt.array[index + 1]) == true)
		{
			count++;
			fprintfWrapper_newline_mid_lower("index %d: %d", index, count);
		}
	}
	fprintfWrapper_newline_tail("countNonUniqueVals1D: success -> %d non-unique values!", count);
	return count;
}

#pragma endregion COUNTS


#pragma endregion RGEOS
Array2D readTable;
Array1D temperatureList;
Array1D pressureList;


DEFINE_ON_LOADING(I_do_nothing)
{
    readTable         = readFromCSV("table.csv");
    temperatures      = extractColumnFromTable(readTable, T);
    pressures         = extractColumnFromTable(readTable, P);
    temperatureTable  = createUniqueValuesArray1D(temperatures);
    pressureTable     = createUniqueValuesArray1D(pressures); 
    free(temperatures.array);
    free(pressures.array);
}

#pragma region IAPWS_NUTNE
void IAPWS_INITIALIZER(
    Domain *domain,
    cxboolean vapor_phase,
    char *species_list,
    int(*messagefunc)(const char *format,...),
    void (*errorfunc)(const char *format,...)
)
{
    usersMessage = messagefunc;
    usersError = errorfunc;
    usersMessage("\nLoading Redlich-Kwong Library: %s\n", species_list);
}


double IAPWS_density(double temp, double pressure)
{
    return interpolation2D_bilinear(readTable, temp, pressure, RHO, temperatureTable, pressureTable);
}

double IAPWS_viscosity(double temp, double pressure)
{
    return interpolation2D_bilinear(readTable, temp, pressure, MU, temperatureTable, pressureTable);
}

double IAPWS_specific_heat(double temp, double pressure)
{
    return interpolation2D_bilinear(readTable, temp, pressure, CP, temperatureTable, pressureTable);
}

double IAPWS_thermal_conductivity(double temp, double pressure)
{
    return interpolation2D_bilinear(readTable, temp, pressure, K, temperatureTable, pressureTable);
}
#pragma endregion IAPWS_NUTNE

#pragma region IAPWS_DOPLNEK
double IAPWS_mw(double temp, double pressure)   return 18.0153e-3;
double IAPWS_enthalpy(double temp, double pressure)   return 0;
double IAPWS_entropy(double temp, double pressure)   return 0;
double IAPWS_speed_of_sound(double temp, double pressure)   return 0;
double IAPWS_rho_t(double temp, double pressure)   return 0;
double IAPWS_rho_p(double temp, double pressure)   return 0;
double IAPWS_enthalpy_t(double temp, double pressure)   return 0;
double IAPWS_enthalpy_p(double temp, double pressure)   return 0;
#pragma endregion IAPWS_DOPLNEK

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
#pragma endregion RGEOS
