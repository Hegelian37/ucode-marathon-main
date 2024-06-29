#include <unistd.h>
#include <stdio.h>

void mx_only_printable(void)
{
	for (int i = 126; i >= 32; i--)
	{
		write(1, &i, 1);
	}
	char s[2] = "\n";
	write(1, &s, 1);
}

