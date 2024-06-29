#include "file_to_str.h"

int mx_strlen(const char *s)
{
	int l = 0;
	while(*s != '\0')
	{
		l++;
		s++;
	}
	return l;
}

