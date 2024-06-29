#include "list.h"

int mx_list_size(t_list *list)
{
	int count = 0;
	t_list *temp = list;
	while (temp != NULL)
	{
		count += 1;
		temp = temp->next;
	}
	return count; 
}

