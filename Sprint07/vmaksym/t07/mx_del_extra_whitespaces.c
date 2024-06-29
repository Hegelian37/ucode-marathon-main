#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool mx_isspace(char c);
int mx_strlen(const char *s);
char *mx_strnew(const int size);
char *mx_strncpy(char *dst, const char *src, int len);
char *mx_strtrim(const char *str);
void mx_strdel(char **str);
char *mx_del_extra_whitespaces(const char *str)
{
	char *trim_str = mx_strtrim(str);
	if (trim_str == NULL)
	{
		return NULL;
	}
	int len = 0;
	int wordCnt = 0;
	bool wasSpace = false;
	for (int i = 0; trim_str[i] != '\0'; i++)
	{
		if (mx_isspace(trim_str[i]) && !wasSpace)
		{
			wordCnt++;
			wasSpace = true;
		}
		else if (!mx_isspace(trim_str[i]))
		{
			wasSpace = false;
			len++;
		}
	}
	char *new_str = mx_strnew(len + wordCnt);
	if (new_str == NULL)
	{
		return NULL;
	}
	int cnt = 0;
	for (int i = 0; trim_str[i] != '\0'; i++)
	{
		if (mx_isspace(trim_str[i]) && !wasSpace)
		{
			new_str[cnt++] = ' ';
			wasSpace = true;
		}
		else if (!mx_isspace(trim_str[i]))
		{
			wasSpace = false;
			new_str[cnt++] = trim_str[i];
		}
	}
	mx_strdel(&trim_str);
	return new_str;
}

