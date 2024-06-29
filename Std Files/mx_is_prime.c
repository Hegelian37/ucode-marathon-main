#include <stdbool.h>

bool mx_is_prime(int num)
{
	if (num <= 1)
	{
		return false;
	}
	for (int i = 2; i <= num / 2; i++)
	{
		if (!(num % i))
		{
			return false;
		}
	}
	return true;
}

