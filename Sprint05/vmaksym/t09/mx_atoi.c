#include <stdbool.h>

bool mx_isdigit(int c);
bool mx_isspace(char c);
int mx_atoi(const char *str)
{
	int result = 0;
	int sign = 1;
	while (mx_isspace(*str))
	{
		str++;
	}
	if (*str == '-' || *str == '+')
	{
		sign = (*str++ == '-') ? -1 : 1;
	}
	while (mx_isdigit(*str))
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	return sign * result;
}

