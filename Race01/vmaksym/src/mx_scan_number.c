#include "decrypt.h"

bool mx_scan_number(char *value)
{
	if (mx_strlen(value) == 0) return false;
	for (int i = 0; value[i]; i++)
	{
		if ((!mx_isdigit(value[i]) && value[i] != '?') && !(value[i] == '-' && i == 0)) return false;
	}
	return true;
}

