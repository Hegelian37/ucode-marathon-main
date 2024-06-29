#include <stdbool.h>
#include <stdlib.h>

void swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

void mx_sort(int *arr, int size, bool (*f)(int, int))
{
	if (arr == NULL || f == NULL || size <= 0)
	{
		return;
	}
	for (int i = 0; i < size - 1; i++)
	{
		for (int j = 0; j < size - i - 1; j++)
		{
			if ((*f)(arr[j], arr[j + 1]))
			{
				swap(&arr[j], &arr[j + 1]);
			}
		}
	}
}

