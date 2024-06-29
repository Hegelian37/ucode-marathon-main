#include <stdio.h>
#include <stdlib.h>

int mx_count_words(const char* str, char delimiter);
char *mx_strnew(const int size);
char* mx_strncpy(char* dst, const char* src, int len);
void mx_strdel(char **str);
char **mx_strsplit(char const *s, char c)
{
	if (s == NULL)
	{
		return NULL;
	}
	int wordCnt = mx_count_words(s, c);
	char **arr = malloc((wordCnt + 1) * sizeof(char *));
	if (arr == NULL)
	{
		return NULL;
	}
	int pos = 0;
	for (int i = 0; i < wordCnt; i++)
	{
		int len = 0;
		while (s[pos] == c)
		{
		    pos++;
		}
		while (s[pos + len] != c && s[pos + len] != '\0')
		{
		    len++;
		}
		arr[i] = mx_strnew(len);
		mx_strncpy(arr[i], s + pos, len);
		pos += len;
	}
	arr[wordCnt] = NULL;
	return arr;
}

