#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

char *mx_file_to_str(const char *filename);
char *mx_strcat(char *s1, const char *s2);
char* mx_strcpy(char* dst, const char* src);
char *mx_strdup(const char *str);
char *mx_strjoin(char const *s1, char const *s2);
int mx_strlen(const char *s);
char *mx_strnew(const int size);

