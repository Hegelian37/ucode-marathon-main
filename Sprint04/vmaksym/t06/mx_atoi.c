#include <stdio.h>

int mx_isdigit(int c);
int mx_isspace(char c);
int mx_atoi(const char *str)
{
	int result = 0;
	int sign = 1;
	int int_max = 2147483647;
	int int_min = -2147483647 - 1;
	while (mx_isspace(*str))
	{
		str++;
	}
	if (*str == '-' || *str == '+')
	{
		sign = (*str == '-') ? -1 : 1;
		str++;
	}
	while (mx_isdigit(*str))
	{
		if (result > (int_max - (*str - '0')) / 10)
		{
			return (sign == 1) ? int_max : int_min;
		}
		result = result * 10 + (*str - '0');
		str++;
	}
	return sign * result;
}

