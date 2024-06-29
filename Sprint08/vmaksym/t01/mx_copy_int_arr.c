#include "duplicate.h"
#include <stdlib.h>

t_intarr *mx_copy_int_arr(const int *src, int size)
{
	if (src == NULL || size <= 0)
	{
		return NULL;
	}
	t_intarr *result = (t_intarr *)malloc(sizeof(t_intarr)); 
	if (result == NULL)
	{
		return NULL;
	}
	result->arr = (int *)malloc(sizeof(int) * size);
	if (result->arr == NULL)
	{
		free(result);
		return NULL;
	}
	for (int i = 0; i < size; i++)
	{
		result->arr[i] = src[i];
	}
	result->size = size;
	return result;
}

