#include "decrypt.h"

void mx_proc_err(int error, char *value)
{
	char *message = NULL;
	char newline = '\n';
	switch (error)
	{
		case INVALID_OPERAND:
			message = MX_INVALID_OPERAND_STR;
			break;
		case INVALID_OPERATION:
			message = MX_INVALID_OPERATION_STR;
			break;
		case INVALID_RESULT:
			message = MX_INVALID_RESULT_STR;
			break;
	}
	write(2, message, mx_strlen(message));
	write(2, value, mx_strlen(value));
	write(2, &newline, 1);
}

