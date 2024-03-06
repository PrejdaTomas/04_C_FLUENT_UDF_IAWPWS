#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <math.h>


#define BUF_SIZE 65536 //65536
#define BUF_SIZE_STRING 512 //512
#define ITER_PRINT_FREQUENCY 40

void fprintfWrapper_newline(const char* format, ...);
void fprintfWrapper_newline_head(const char* format, ...);
void fprintfWrapper_newline_mid(const char* format, ...);
void fprintfWrapper_newline_tail(const char* format, ...);
void fprintfWrapper_comma(const char* format, ...);


void swap(double* a, double* b);
int partition(double arr[], int low, int high);
void quickSort(double arr[], int low, int high);

double *arrayGen1D(int count);
double **arrayGen2D(int countRow, int countCol);
double **getRectangle(int countRow, int countCol, int xIndex, int yIndex, double **arrayIpt);
double **readFromCSV(char *filePath);

double interpolation2D_bilinear(int countRow, int countCol, double **pointset, double x, double y, int selectedColumn);

int countLinesFile(char *filePath);
int countLinesFile_noHeader(char *filePath);
int countLinesReadTable(double **arrayIpt);
int countColumnsFile(char *filePath);
int countColumnsReadTable(double **arrayIpt);
int findClosest(double temperature, double pressure, double **arrayIpt);

void printArray1D(int count, double *arrayIpt);
void printArray2D(int countRow, int countCol, double **arrayIpt);
void newline(int count);

/*
void sin2D(int countRow, int countCol, double **arrayIpt);
*/

char *strConcat(const char *s1, const char *s2);

double *extractColumnFromTable(int index, double **arrayIpt);
double *createUniqueValuesArray(int countRow, double *arrayIpt);

bool debug = true;
bool debugMain = true;

int main()
{
    if (debugMain)
    {
	    newline(2);   
        printf("Running the Main!\n");
        fflush(stdout); /*branim se preteceni stdout bufferu a sigsegv*/
    }

	char *tablefilePath		= "table.csv";
	int row_count_csv	    = countLinesFile_noHeader(tablefilePath);
	int column_count_csv	= countColumnsFile(tablefilePath);

	double **read		    = readFromCSV(tablefilePath);
    if (debugMain)
    {
        printf("Finished reading the csv in the main loop!\n");
	    printArray2D(row_count_csv, column_count_csv, read);
        newline(1);
        fflush(stdout);
    }
    //getchar();
	int row_count_array	    = countLinesReadTable(read);
	int column_count_array	= countColumnsReadTable(read);

	if (debugMain) {newline(2);}

    if (debugMain) {printf("TEMPERATURES\n");}
	double *temperatures	= extractColumnFromTable(0, read);
    
    if (debugMain) {printf("PRESSURES\n");}
	double *pressures       = extractColumnFromTable(1, read);

    if (debugMain)
    {
        printArray1D(row_count_array, pressures);
        printArray1D(row_count_array, temperatures);
        fflush(stdout);
    }
	quickSort(temperatures, 0, row_count_array -1 ); 
	quickSort(pressures, 0, row_count_array -1 ); 

	//bubbleSort(temperatures, row_count_array);
	//bubbleSort(pressures, row_count_array);
    if (debugMain)
    {
	    printArray1D(row_count_array, pressures);
        fflush(stdout);
	    printArray1D(row_count_array, temperatures);
        fflush(stdout);
    }

	double *unique_temperatures	= createUniqueValuesArray(row_count_array, temperatures);
	double *unique_pressures	= createUniqueValuesArray(row_count_array, pressures);

    free(temperatures);
    free(pressures);
	double **rect	= getRectangle(row_count_csv, column_count_csv, 20, 3, read);
	if (debugMain)
    {
        printArray2D(4, 2, rect);
	    newline(2);
        fflush(stdout);
    }


	return 1;
}


void fprintfWrapper_newline(const char* format, ...)
{
    if (debug)
    {
        va_list args;
        va_start(args, format);
        vfprintf(stdout, format, args);
        fprintf(stdout, "\n");  // Print to stdout (you can change this)
        fflush(stdout);
        va_end(args);
    }
}

void fprintfWrapper_newline_tail(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    fprintf(stdout, "\t");
    fprintfWrapper_newline(format, args);
    fprintfWrapper_newline("", args);
}

