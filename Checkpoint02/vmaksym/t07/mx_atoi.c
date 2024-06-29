#include <stdbool.h>

bool mx_isdigit(int c)
{
	if ((c >= 48 && c <= 57))
	{
		return true; 
	}
	else
	{
		return false;
	}
}

bool mx_isspace(char c)
{
	if (c == ' ' || c == '\n' || c == '\t' || c == '\v' || c == '\f' || c == '\r')
	{
		return true;
	}
	return false;
}

int mx_atoi(const char *str)
{
	int i = 0;
	int count = 0;
	if (str[0] == '-')
	{
		count = -1;
		i++;
	}
	if (str[0] == '+')
	{
		i++;
	}
	int number = 0;
	for (int d = i; str[d] != '\0'; d++)
	{
		if (mx_isdigit(str[d]))
		{
			number = number * 10 + str[d] - '0';
		}
		else
		{
			return 0;
		}
		if (mx_isspace(str[d]))
		{
			return 0;
		}
	}
	return (count == -1) ? number *= -1 : number;
}

