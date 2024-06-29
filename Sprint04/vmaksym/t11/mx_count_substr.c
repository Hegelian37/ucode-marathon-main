char* mx_strchr(const char* s, int c);
int mx_strlen(const char *s);
int mx_strncmp(const char *s1, const char *s2, int n);
char* mx_strstr(const char* s1, const char* s2);
int mx_count_substr(const char *str, const char *sub)
{
	if (*sub == '\0')
	{
		return 0; 
	}
	int sub_len = mx_strlen(sub);
	int count = 0;
	while (*str != '\0')
	{
		char *found = mx_strstr(str, sub);
		if (found != NULL)
		{
			count++;
			str = found + sub_len;
		}
		else
		{
			break;
		}
	}
	return count;
}

