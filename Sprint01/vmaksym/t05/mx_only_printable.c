#include "mx_printchar.c"
void mx_only_printable(void)
{
	for (int i = 127; i >= 32; i--)
	{
		mx_printchar(i);
	}
	write('\n');
}