void fprintfWrapper_newline_mid(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    fprintf(stdout, "\t");
    fprintfWrapper_newline(format, args);
}


void fprintfWrapper_newline_head(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    fprintfWrapper_newline(format, args);
}

void fprintfWrapper_comma(const char* format, ...)
{
    if (debug)
    {
        va_list args;
        va_start(args, format);
        vfprintf(stdout, format, args);
        fprintf(stdout, ", ");  // Print to stdout (you can change this)
        fflush(stdout);
        va_end(args);
    }
}



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

void quickSort(double arr[], int low, int high)
{
	if (low < high)
	{
		int partitionIndex = partition(arr, low, high);
		quickSort(arr, low, partitionIndex - 1);
		quickSort(arr, partitionIndex + 1, high);
	}
}
 

















double *arrayGen1D(int count)
{
    fprintfWrapper_newline_head("arrayGen1D: start!");
	double *nuArray = (double*)malloc(sizeof(double)*count);
	int index;
	for (index = 0; index < count; index++)
	{
		nuArray[index] = (double)index;
	}
    fprintfWrapper_newline_mid("arrayGen1D: success!");
	return nuArray;
}

double **arrayGen2D(int countRow, int countCol)
{
	fprintfWrapper_newline_head("arrayGen2D: start!");
	double **nuArray = (double**)malloc(sizeof(double*)*countRow);
	int indexRow, indexCol;
    
	for (indexRow = 0; indexRow < countRow; indexRow++)
	{
		nuArray[indexRow] = (double*)malloc(sizeof(double)*countCol);
		for (indexCol = 0; indexCol < countCol; indexCol++)
		{
			nuArray[indexRow][indexCol] = (double)(indexRow+indexCol)/10;
		}
	}
    fprintfWrapper_newline_tail("arrayGen2D: success!");
	return nuArray;
}



void printArray1D(int count, double *arrayIpt)
{
	fprintfWrapper_newline_head("printArray1D: start!");
	int index;
	printf("[");
	for (index = 0; index < count; index++)
	{
		printf("%lf, ", arrayIpt[index]);
	}
	printf("]\n");
    fflush(stdout);
    fprintfWrapper_newline_tail("printArray1D: success!");
}

void printArray2D(int countRow, int countCol, double **arrayIpt)
{
	fprintfWrapper_newline_head("printArray2D: start!");
	int indexRow, indexCol;
	printf("[\n");
	for (indexRow = 0; indexRow < countRow; indexRow++)
	{
		printf("\t[");
		for (indexCol = 0; indexCol < countCol; indexCol++)
		{
			printf("%lf, ", arrayIpt[indexRow][indexCol]);
		}
		printf("]\n");
	}
	printf("]\n");
    fprintfWrapper_newline_tail("printfArray2D: success!");
    fflush(stdout);
}


/*
void sin2D(int countRow, int countCol, double **arrayIpt)
{
	fprintfWrapper_newline_head("sin2D: start!");
	int indexRow, indexCol;
	for (indexRow = 0; indexRow < countRow; indexRow++)
	{
		for (indexCol = 2; indexCol < countCol; indexCol++)
		{
			arrayIpt[indexRow][indexCol] = sin(arrayIpt[indexRow][indexCol]/180.0*3.14159);
		}
	}
    fprintfWrapper_newline_tail("sin2D: success!");

}
*/

void newline(int count)
{
	int position = 0;
	do
	{	printf("\n");
		position++;
	}
	while (position < count) ;
}

double interpolation2D_bilinear(int countRow, int countCol, double **pointset, double x, double y, int selectedColumn)
{
	fprintfWrapper_newline_head("interpolation2D_bilinear: start");
	double result;
	int indexRow;
	int indexCol;
	double *point1 = pointset[0];
	double *point2 = pointset[1];
	double *point3 = pointset[2];
	double *point4 = pointset[3];
	
	double x1 = point1[0];
	double x2 = point2[0];
	double y1 = point3[1];
	double y2 = point4[1];

	double q11 = point1[selectedColumn];
	double q21 = point2[selectedColumn];
	double q12 = point3[selectedColumn];
	double q22 = point4[selectedColumn];
	
	double dxdy = (x2-x1)*(y2-y1);
	
	double lt = q11 * (x2 - x) * (y2 - y);
	double rt = q21 * (x - x1) * (y2 - y);
	double lb = q12 * (x2 - x) * (y - y1);
	double rb = q22 * (x - x1) * (y - y1);
	
	result = (lt + rt + lb + rb)/dxdy;
    fprintfWrapper_newline_mid("interpolation2D_bilinear: success!");
	return result;
}


