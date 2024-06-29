#include <time.h>

double mx_timer(void (*f)())
{
	if (f == NULL)
	{
		return -1;
	}
	clock_t start;
	clock_t end;
	start = clock();
	(*f)();
	end = clock();
	if (start == (clock_t)-1 || end == (clock_t)-1)
	{
		return -1;
	}
	double execution_time = ((double)(end - start)) / CLOCKS_PER_SEC;
	return execution_time;
}

