#include <stdio.h>

int mx_strcmp(const char*s1, const char*s2);
int mx_binary_search(char **arr, int size, const char *s, int *count)
{
	int low = 0;
	int high = size - 1;
	int iter = 1;
	while (low <= high)
	{
		int mid = low + (high - low) / 2;
		int cmp = mx_strcmp(arr[mid], s);
		if (cmp == 0)
		{
			*count = iter;
			return mid;
		}
		else if (cmp < 0)
		{
			low = mid + 1;
			iter++;
		}
		else
		{
			high = mid - 1;
			iter++;
		}
		*count = iter;
	}
	*count = 0;
	return -1;
}

