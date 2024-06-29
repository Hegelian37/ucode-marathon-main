#include "minilibmx.h"

int main(int argc, char *argv[])
{
	int err_stat = 0;
	if (argc != 2 && err_stat < 1)
	{
		mx_printerr("usage: ./read_file [file_path]\n");
		err_stat++;
	}
	int file_desc = open(argv[1], O_RDONLY);
	int size;
	char buffer[1];
	if (file_desc < 0 && err_stat < 1)
	{
		mx_printerr("error\n");
		err_stat++;
	}
	size = read(file_desc, buffer, 1);
	if (err_stat < 1)
	{	
		while (size)
		{
			buffer[size] = '\0';
			write(1, buffer, 1);
			size = read(file_desc, buffer, 1);
		}
	}
	if (close(file_desc) < 0 && err_stat < 1)
	{
		mx_printerr("error\n");
		err_stat++;
	}
	return 0;
}

