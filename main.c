#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>


#define BUF_SIZE 65536
#define BUF_SIZE_STRING 512


void swap(double* a, double* b);
int partition(double arr[], int low, int high);
void quickSort(double arr[], int low, int high);

void swapbsort(int* xp, int* yp);
void bubbleSort(int *arr, int n);


double *arrayGen1D(int count);
double **arrayGen2D(int countRow, int countCol);
double **getRectangle(int countRow, int countCol, int xIndex, int yIndex, double **arrayIpt);
double **readFromCSV(char *path);

double interpolation2D_bilinear(int countRow, int countCol, double **pointset, double x, double y, int selectedColumn);

int countLinesFile(char *path);
int countLinesFile_noHeader(char *path);
int countLinesReadTable(double **arrayIpt);
int countColumnsFile(char *path);
int countColumnsReadTable(double **arrayIpt);
int findClosest(double temperature, double pressure, double **arrayIpt);

void printArray1D(int count, double *arrayIpt);
void printArray2D(int countRow, int countCol, double **arrayIpt);
void newline(int count);

void sin2D(int countRow, int countCol, double **arrayIpt);
char *strConcat(const char *s1, const char *s2);

double *extractColumnFromTable(int index, double **arrayIpt);
double *createUniqueValuesArray(int countRow, double *arrayIpt);


int main()
{
	newline(2);
	char *tablePath		= "table.csv";
	int row_count_csv	= countLinesFile_noHeader(tablePath);
	int column_count_csv	= countColumnsFile(tablePath);

	double **read		= readFromCSV(tablePath);
	printArray2D(row_count_csv, column_count_csv, read);
	newline(1);
	int row_count_array	= countLinesReadTable(read);
	int column_count_array	= countColumnsReadTable(read);
	newline(2);

	double *temperatures	= extractColumnFromTable(0, read);
	double *pressures	= extractColumnFromTable(1, read);
	printArray1D(row_count_array, pressures);
	printArray1D(row_count_array, temperatures);

	quickSort(temperatures, 0, row_count_array -1 ); 
	quickSort(pressures, 0, row_count_array -1 ); 

	//bubbleSort(temperatures, row_count_array);
	//bubbleSort(pressures, row_count_array);
	printArray1D(row_count_array, pressures);
	printArray1D(row_count_array, temperatures);

	double *unique_temperatures	= createUniqueValuesArray(row_count_array, temperatures);
	double *unique_pressures	= createUniqueValuesArray(row_count_array, pressures);


	double **rect	= getRectangle(row_count_csv, column_count_csv, 20, 3, read);
	printArray2D(4, 2, rect);
	newline(2);


	return 1;
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
 


// An optimized version of Bubble Sort
void bubbleSort(int *arr, int n)
{
    int i, j;
    bool swapped;
    for (i = 0; i < n - 1; i++) {
        swapped = false;
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
                swapped = true;
            }
        }
 
        // If no two elements were swapped by inner loop,
        // then break
        if (swapped == false)
            break;
    }
}




















double *arrayGen1D(int count)
{
	printf("Generating 1D array!\n");
	double *nuArray = (double*)malloc(sizeof(double)*count);
	int index;
	for (index = 0; index < count; index++)
	{
		nuArray[index] = (double)index;
	}
	return nuArray;
}

double **arrayGen2D(int countRow, int countCol)
{
	printf("Generating 2D array!\n");
	double **nuArray = (double**)malloc(sizeof(double*)*countRow);
	int indexRow;
	int indexCol;
	for (indexRow = 0; indexRow < countRow; indexRow++)
	{
		nuArray[indexRow] = (double*)malloc(sizeof(double)*countCol);
		for (indexCol = 0; indexCol < countCol; indexCol++)
		{
			nuArray[indexRow][indexCol] = (double)(indexRow+indexCol)/10;
		}
	}
	return nuArray;
}



void printArray1D(int count, double *arrayIpt)
{
	printf("Printing 1D array!\n");
	int index;
	printf("[");
	for (index = 0; index < count; index++)
	{
		printf("%lf, ", arrayIpt[index]);
	}
	printf("]\n");
}

void printArray2D(int countRow, int countCol, double **arrayIpt)
{
	printf("Printing 2D array!\n");
	int indexRow;
	int indexCol;
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
}

void sin2D(int countRow, int countCol, double **arrayIpt)
{
	printf("Sinus 2D array!\n");
	int indexRow;
	int indexCol;
	for (indexRow = 0; indexRow < countRow; indexRow++)
	{
		for (indexCol = 2; indexCol < countCol; indexCol++)
		{
			arrayIpt[indexRow][indexCol] = sin(arrayIpt[indexRow][indexCol]/180.0*3.14159);
		}
	}
}
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
	printf("Bilinear interpolation\n");
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
	return result;
}


double *createUniqueValuesArray(int countRow, double *arrayIpt)
{
	printf("Creating Unique Values Array!\n");
	int uniqueVals  = 1;
	int position_1  = 0;
	int position_2  = 0;
	while (arrayIpt[position_1] == arrayIpt[position_1 + 1])
	{

		uniqueVals++;
		position_1++;
	}
	printf("\tReached the next unique value! Breaking at %d unique values!\n", uniqueVals);
	

	double *nuArray = (double*)malloc(sizeof(double)*uniqueVals);
	printf("\tCreating a 1D array of unique values!\n");
	for (position_1 = 0; position_1 < countRow; position_1++)
	{
		if (arrayIpt[position_1] != nuArray[position_2])
		{
			nuArray[position_2] = arrayIpt[position_1];
			position_2++;
			printf("\t\tAdded %lf to arrayIpt at position %d!\n", arrayIpt[position_1], position_2);
		}
	}
	printf("\tSuccesfully created the unique values array!\n");
	return nuArray;
}


