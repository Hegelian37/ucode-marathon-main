#include <unistd.h>

int main(int argc, char *argv[])
{
	char *progName = argv[0];
	while (*progName != '\0')
	{
		write(1, progName, 1);
		progName++;
	}
	write(1, "\n", 1);
	int argCount = argc;
	char argCountStr[12];
	int index = 0;
	if (argCount < 0)
	{
		write(1, "-", 1);
		argCount = -argCount;
	}
	do
	{
		argCountStr[index++] = argCount % 10 + '0';
		argCount /= 10;
	} while (argCount > 0);
	while (--index >= 0)
	{
		write(1, &argCountStr[index], 1);
	}
	write(1, "\n", 1);
	return 0;
}
