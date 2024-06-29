#include "decrypt.h"

void mx_ops(char *a, char *o, char *b, char *r)
{
	if (*o == '?')
	{
		mx_ops(a, "+", b, r);
		mx_ops(a, "-", b, r);
		mx_ops(a, "*", b, r);
		mx_ops(a, "/", b, r);
		return;
	}
	int a_length = mx_strlen(a);
	int b_length = mx_strlen(b);
	int r_length = mx_strlen(r);
	bool a_negative = false;
	bool b_negative = false;
	bool r_negative = false;
	if (a[0] == '-')
	{
		a_negative = true;
		a++;
		a_length--;
	} 
	if (b[0] == '-')
	{
		b_negative = true;
		b++;
		b_length--;
	}
	if (r[0] == '-')
	{
		r_negative = true;
		r++;
		r_length--;
	}
	int a_integer = mx_atoi(a);
	int b_integer = mx_atoi(b);
	int r_integer = mx_atoi(r);
	for (long long i = 0; i < mx_pow(10, a_length); i++)
	{
			if (a_integer > 0) i = a_integer;
			char *i_queue = mx_itoa(i);
			int i_length = mx_strlen(i_queue);
			bool i_is_correct = true;
			for (int j = 0; j < a_length - i_length; j++)
			{
				if (mx_isdigit(a[j]) && a[j] != '0')
				{
					i_is_correct = false;
					break;
				}
			}
			for (int j = a_length - i_length, k = 0; j < a_length; j++, k++)
			{
				if (a[j] != '?' && a[j] != i_queue[k])
				{
					i_is_correct = false;
					break;
				}
			}
			free(i_queue);
			i_queue = NULL;
			if (!i_is_correct) continue;
			for (long long j = 0; j < mx_pow(10, b_length); j++)
			{  
			if (b_integer > 0)
			{
				j = b_integer;
			}
			char *j_queue = mx_itoa(j);
			int j_length = mx_strlen(j_queue);
			bool j_is_correct = true;
			for (int p = 0; p < b_length - j_length; p++)
			{
				if (mx_isdigit(b[p]) && b[p] != '0')
				{
					j_is_correct = false;
					break;
				}
			}
			for (int p = b_length - j_length, j_queue_i = 0; p < b_length; p++, j_queue_i++)
			{
				if (b[p] != '?' && b[p] != j_queue[j_queue_i])
				{
					j_is_correct = false;
					break;
				}
			}
			free(j_queue);
			j_queue = NULL;
			if (!j_is_correct) continue;
			for (long long k = 0; k < mx_pow(10, r_length); k++)
			{
				if (r_integer > 0)
				{
					k = r_integer;
				}
				char *k_queue = mx_itoa(k);
				int k_length = mx_strlen(k_queue);
				bool k_is_correct = true;
				for (int n = 0; n < r_length - k_length; n++)
				{
					if (mx_isdigit(r[n]) && r[n] != '0')
					{
						k_is_correct = false;
						break;
					}
				}
				for (int n = r_length - k_length, k_queue_i = 0; n < r_length; n++, k_queue_i++)
				{
					if (r[n] != '?' && r[n] != k_queue[k_queue_i])
					{
						k_is_correct = false;
						break;
					}
				}
				free(k_queue);
				k_queue = NULL;
				if (!k_is_correct) continue;
				if (a_negative) i *= -1;
				if (b_negative) j *= -1;
				if (r_negative) k *= -1;
				if (*o == '+')
				{
					if (i + j == k)
					{
						mx_printint(i);
						mx_printstr(" + ");
						mx_printint(j);
						mx_printstr(" = ");
						mx_printint(k);
						mx_printchar('\n');
					}
				}
				else if (*o == '-')
				{
					if (i - j == k)
					{
						mx_printint(i);
						mx_printstr(" - ");
						mx_printint(j);
						mx_printstr(" = ");
						mx_printint(k);
						mx_printchar('\n');
					}
				}
				else if (*o == '*')
				{
					if (i * j == k)
					{
						mx_printint(i);
						mx_printstr(" * ");
						mx_printint(j);
						mx_printstr(" = ");
						mx_printint(k);
						mx_printchar('\n');
					}
				}
				else if (*o == '/')
				{
					if (j != 0 && i / j == k)
					{
						mx_printint(i);
						mx_printstr(" / ");
						mx_printint(j);
						mx_printstr(" = ");
						mx_printint(k);
						mx_printchar('\n');
					}
				}
				if (a_negative) i *= -1;
				if (b_negative) j *= -1;
				if (r_negative) k *= -1;
				if (r_integer > 0) break;
			}
			if (b_integer > 0) break;
		}
		if (a_integer > 0) break;
	}
	if (a_negative) a--;
	if (b_negative) b--;
	if (r_negative) r--;
}

