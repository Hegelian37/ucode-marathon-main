int mx_popular_int(const int *arr, int size)
{
	if (size <= 0)
	{
		return 0;
	}
	int most_common = arr[0];
	int max_count = 1;
	for (int i = 0; i < size; i++)
	{
		int current = arr[i];
		int current_count = 1;
		for (int j = i + 1; j < size; j++)
		{
			if (arr[j] == current)
			{
				current_count++;
			}
		}
		if (current_count > max_count)
		{
			most_common = current;
			max_count = current_count;
		}
	}
	return most_common;
}

