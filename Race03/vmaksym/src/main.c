#include "header.h"

static void mx_printerror(int argc) {
	if (argc != 6)
	{
		char *err = "usage: ./way_home [file_name] [x1] [y1] [x2] [y2]\n";
		mx_printerr(err);
		exit(-1);
	}
}

int main(int argc, char *argv[])
{
	int file = open(argv[1], O_RDONLY);
	mx_printerror(argc);
	if (file < 0)
	{
		mx_printerr("map does not exist\n");
		exit(-1);
	}
	close(file);
	char *string = mx_file_to_str(argv[1]);   
	int height = 1;
	int width = 0;
	int max = 0;
	int temp = 0;
	while (string[temp] != '\0')
	{
		if (string[temp] == '\n')
		{
			if (string[temp + 1] == '\0') break;
			height++;
			if (max != 0 && width != 0 && width != max)
			{
				mx_printerr("map error\n");
				exit(-1);
			}
			if (max < width) max = width;
			width = 0;
		}
		else width++;
		temp++;
	}
	width = max;
	temp = 0;
	char array[height][max];
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j <= max; j++)
		{
			if (string[temp] == 0) break;
			if (string[temp] == '\n')
			{
				temp++;
				continue;
			}
			array[i][j] = string[temp];
			temp++;
		}
	}
	int x1 = mx_atoi(argv[2]);
	int x2 = mx_atoi(argv[4]);
	int y1 = mx_atoi(argv[3]);
	int y2 = mx_atoi(argv[5]);
	bool status = true;
	bool tmp = true;
	int counter = 1;
	int xt = 0;
	if (x1 < 0 || x2 < 0 || y1 < 0 || y2 < 0 || x1 > width || x2 > width || y1 > height || y2 > height)
	{
		mx_printerr("points are out of map range\n");
		exit(-1);
	}
	if (array[y1][x1] != '.')
	{
		mx_printerr("entry point can not be obstacle\n");
		exit(-1);
	}
	else array[y1][x1] = counter - 1;
	if (array[y2][x2] != '.')
	{
		mx_printerr("exit point can not be obstacle\n");
		exit(-1);
	}
	while (status)
	{
		status = false;
		for (int x1 = 0; x1 < height; x1++)
		{
			for (int y1 = 0; y1 < width; y1++)
			{
				if (array[x1][y1] == (char)(counter - 1))
				{
					if (array[x1 - 1][y1] == '.')
					{
						array[x1 - 1][y1] = counter;
						status = true;
					}
					if (array[x1][y1 + 1] == '.')
					{
						array[x1][y1 + 1] = counter;
						status = true;
					}
					if (array[x1 + 1][y1] == '.')
					{
						array[x1 + 1][y1] = counter;
						status = true;
					}
					if (array[x1][y1 - 1] == '.')
					{
						array[x1][y1 - 1] = counter;
						status = true;
					}
				}
			}
		}
		if (tmp && array[mx_atoi(argv[4])][mx_atoi(argv[5])] != '.')
		{
			xt = counter;
			tmp = false;
		}
		counter++;
	}
	counter -= 2;
	x1 = mx_atoi(argv[4]);
	y1 = mx_atoi(argv[5]);
	if (array[y1][x1] == '.' || array[y1][x1] == '#')
	{
		mx_printerr("route not found\n");
		exit(-1);
	}
	mx_printstr("dist=");
	mx_printint(counter);
	mx_printstr("\n");
	mx_printstr("exit=");
	mx_printint(array[y1][x1]);
	mx_printstr("\n");
	if (array[y1][x1] == (char)xt)
	{
		array[y1][x1] = '*';
	}
	else xt = counter;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (array[i][j] == counter) array[i][j] = 'D';
		}
	}
	while (xt != 0)
	{
		if (array[y2][x2 - 1] == (char)(xt - 1)) x2 -= 1;
		else if (array[y2 + 1][x2] == (char)(xt - 1)) y2 += 1;
		else if (array[y2 - 1][x2] == (char)(xt - 1) && (y2 - 1) > 0) y2 -= 1;
		else if (array[y2][x2 + 1] == (char)(xt - 1) && (x2 + 1) < width) x2 += 1;
		array[y2][x2] = '*';
		xt--;
	}
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (array[i][j] != 'D' && array[i][j] != '.' && array[i][j] != '#' && array[i][j] != '*') array[i][j] = '.';
		}
	}
	if (array[y1][x1] == 'D') array[y1][x1] = 'X';
	file = open("path.txt", O_CREAT | O_WRONLY, S_IWUSR | S_IRUSR);
	int cur_x = 0;
	int cur_y = 0;
	int length = (width + 1 ) * height;
	char buf[length];
	int indx = 0;
	while (cur_y != height)
	{
		if (cur_x == width)
		{
			buf[indx++] = '\n';
			cur_x = 0;
			cur_y++;
		}
		buf[indx++] = array[cur_y][cur_x];
		cur_x++;
	}
	write(file, buf, length);
	close(file);
	return 0;
}

