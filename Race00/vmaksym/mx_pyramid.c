void mx_printchar(char c);
void mx_pyramid(int n)
{
	if (n <= 1 || (n % 2 != 0)) return;
	else
	{
		int sm = 1, ls = 0;
		for (int i = 0; i < n - 1; i++)
		{
			mx_printchar(' ');
		}
		mx_printchar('/');
		mx_printchar('\\');
		mx_printchar('\n');
		ls++;
		for (int rm = 1; rm < n / 2; rm++)
		{
			for (int i = 0; i < n - ls - 1; i++)
			{
				mx_printchar(' ');
			}
			mx_printchar('/');
			for (int i = 0; i < sm; i++)
			{
				mx_printchar(' ');
			}
			mx_printchar('\\');
			for (int i = 0; i < ls; i++)
			{
				mx_printchar(' ');
			}
			mx_printchar('\\');
			mx_printchar('\n');
			sm += 2;
			ls++;
		}
		for (int sb = 1; sb < n / 2; sb++)
		{
			for (int i = 0; i < n - ls - 1; i++)
			{
				mx_printchar(' ');
			}
			mx_printchar('/');
			for (int i = 0; i < sm; i++)
			{
				mx_printchar(' ');
			}
			mx_printchar('\\');
			for (int i = 0; i < n - ls - 1; i++)
			{
				mx_printchar(' ');
			}
			mx_printchar('|');
			mx_printchar('\n');
			sm += 2;
			ls++;
		}
		mx_printchar('/');
		for (int i = 0; i < 2 * n - 3; i++)
		{
			mx_printchar('_');
		}
		mx_printchar('\\');
		mx_printchar('|');
		mx_printchar('\n');
	}
}

