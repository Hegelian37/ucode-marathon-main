#include <stdio.h>
#include <stdlib.h>

int *mx_copy_int_arr(const int *src, int size);
int *mx_del_dup_arr(int *src, int src_size, int *dst_size)
{
	if (src == NULL)
	{
		return NULL;
	}
	*dst_size = 0;
	for (int i = 0; i < src_size; i++)
	{
		int current = src[i];
		int isUnique = 1;
		for (int j = 0; j < *dst_size; j++)
		{
			if (src[i] == src[j])
			{
				isUnique = 0;
				break;
			}
		}
		if (isUnique)
		{
			src[*dst_size] = current;
			(*dst_size)++;
		}
	}
	int *arr = malloc(sizeof(int) * (*dst_size));
	if (arr ==  NULL)
	{
		return NULL;
	}
	arr = mx_copy_int_arr(src, *dst_size);
	return arr;
}

