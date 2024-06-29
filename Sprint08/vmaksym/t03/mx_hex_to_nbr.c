unsigned long mx_hex_to_nbr(const char *hex)
{
	unsigned long step = 1;
	unsigned long itog = 0;
	char *str = (char *)hex;
	char *str2 = str;
	while (*str != '\0')
	{
		str++;
	}
	while (str-- != str2)
	{
		if (*str >= '0' && *str <= '9')
		{
			itog += (*str - '0') * step;
			step *= 16;
		}
		else
		{
			if (*str >= 'a' && *str <= 'f')
			{
				itog += (*str - 'a' + 10) * step;
				step *= 16;
			}
			else
			{
				if (*str >= 'A' && *str <= 'F')
				{
					itog+=(*str - 'A' + 10)*step;
					step *= 16;
				}
				else
				{
					return 0;
				}
			}
		}
	}
	return itog;
}

