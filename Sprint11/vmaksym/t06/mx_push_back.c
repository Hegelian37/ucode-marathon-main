#include "list.h"

void mx_push_back(t_list **list, void *data)
{
	if (!*list)
	{
		*list = mx_create_node(data);
		return;
	}
	t_list *current = *list;
	while (current->next != NULL) current = current->next;
	current->next = mx_create_node(data); 
}

