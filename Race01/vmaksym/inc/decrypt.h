#ifndef DECRYPT_H
#define DECRYPT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <limits.h>

#define MX_NO_ARGUMENTS "usage: ./part_of_the_matrix [operand1] [operation] [operand2] [result]\n"
#define MX_INVALID_OPERATION_STR "Invalid operation: "
#define MX_INVALID_OPERAND_STR "Invalid operand: " 
#define MX_INVALID_RESULT_STR "Invalid result: "

enum e_error {
	INVALID_OPERATION,
	INVALID_OPERAND,
	INVALID_RESULT
};

enum e_type {
	OPERAND,
	OPERATOR,
	RESULT
};

int main(int argc, char **argv);
void mx_args_parse(char **a, char **o, char **b, char **r);
int mx_atoi(const char *str);
bool mx_isdigit(int c);
bool mx_isspace(char c);
char *mx_itoa(int number);
void mx_ops(char *a, char *o, char *b, char *r);
int mx_pow(int num, int pow);
void mx_printchar(char c);
void mx_printerr(const char *s);
void mx_printint(int num);
void mx_printstr(const char *s);
void mx_proc_err(int error, char *value);
bool mx_scan_number(char *value);
bool mx_scan_op(char *value);
bool mx_scan_value(char *value, int type);
int mx_strcmp(const char *s1, const char *s2);
char *mx_strcpy(char *dst, const char *src);
int mx_strlen(const char *s);
char* mx_strncpy(char* dst, const char* src, int len);
char *mx_strndup(const char *s1, int n);
char *mx_strnew(const int size);
void mx_str_reverse(char *s);
char *mx_strtrim(const char *str);
void mx_swap_char(char *s1, char *s2);

#endif
