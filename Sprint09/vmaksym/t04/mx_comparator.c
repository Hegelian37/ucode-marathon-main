#include <stdio.h>
#include <stdbool.h>

int mx_comparator(const int *arr, int size, int x, bool (*compare)(int, int))
{
	if (arr == NULL || compare == NULL || size <= 0)
	{
		return -1;
	}
	for (int i = 0; i < size; i++)
	{
		if ((*compare)(arr[i], x))
		{
			return i;
		}
	}
	return -1;
}

