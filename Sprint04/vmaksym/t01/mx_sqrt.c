int mx_sqrt(int x)
{
	if (x <= 1)
	{
		return x;
	}
	int low = 1, high = x, result = 0;
	while (low <= high)
	{
		int mid = low + (high - low) / 2;
		if (mid <= x / mid)
		{
			low = mid + 1;
			result = mid;
		}
		else
		{
			high = mid - 1;
		}
	}
	return (result * result == x) ? result : 0;
}

