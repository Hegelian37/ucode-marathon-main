#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/stat.h>

typedef struct s_list {
	void *data;
	struct s_list *next;
} t_list;

t_list *mx_sort_list(t_list *list, bool (*cmp)(void *a, void *b));

