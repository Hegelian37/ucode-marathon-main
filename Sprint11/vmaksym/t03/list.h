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

void mx_pop_back(t_list **list);

