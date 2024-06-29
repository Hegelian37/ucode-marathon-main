#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <string.h>

typedef struct s_agent {
	char *name;
	int power;
	int strength;
} t_agent;

int main(int argc, char *argv[]);
int mx_atoi(const char *str);
int mx_count_words(const char* str, char delimiter);
bool mx_isdigit(int c);
bool mx_isspace(char c);
void mx_print_agents(t_agent **agents, int count_of_agents);
void mx_printchar(char c);
void mx_printerr(const char*s);
void mx_printint(int n);
void mx_printstr(const char *s);
int mx_strcmp(const char *s1, const char *s2);
void mx_strdel(char **str);
int mx_strlen(const char *s);
char* mx_strncpy(char* dst, const char* src, int len);
char *mx_strnew(const int size);
char **mx_strsplit(char const *s, char c);

