#include "decrypt.h"

bool mx_scan_value(char *value, int type)
{
	int valid = 0;
	switch(type)
	{
		case OPERAND:
			valid = mx_scan_number(value);
			break;
		case OPERATOR:
			valid = mx_scan_op(value);
			break;
		case RESULT:
			valid = mx_scan_number(value);
		default:
			break;
	}
	return valid;
}

