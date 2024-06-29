#include <stddef.h>
char* mx_strchr(const char* s, int c)
{
	while (*s != '\0' && *s != (char)c)
	{
		s++;
	}
	return (*s == (char)c) ? (char*)s : NULL;
}

