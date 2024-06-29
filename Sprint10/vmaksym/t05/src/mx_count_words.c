#include "minilibmx.h"

int mx_count_words(const char* str, char delimiter)
{
	int word_count = 0;
	int in_word = 0;
	while (*str != '\0')
	{
		if (*str != delimiter)
		{
			if (!in_word)
			{
				in_word = 1;
				word_count++;
			}
		}
		else
		{
			in_word = 0;
		}
		str++;
	}
	return word_count;
}

