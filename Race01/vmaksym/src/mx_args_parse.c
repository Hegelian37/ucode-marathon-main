#include "decrypt.h"

void mx_args_parse(char **a, char **o, char **b, char **r)
{
	*a = mx_strtrim(*a);
	*o = mx_strtrim(*o);
	*b = mx_strtrim(*b);
	*r = mx_strtrim(*r);
	if(!mx_scan_value(*o, OPERATOR))
	{
		mx_proc_err(INVALID_OPERATION, *o);
		exit(-1);
	}
	if(!mx_scan_value(*a, OPERAND))
	{
		mx_proc_err(INVALID_OPERAND, *a);
		exit(-1);
	}
	if(!mx_scan_value(*b, OPERAND))
	{
		mx_proc_err(INVALID_OPERAND, *b);
		exit(-1);
	}
	if(!mx_scan_value(*r, RESULT))
	{
		mx_proc_err(INVALID_RESULT, *r);
		exit(-1);
	}
}