int countLinesReadTable(double **arrayIpt)
{
	printf("Counting lines from a read table!\n");
	int position = 0;
	while (arrayIpt[position])
	{
		position++;
	}
	printf("\tSuccess: The table has: %d lines!\n", position);
	return position;
}


int countColumnsReadTable(double **arrayIpt)
{
	printf("Counting columns from a read table!\n");
	int position = 0;
	while (arrayIpt[0][position])
	{
		position++;
	}
	printf("\tSuccess: The table has: %d columns!\n", position);
	return position;
}


double *extractColumnFromTable(int index, double **arrayIpt)
{
	printf("Extracting column from a read table!\n");
	int position;
	printf("\tExtracting column: getting the numLines!\n");
	int numLines	= countLinesReadTable(arrayIpt);
	printf("\tExtracted column will have %d lines!\n", numLines);
	double *nuArray = (double*)malloc(sizeof(double)*numLines);

	for (position = 0; position < numLines; position++)
	{
		nuArray[position] = arrayIpt[position][index];
	}
	printf("\tSuccesfully extracted!\n");
	return nuArray;
}

int findClosest(double temperature, double pressure, double **arrayIpt)
{
	printf("Finding the closest (T,P) value!\n");
	return -1;
}



double **getRectangle(int countRow, int countCol, int xIndex, int yIndex, double **arrayIpt)
{
	printf("Generating a rectangle around [%d , %d]!\n", xIndex, yIndex);
	double **nuArray = (double**)malloc(sizeof(double*)*4);
	
	nuArray[0] = (double*)malloc(sizeof(double)*countCol);
	nuArray[1] = (double*)malloc(sizeof(double)*countCol);
	nuArray[2] = (double*)malloc(sizeof(double)*countCol);
	nuArray[3] = (double*)malloc(sizeof(double)*countCol);
	
	nuArray[0] = arrayIpt[xIndex+0];
	nuArray[1] = arrayIpt[xIndex+1];
	nuArray[2] = arrayIpt[xIndex+0];
	nuArray[3] = arrayIpt[xIndex+1];
		
	return nuArray;
}

char* strConcat(const char *s1, const char *s2)
{
	const size_t len1 = strlen(s1);
	const size_t len2 = strlen(s2);

	char *result = malloc(len1 + len2 + 1); 

	memcpy(result, s1, len1);
	memcpy(result + len1, s2, len2 + 1);
	printf("\tSuccesfully interpolated!\n");
	return result;
}


int countLinesFile(char *path)
{
	printf("Counting lines from: %s!\n", path); 
	FILE *file = fopen(path, "r");
	if (file != NULL)
	{
		printf("Reading %s\n", path);
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
		fclose(file);
		printf("\tSuccess: The number of lines in %s:\t%d\n", path, counter);
		return counter;
	}
	printf("\tThe file does not exist: %s\n", path);
	return -1;
}

int countLinesFile_noHeader(char *path)
{
	printf("Counting lines from %s without a header!\n", path);
	return countLinesFile(path) - 1;
}

int countColumnsFile(char *path)
{
	printf("Counting columns from %s!\n", path);
	FILE *file = fopen(path, "r");
	if (file != NULL)
	{
		printf("Reading %s\n", path);
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
		printf("\tSuccess: The number of columns in %s:\t%d\n", path, counter);
		return counter;
	}
	printf("\tThe file does not exist: %s\n", path);
	return -1;
}

double **readFromCSV(char *path)
{
	printf("Reading a table from a csv: %s!\n", path);
	double stringToNumber;
	int position	= 0;
	int lineNum	 = 0;
	int rowCount 	= countLinesFile_noHeader(path);
	int columnCount = countColumnsFile(path);

	char *headers[32]; 
	char line[BUF_SIZE_STRING];
	char *data;
	double **nuArray = (double**)malloc(sizeof(double*)*rowCount);

	for (position = 0; position < rowCount; position++)
	{
		nuArray[position] = (double*)malloc(sizeof(double)*columnCount);
	}

	FILE* file = fopen(path, "r");
	if (file != NULL)
	{
		printf("Reading %s\n", path);
		while (fgets(line, sizeof(line), file))
		{
			if (line[0] == '\n') {continue;}
			position = 0;
			newline(1);
			data = strtok(line, ", ");
			while (data != NULL)
			{
				sscanf(data, "%lf", &stringToNumber); /* na adresu d prevedu hodnotu ze stringu na double */

				if (lineNum == 0)
				{
					headers[position] = strdup(data); /* aby mi zustala data, porad odkazuji pointery, tak musim string v headeru duplikovat */
					printf("%s, ", headers[position]);
				}

				if (lineNum > 0)
				{
					/*printf("%s: %.3lf, ", headers[position], stringToNumber);*/
					printf("%.6lf, ", stringToNumber);
					nuArray[lineNum-1][position] = stringToNumber;
				}
				data = strtok(NULL, ", ");
				position++;
			}
			lineNum++;
		}
		newline(1);
		fclose(file);
		printf("\tSuccesfully read the table into a 2D array!\n");
		return nuArray;
	}
	printf("\tThe file does not exist: %s\n", path);
	return NULL;
}
