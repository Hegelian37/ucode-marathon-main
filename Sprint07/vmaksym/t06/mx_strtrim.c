#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool mx_isspace(char c);
int mx_strlen(const char *s);
char *mx_strnew(const int size);
char *mx_strncpy(char *dst, const char *src, int len);
void mx_strdel(char **str);
char *mx_strtrim(const char *str)
{
	if (str == NULL)
	{
		return NULL;
	}
	int strLen = mx_strlen(str);
	int new_strLen = strLen;
	int leadingSpaces = 0;
	for (int i = 0; i < strLen && mx_isspace(str[i]); i++)
	{
		leadingSpaces++;
	}
	int trailingSpaces = 0;
	for (int i = strLen - 1; i >= 0 && mx_isspace(str[i]); i--)
	{
		trailingSpaces++;
	}
	new_strLen -= (leadingSpaces + trailingSpaces);
	char *new_str = mx_strnew(new_strLen);
	if (new_str == NULL)
	{
		return NULL;
	}
	new_str = mx_strncpy(new_str, str + leadingSpaces, new_strLen);
	return new_str;
}

