#!/bin/bash
clang -c mx_printchar.c
clang -c mx_printint.c
clang -c mx_printstr.c
clang -c mx_strcpy.c
clang -c mx_strlen.c
clang -c mx_strcmp.c
clang -c mx_isdigit.c
clang -c mx_isspace.c
clang -c mx_atoi.c
ar rcs minilibmx.a mx_printchar.o mx_printint.o mx_printstr.o mx_strcpy.o mx_strlen.o mx_strcmp.o mx_isdigit.o mx_isspace.o mx_atoi.o
rm *.o

