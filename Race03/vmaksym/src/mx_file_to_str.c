#include "header.h"

char *mx_file_to_str(const char *filename)
{
	if (!filename)
	{
		return NULL;
	}
	int fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		return NULL;
	}
	int lenght = 1;
	char element;
	while (read(fd, &element, 1) != 0)
	{
		if (element != ',')
		{
			if (element != '#' && element != '.' && element != '\n')
			{
				mx_printerr("map error\n");
				exit(-1);
			}
			lenght++;
		}
	}
	if (close(fd) < 0)
	{
		return NULL;
	}
	int fd1 = open(filename, O_RDONLY);
	if (fd1 < 0)
	{
		return NULL;
	}
	int j = 0;
	char *str = mx_strnew(lenght);
	for (int i = 0; read(fd, &element, 1) != 0 ; i++)
	{
		if (element != ',' && element != '\n')
		{
			str[j++] = element;
		}
		else if (element == '\n' && str[j-1] != '\n')
		{
			str[j++] = '\n';
		}
		continue;
	}
	str[lenght] = '\0';
	if (close(fd) < 0)
	{
		return NULL;
	}
	return str;
}

