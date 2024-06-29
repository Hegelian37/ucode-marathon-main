#include <stdbool.h>

bool mx_isdigit(int c);
bool mx_isspace(char c);
int mx_atoi(const char *str);
void mx_printchar(char c);
void mx_printint(int n);
int main(int argc, char *argv[])
{
	int sum = 0;
	for (int i = 1; i <= argc - 1; i++)
	{
		sum += mx_atoi(argv[i]);
	}
	mx_printint(sum);
}

