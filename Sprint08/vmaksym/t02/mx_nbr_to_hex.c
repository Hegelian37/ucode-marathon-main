char *mx_strnew(const int size);
char *mx_nbr_to_hex(unsigned long nbr)
{
	int size = 1;
	char *result = mx_strnew(size);
	result[0] = '\0';
	while(nbr != 0)
	{
		unsigned long t = nbr % 16;
		if(t < 10)
		{
			t += '0';
		}
		else
		{
			t += 'a' - 10;
		}
		char *s = mx_strnew(size++);
		s[0] = t;
		for(int i = 0; i < size - 1; i++)
		{
			s[i + 1] = result[i];
		}
		result = s;
		nbr /= 16;
	}
	return result;
}