double *createUniqueValuesArray(int countRow, double *arrayIpt)
{
	fprintfWrapper_newline_head("createUniqueValuesArray: start!");
	int uniqueVals  = 1;
	int position_1  = 0;
	int position_2  = 0;
	while (arrayIpt[position_1] == arrayIpt[position_1 + 1])
	{

		uniqueVals++;
		position_1++;
	}
	fprintfWrapper_newline_mid("createUniqueValuesArray: reached unique value -> Breaking at %d unique values!", uniqueVals);
	

	double *nuArray = (double*)malloc(sizeof(double)*uniqueVals);
	fprintfWrapper_newline_mid("createUniqueValuesArray: creating unique 1D array!");
	for (position_1 = 0; position_1 < countRow; position_1++)
	{
		if (arrayIpt[position_1] != nuArray[position_2])
		{
			nuArray[position_2] = arrayIpt[position_1];
			position_2++;
			fprintfWrapper_newline_mid("\tAdded %lf to arrayIpt at position %d!", arrayIpt[position_1], position_2);
		}
	}
	fprintfWrapper_newline_tail("createUniqueValuesArray: success!");
	return nuArray;
}


int countLinesReadTable(double **arrayIpt)
{
	fprintfWrapper_newline_head("countLinesReadTable: start!");
	int position = 0;
	while (arrayIpt[position])
	{
		position++;
	}
	fprintfWrapper_newline_tail("countLinesReadTable: success -> %d lines!", position);
	return position;
}


int countColumnsReadTable(double **arrayIpt)
{
	fprintfWrapper_newline_head("countColumnsReadTable: start!");
	int position = 0;
	while (arrayIpt[0][position])
	{
		position++;
	}
	fprintfWrapper_newline_tail("countColumnsReadTable: success -> %d columns!", position);
	return position;
}


double *extractColumnFromTable(int index, double **arrayIpt)
{
	fprintfWrapper_newline_head("extractColumnFromTable: start!");
	int position;
	fprintfWrapper_newline_mid("extractColumnFromTable: getting the numLines!");

	int numLines	= countLinesReadTable(arrayIpt);
	fprintfWrapper_newline_mid("extractColumnFromTable: extracted column will have %d lines!", numLines);

	double *nuArray = (double*)malloc(sizeof(double)*numLines);

	for (position = 0; position < numLines; position++)
	{
		nuArray[position] = arrayIpt[position][index];
	}
	fprintfWrapper_newline_tail("extractColumnFromTable: success!");
	return nuArray;
}

int findClosest(double temperature, double pressure, double **arrayIpt)
{
	fprintfWrapper_newline_head("findClosest: start -> T/P [%lf K, %lf MPa]", temperature, pressure);
	fprintfWrapper_newline_tail("findClosest: success!");
	return -1;
}



