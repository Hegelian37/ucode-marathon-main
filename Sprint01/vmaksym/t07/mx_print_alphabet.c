#include "mx_printchar.c"
void mx_print_alphabet(void)
{
	for (int i = 65; i < 90; i += 2)
	{
		mx_printchar(i);
		mx_printchar(i + 33);
	}
	write('\n');
}
