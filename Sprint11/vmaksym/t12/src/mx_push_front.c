#include "list.h"

void mx_push_front(t_list **list, char *artist, char *name)
{
	if (!*list)
	{
		*list = mx_create_node(artist, name);
		return;
	}
	t_list *newNode = mx_create_node(artist, name);
	newNode->next = *list;
	*list = newNode;
}

