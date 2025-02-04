#include "list.h"

void mx_foreach_list(t_list *list, void(*f)(t_list *node))
{
	if (f == NULL || list == NULL) return;
	t_list *node = list;
	while (node != NULL)
	{
		(*f)(node);
		node = node->next;
	}
}

