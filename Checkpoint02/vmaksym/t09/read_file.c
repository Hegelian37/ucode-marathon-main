#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>

int mx_strlen(const char *s)
{
	int i;
	for (i = 0; s[i] != '\0'; i++);
	return i;
}

void mx_printerr(const char *s)
{
	write(2, s, mx_strlen(s));
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		mx_printerr("usage: ./read_file [file_path]\n");
		return -1;
	}
	int fd = open(argv[1], O_RDONLY); 
	if (fd < 0)
	{
		mx_printerr("error\n");
		return -1;
	}
	char element;
	while (read(fd, &element, 1) != 0)
	{
		write(1, &element, 1);
	}
	if (close(fd) < 0)
	{
		mx_printerr("error\n");
		return -1;
	}
	return 0;
}

