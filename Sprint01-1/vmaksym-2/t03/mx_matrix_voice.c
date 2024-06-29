#include <unistd.h>
#include <stdio.h>
void mx_matrix_voice(void)
{
	char message[] = "\a";
	write(1, message, 1);
}

