#ifndef MX_LIBRATY_INCLUDED
#define MX_LIBRATY_INCLUDED

#include <stdbool.h>
#include <unistd.h>

void mx_printchar(char c);
void mx_printint(int n);
void mx_printstr(const char *str);
char *mx_strcpy(char *dst, const char *src);
int mx_strlen(const char *s);
int mx_strcmp(const char *s1, const char *s2);
bool mx_isdigit(int c);
bool mx_isspace(int c);
int mx_atoi(const char *str);

#endif
