#include "TOPR_07_ARRAY_STATISTICS.h"
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