double **getRectangle(int countRow, int countCol, int xIndex, int yIndex, double **arrayIpt)
{
	fprintfWrapper_newline_head("getRectangle: start -> indices[%d , %d]!", xIndex, yIndex);
	double **nuArray = (double**)malloc(sizeof(double*)*4);
	
	nuArray[0] = (double*)malloc(sizeof(double)*countCol);
	nuArray[1] = (double*)malloc(sizeof(double)*countCol);
	nuArray[2] = (double*)malloc(sizeof(double)*countCol);
	nuArray[3] = (double*)malloc(sizeof(double)*countCol);
	
	nuArray[0] = arrayIpt[xIndex+0];
	nuArray[1] = arrayIpt[xIndex+1];
	nuArray[2] = arrayIpt[xIndex+0];
	nuArray[3] = arrayIpt[xIndex+1];
	fprintfWrapper_newline_tail("getRectangle: success!");
	return nuArray;
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


int countLinesFile(char *filePath)
{
	fprintfWrapper_newline_head("countLinesFile: start -> from %s!", filePath);
	FILE *file = fopen(filePath, "r");
	if (file != NULL)
	{
		fprintfWrapper_newline_mid("countLinesFile: reading from %s", filePath);
		char buf[BUF_SIZE];
		int counter = 0;
		for(;;)
		{
				size_t res = fread(buf, 1, BUF_SIZE, file);
				if (ferror(file)) { return -1; }

				int i;
				for(i = 0; i < res; i++)
                {
                    if (buf[i] == '\n') {counter++ ;}
                }
				if (feof(file)) {break; }
		}
        fprintfWrapper_newline_mid("countLinesFile: closing %s", filePath);
		fclose(file);

		fprintfWrapper_newline_tail("countLinesFile: success -> lines in %s: %d", filePath, counter);
		return counter;
	}
	fprintfWrapper_newline_tail("countLinesFile: failure -> not exist:  %s", filePath);
	return -1;
}

int countLinesFile_noHeader(char *filePath)
{
	fprintfWrapper_newline_head("countLinesFile_noHeader: start -> from %s without a header!", filePath);
	fprintfWrapper_newline_tail("countLinesFile_noHeader: success");
	return countLinesFile(filePath) - 1;
}

int countColumnsFile(char *filePath)
{
	fprintfWrapper_newline_head("countColumnsFile: start -> from %s!", filePath);
	FILE *file = fopen(filePath, "r");
	if (file != NULL)
	{
		fprintfWrapper_newline_mid("countColumnsFile: reading %s", filePath);
		char buf[BUF_SIZE];
		int counter = 0;
		for(;;)
		{
				size_t res = fread(buf, 1, BUF_SIZE, file);
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
	return -1;
}

double **readFromCSV(char *filePath)
{
	fprintfWrapper_newline_head("readFromCSV: start -> from %s!", filePath);
	double stringToNumber;
	int position	= 0;
	int lineNum	    = 0;
	int rowCount 	= countLinesFile_noHeader(filePath);
	int columnCount = countColumnsFile(filePath);

	char *headers[32]; 
	char line[BUF_SIZE_STRING];
	char *data;
	double **nuArray = (double**)malloc(sizeof(double*)*rowCount);

	for (position = 0; position < rowCount; position++)
	{
		nuArray[position] = (double*)malloc(sizeof(double)*columnCount);
	}

	FILE* file = fopen(filePath, "r");
	if (file != NULL)
	{
		fprintfWrapper_newline_mid("readFromCSV: reading %s", filePath);
		while (fgets(line, sizeof(line), file))
		{
			if (line[0] == '\n') {continue;}
			position = 0;

			data = strtok(line, ", ");
			while (data != NULL)
			{
				sscanf(data, "%lf", &stringToNumber); /* na adresu d prevedu hodnotu ze stringu na double */
                //free(data);
				if (lineNum == 0)
				{
					headers[position] = strdup(data); /* aby mi zustala data, porad odkazuji pointery, tak musim string v headeru duplikovat */
					fprintfWrapper_comma("%s", headers[position]);
				}

				if (lineNum > 0  && lineNum < (rowCount*columnCount))
				{
					/*printf("%s: %.3lf, ", headers[position], stringToNumber);*/

                    if (lineNum % ITER_PRINT_FREQUENCY == 0)
                    {
                        fprintfWrapper_comma("%.6lf", stringToNumber);
                    }
					nuArray[lineNum-1][position] = stringToNumber;
				}
				data = strtok(NULL, ", ");
                fflush(stdout);
				position++;
			}

            if (lineNum % ITER_PRINT_FREQUENCY == 0) {fprintfWrapper_newline("");}
            if (lineNum > rowCount*columnCount)
            {
                fprintfWrapper_newline_mid("readFromCSV: Exceeded the length of a file, how?");
                break;
            }
			lineNum++;
            free(line);
		}
		//if (lineNum < 50)   {newline(1);}
        if (lineNum == ITER_PRINT_FREQUENCY)  {fprintfWrapper_newline_mid("readFromCSV: End of table preview!");}

        fprintfWrapper_newline_mid("readFromCSV: closing %s", filePath);
		fclose(file);
		fprintfWrapper_newline_tail("readFromCSV: success -> read %s into a 2D array!", filePath);
		return nuArray;
    }
    
    fprintfWrapper_newline_tail("readFromCSV: failure -> does not exist: %s", filePath);
	return NULL;
}



