#include <unistd.h>

void mx_printint(int n)
{
	char s[1] = "-";
	if (n < 0)
	{
		write(1, &s, 1);
		n = -n;
	}
	int div = 1;
	while (n / div >= 10)
	{
		div *= 10;
	}
	while (div > 0)
	{
		int dig = n / div + '0';
		write(1, &dig, 1);
		n %= div;
		div /= 10;
	}
}

