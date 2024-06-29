void mx_printchar(char c);
void mx_printint(int n);

void mx_print_arr_int(const int *arr, int sz)
{
	for (int i = 0; i < sz; i++)
	{
		mx_printint(arr[i]);
		mx_printchar('\n');
	}
}

