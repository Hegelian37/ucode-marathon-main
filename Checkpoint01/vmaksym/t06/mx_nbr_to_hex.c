#include <stdio.h>
#include <stdlib.h>

char *mx_nbr_to_hex(unsigned long num)
{
	int length = 0;
	unsigned long temp = num;
	do
	{
		temp /= 16;
		length++;
	} while (temp != 0);
	char *hexStr = (char *)malloc((length + 1) * sizeof(char));
	if (hexStr == NULL)
	{
		return NULL;
	}
	int index = 0;
	do
	{
		int remainder = num % 16;
		hexStr[index++] = (remainder < 10) ? ('0' + remainder) : ('a' + remainder - 10);
		num /= 16;
	} while (num != 0);
	hexStr[index] = '\0';
	int start = 0, end = index - 1;
	while (start < end)
	{
		char temp = hexStr[start];
		hexStr[start] = hexStr[end];
		hexStr[end] = temp;
		start++;
		end--;
	}
	return hexStr;
}

