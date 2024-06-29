#include "decrypt.h"

char *mx_strndup(const char *s1, int n)
{
	char *dup = NULL;
	int length = mx_strlen(s1);
	if (n < length) length = n;
	dup = mx_strnew(length);
	mx_strncpy(dup, s1, length);
	return dup;
}

