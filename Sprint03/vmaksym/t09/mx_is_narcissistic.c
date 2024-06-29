#include <stdbool.h>

double mx_pow(double n, unsigned int pow);
bool mx_is_narcissistic(int num)
{
	if (num <= 0)
	{
		return false;
	}
	int pow = 0;
	for (int i = num; i != 0; i /= 10)
	{
		pow++;
	}
	int sum = 0;
	for (int i = num; i != 0; i /= 10)
	{
		sum += mx_pow(i % 10, pow);
	}
	return num == sum;
}

