#include <stdio.h>

int mx_sum_digits(int num)
{
	num = (num < 0) ? -num : num;
	int sum = 0;
	while (num > 0)
	{
		sum += num % 10;
		num /= 10;
	}
	return sum;
}

