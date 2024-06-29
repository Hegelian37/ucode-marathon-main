int mx_strlen(const char *s);
char *mx_strcpy(char *dst, const char *src);
char *mx_strnew(const int size);
char *mx_nbr_to_hex(unsigned long nbr);
char* mx_get_address(void* p)
{
	char* temp = mx_nbr_to_hex((unsigned long)p);
	char* t = mx_strnew(mx_strlen(temp)+2);
	t[0] ='0';
	t[1] ='x';
	for(int i = 2; i<(mx_strlen(temp)+2); i++)
	{
		t[i] = temp[i-2];
	}
	return t;
}

