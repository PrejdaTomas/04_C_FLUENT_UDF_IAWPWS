#include "TOPR_10_ARRAY_COPYING.h"

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