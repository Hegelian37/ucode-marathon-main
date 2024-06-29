double mx_pow(double n, unsigned int pow)
{
	if (!pow)
	{
		return 1;
	}
	double result = n;
	for (unsigned int i = 0; i < pow - 1; i++)
	{
		result *= n;
	}
	return result;
}

