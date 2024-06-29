#include "file_to_str.h"

char *mx_strdup(const char *str)
{
	if (str == NULL)
	{
		return NULL;
	}
	int len = mx_strlen(str);
	char *new_string = mx_strnew(len);
	if (new_string == NULL)
	{
		return NULL;
	}
	mx_strcpy(new_string, str);
	return new_string;
}

