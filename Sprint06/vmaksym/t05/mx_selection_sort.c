#include <stdio.h>

int mx_strlen(const char *s);
int mx_strcmp(const char*s1, const char*s2);
int mx_selection_sort(char **arr, int size)
{
	int count = 0;
	for (int i = 0; i < size - 1; i++)
	{
		int min_pos = i;
		for (int j = i + 1; j < size; j++)
		{
			if (mx_strlen(arr[j]) < mx_strlen(arr[min_pos]))
			{
				min_pos = j;
			}
			else if (mx_strlen(arr[j]) == mx_strlen(arr[min_pos]))
			{
				if (mx_strcmp(arr[j], arr[min_pos]) < 0)
				{
					min_pos = j;
				}
			}
		}
		if (i != min_pos)
		{
			char *temp = arr[min_pos];
			arr[min_pos] = arr[i];
			arr[i] = temp;
			count++;
		}
	}
	return count;
}

