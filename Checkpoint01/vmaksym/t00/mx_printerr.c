#include <unistd.h>

void mx_printerr(const char *s)
{
	while (*s != '\0')
	{
		write(2, s, 1);
		s++;
	}
}

