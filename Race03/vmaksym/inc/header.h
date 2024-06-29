#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

bool mx_isdigit(int c);
void mx_printint(int n);
void mx_printchar(char c);
int mx_atoi(const char *str);
int mx_strlen(const char *c);
void mx_printerr(const char *s);
void mx_printstr(const char *s);
char *mx_strnew(const int size);
char *mx_file_to_str(const char *filename);

