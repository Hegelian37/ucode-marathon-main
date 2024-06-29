#include "header.h"

char *mx_strnew(const int size)
{
	if (size < 0)
	{
		return NULL;
	}
	char *s = (char *) malloc((size + 1) * sizeof(char));
	if (s == NULL)
	{
		return NULL;
	}
	for (int i = 0; i <= size; i++)
	{
		s[i] = '\0';
	}
	return s;
}

