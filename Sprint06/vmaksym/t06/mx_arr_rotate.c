#include <stdio.h>

void mx_arr_rotate(int *arr, int size, int shift)
{
	int temp;
	int previous;
	if (shift > 0)
	{
		for (int i = 0; i < shift; i++)
		{
			previous = arr[size - 1];
			for (int j = 0; j < size; j++)
			{
				temp = arr[j];
				arr[j] = previous;
				previous = temp;
			}
		}
	}
	else if (shift < 0)
	{
		shift *= -1;
		for (int i = 0; i < shift; i++)
		{
			int j;
			int first = arr[0];
			for(j = 0; j < size - 1; j++)
			{
				arr[j] = arr[j+1];
			}
			arr[j] = first;
		}
	}
}

