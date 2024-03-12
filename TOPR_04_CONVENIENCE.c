#include "TOPR_04_CONVENIENCE.h"


double timer(clock_t start, clock_t end)
{
	return ((double)(end-start))/ CLOCKS_PER_SEC;
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

double drand (double loBound, double hiBound)
{
	return ((double)rand() * ( hiBound - loBound )) / (double)RAND_MAX + loBound;
}

double *ardrand(double loBound, double hiBound, int Count)
{
	double *nuArray = (double*)malloc(sizeof(double)*Count);
	int index = 0;
	for (index = 0; index < Count; index++) nuArray[index] = drand(loBound, hiBound);
	return nuArray;
}

bool compare(double compared, double reference)
{
	double lower = reference*(1 - COMPARE_THRESHOLD);
	double upper = reference*(1 + COMPARE_THRESHOLD);

	if ((lower <= compared) && (compared <= upper)) { return true; } 
	else { return false; }
}

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