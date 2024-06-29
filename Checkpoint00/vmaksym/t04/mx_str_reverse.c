void mx_str_reverse(char *s)
{
	int l = 0;
	while(s[l] != '\0')
	{
		l++;
	}
	for (int i = 0; i < l / 2; i++)
	{
		char temp = s[i];
		s[i] = s[l - 1 - i];
		s[l - 1 - i] = temp;
	}
}

