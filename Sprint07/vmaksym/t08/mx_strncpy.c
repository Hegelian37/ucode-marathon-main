char* mx_strncpy(char* dst, const char* src, int len)
{
	char* original_dst = dst;
	while (*src != '\0' && len > 0)
	{
		*dst++ = *src++;
		len--;
	}
	*dst++ = '\0';
	return original_dst;
}

