#include "TOPR_11_ARRAY_SEARCHING.h"

int findClosest1D(double variable, Array1D arrayIpt)
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

int findClosest2D(double independentVariable1, double independentVariable2, Array1D uniqueList1, Array1D uniqueList2)
{
	fprintfWrapper_newline_head("findClosest2D_efficient: start -> [%lf, %lf]", independentVariable1, independentVariable2);
	int closest_1		= findClosest1D_efficient(independentVariable1, uniqueList1);
	int closest_2		= findClosest1D_efficient(independentVariable2, uniqueList2);
	int result			= closest_1*uniqueList2.count + closest_2;
	fprintfWrapper_newline_tail("findClosest2D_efficient: success! -> index: %d", result);
	return result;
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