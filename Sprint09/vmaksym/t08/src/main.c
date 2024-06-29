#include "calculator.h"
#include <stdio.h>
#include <stdlib.h>

enum e_operation get_operation_enum(const char *op)
{
	switch(*op)
	{
		case '-':
			return SUB;
		case '+':
			return ADD;
		case '*':
			return MUL;
		case '/':
			return DIV;
		case '%':
			return MOD;
		default:
			return -1; 
	}
}

t_operation *get_operation_structure(enum e_operation op)
{
	t_operation *result_operation = malloc(sizeof(t_operation));
	if (result_operation == NULL)
	{
		return NULL;
	}
	switch(op)
	{
		case ADD:
			result_operation->op = '+';
			result_operation->f = mx_add;
			break;
		case SUB:
			result_operation->op = '-';
			result_operation->f = mx_sub;
			break;
		case MUL:
			result_operation->op = '*';
			result_operation->f = mx_mul;
			break;
		case DIV:
			result_operation->op = '/';
			result_operation->f = mx_div;
			break;
		case MOD:
			result_operation->op = '%';
			result_operation->f = mx_mod;
			break;
		default:
			return NULL;
	}
	return result_operation;
}

bool is_valid_num(const char *num)
{
	if ((*num == '+' || *num == '-') && mx_isdigit(num[1]))
	{
		num++;
	}
	while(*num != '\0')
	{
		if (!mx_isdigit(*num))
		{
			return false;
		}
		num++;
	}
	return true;
}

bool is_valid_operation(const char *op)
{
	if (mx_strlen(op) != 1)
	{
		return false;
	}
	return *op == '+'
		|| *op == '-'
		|| *op == '*'
		|| *op == '/'
		|| *op == '%';
}

int main(int argc, char *argv[])
{
	if (argc != 4)
	{
		mx_printerr("usage: ./calc [operand1] [operation] [operand2]");
		exit(EXIT_FAILURE);
	}
	if (!is_valid_num(argv[1]) || !is_valid_num(argv[3]))
	{
		mx_printerr("error: invalid number");
		exit(INCORRECT_OPERAND);
	}
	if (!is_valid_operation(argv[2]))
	{
		mx_printerr("error: invalid operation");
		exit(INCORRECT_OPERATION);
	}
	enum e_operation operation = get_operation_enum(argv[2]);
	int operand1 = mx_atoi(argv[1]);
	int operand2 = mx_atoi(argv[3]);
	if ((operation == DIV && operand2 == 0) || (operation == MOD && operand2 == 0))
	{
		mx_printerr("error: division by zero");
		exit(DIV_BY_ZERO);
	}
	t_operation *s_operation = get_operation_structure(operation);
	int result = (*s_operation->f)(operand1, operand2);
	mx_printint(result);
	mx_printchar('\n');
	exit(EXIT_SUCCESS);
}

