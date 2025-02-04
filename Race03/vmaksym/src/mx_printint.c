#include "header.h"

void mx_printint(int n)
{
	int pow = 1;
	if (n < 0)
	{
		mx_printchar('-');
		pow *= -1;
	}
	for (int i = n; i >= 10 || i <= -10; i /= 10)
	{
		pow *= 10;
	}
	while (pow != 0)
	{
		int c = 48 + n / pow;
		mx_printchar(c);
		n %= pow;
		pow /= 10;
	}
}

