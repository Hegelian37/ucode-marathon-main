#include "list.h"

int mx_strlen(const char *s)
{
	int l = 0;
	char i = s[0];
	while (i != '\0')
	{
		l++;
		i = s[l];
	}
	return l;
}

