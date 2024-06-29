#include "decrypt.h"

bool mx_scan_op(char *value)
{
	if(mx_strlen(value) == 0) return false;
	int question_sign_counter = 0;
	for(int i = 0; value[i]; i++)
	{
		if(question_sign_counter > 1) return false;
		if(value[i] == '?') question_sign_counter++;
	}
	char *temp = mx_strtrim(value);
	if(mx_strcmp(temp, "?") != 0 && mx_strcmp(temp,"+") != 0 && mx_strcmp(temp,"-") != 0 && mx_strcmp(temp,"*") != 0 && mx_strcmp(temp,"/") != 0)
	{
		return false;
	}
	free(temp);
	return true;
}

