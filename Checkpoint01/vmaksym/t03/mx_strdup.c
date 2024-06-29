#include <stdio.h>
#include <stdlib.h>

char *mx_strdup(const char *str)
{
	if (str == NULL)
	{
		return NULL;
	}
	size_t length = 0;
	while (str[length] != '\0')
	{
		length++;
	}
	char *duplicate = (char *)malloc((length + 1) * sizeof(char));
	if (duplicate != NULL)
	{
		for (size_t i = 0; i <= length; i++)
		{
			duplicate[i] = str[i];
		}
		return duplicate;
	}
	else
	{
		return NULL;
	}
}

