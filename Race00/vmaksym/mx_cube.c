void mx_printchar(char c);
void mx_cube(int n)
{
	int ls, lp, ipp, pp, rp = 0;
	if (n <= 1) return;
	if (n == 2) { ipp = n + 1; pp = ipp - 1; }
	else { ipp = n / 2 + 2;	pp = ipp - 1; }
	ls = 3 + n + n / 2;
	lp = 2 + ipp + n * 2;
	int spp = lp - 2;
	for (int i = 0; i < ls; i++)
	{
		if (i == 0)
		{
			for (int j = 1; j < lp; j++)
			{
				if (j < ipp) mx_printchar(' ');
				if (j == ipp) mx_printchar('+');
				if (j > ipp && j < lp - 1) mx_printchar ('-');
				if (j == lp - 1) { mx_printchar('+'); pp--; }
			}
		}
		else if (i > 0 && i <= n / 2)
		{
			for (int j = 0; j < lp; j++)
			{
				if (j < pp) mx_printchar(' ');
				if (j == pp) { mx_printchar('/'); spp--; }
				if (j > pp && j < spp)  mx_printchar (' ');
				if (j == spp) mx_printchar('/');
				if (j > spp && j < lp - 2) mx_printchar(' ');
				if (j == lp - 1) mx_printchar('|');
			}
			pp--;
		}
		else if (i == n / 2 + 1)
		{
			for (int j = 0; j < lp; j++)
			{
				if (j == pp) { mx_printchar('+'); spp--; }
				if (j > pp && j < spp) mx_printchar('-');
				if (j == spp) mx_printchar('+');
				if (j > spp && j < lp - 2) mx_printchar (' ');
				if (j == lp - 1) mx_printchar('|');
			}
		}
		else if (i > n / 2 + 1 && i <= n + 1)
		{
			for (int j = 0; j < lp; j++)
			{
				if (i != n + 1)
				{
					if (j == pp) mx_printchar('|');
					if (j > pp && j < spp) mx_printchar(' ');
					if (j == spp) mx_printchar('|');
					if (j > spp && j < lp - 2) mx_printchar (' ');
					if (j == lp - 1) mx_printchar('|');
				}
				else
				{
					if (j == pp) mx_printchar('|');
					if (j > pp && j < spp) mx_printchar(' ');
					if (j == spp) mx_printchar('|');
					if (j > spp && j < lp - 2) mx_printchar (' ');
					if (j == lp - 1) mx_printchar('+');
				}
			}
		}
		else if (i > n + 1 && i != ls - 1)
		{
			rp++;
			for (int j = 0; j < lp; j++)
			{
				if (j == pp) mx_printchar('|');
				if (j > pp && j < spp) mx_printchar (' ');
				if (j == spp) mx_printchar('|');
				if (j > spp && j < lp - 2 - rp) mx_printchar (' ');
				if (j == lp - 1) mx_printchar('/');
			}
		}
		else if (i == ls - 1)
		{
			for (int j = 0; j < lp; j++)
			{
				if (j == pp) mx_printchar('+');
				if (j > pp && j < spp) mx_printchar('-');
				if (j == spp) mx_printchar('+');
			}
		}
		mx_printchar('\n');
	}
}

