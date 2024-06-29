#include "minilibmx.h"

int mx_atoi(const char *str)
{
	int res = 0;
	int sign = 1;
	int i = 0;
	while (mx_isspace(str[i]))
	{
		++i;
	}
	if (str[i] == '+' || str[i] == '-') 
	{
		sign = str[i] == '-' ? -1 : 1;
		i++;
	}
	if (res > __INT_MAX__  / 10 || (res == __INT_MAX__ / 10 && str[i] - '0' > 7))
	{
		if (sign == 1)
		{
			return INT_MAX;
		}
		else
        	{
			return INT_MIN;
		}
	}
	while (mx_isdigit(str[i]))
	{
		res = res * 10 + str[i] - '0';
		++i;
	}
	if (!mx_isdigit(str[i]) && str[i] != '\0') 
	{
		res = 0;
	}
	return sign * res;  
}

