#include "duplicate.h"
#include <stdlib.h>

t_intarr *mx_del_dup_sarr(t_intarr *src)
{
	if (src == NULL || src->size <= 0 || src->arr == NULL)
	{
		return NULL;
	}
	t_intarr *result = (t_intarr *)malloc(sizeof(t_intarr));
	if (result == NULL)
	{
		return NULL;
	}
	result->arr = (int *)malloc(src->size * sizeof(int));
	if (result->arr == NULL)
	{
		free(result);
		return NULL;
	}
	result->arr[0] = src->arr[0];
	int newSize = 1;
	for (int i = 1; i < src->size; i++)
	{
		int j;
		for (j = 0; j < newSize; j++)
		{
			if (src->arr[i] == result->arr[j]) break;
		}
		if (j == newSize)
		{
			result->arr[newSize] = src->arr[i];
			newSize++;
		}
	}
	result->size = newSize;
	return result;
}

