#include "list.h"

t_list *mx_create_node(void *data)
{
	t_list *new_node = malloc(sizeof(t_list));
	new_node->next = NULL;
	new_node->data = data;
	return new_node;
}

