#include "header.h"

FILE *file = NULL;

FILE *ofile(char* path) {
	file = fopen(path, "r");

	// Проверяем, успешно ли открыт файл
	if (file == NULL) 
	{
		printf("Ошибка при открытии файла.\n");
		return NULL;
	}
	return file;
}

