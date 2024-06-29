#include "list.h"

void mx_clear_list(t_list **list)
{
	if (*list == NULL || list == NULL)
	{
		return;
	}
	t_list *temp;
	while (*list)
	{
		temp = *list;
		if ((*list)->next)
		{
			*list = (*list)->next;
			free(temp);
		}
		else
		{
			free(temp);
			*list = NULL;
			return;
		}
	}
}

