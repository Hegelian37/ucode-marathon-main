#include <stdio.h>

int mx_strlen(const char *s);
void mx_printchar(char c);
void mx_printstr(const char *s);
char *mx_strchr(const char *s, int c);
int main(int argc, char *argv[])
{
	if (argc > 0)
	{
		while (mx_strchr(argv[0], '/'))
	        {
			mx_printstr(argv[0]++);
		}
		mx_printstr(argv[0]);
		mx_printchar('\n');
	}
	return 0;
}

