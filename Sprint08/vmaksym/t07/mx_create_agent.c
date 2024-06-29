#include <stdlib.h>
#include <stddef.h>
#include "agent.h"

char *mx_strdup(const char *str);
t_agent *mx_create_agent(char *name, int power, int strength)
{
	if(name == NULL)
	{
		return NULL;
	}
	t_agent *agent = (t_agent *)(malloc(sizeof(t_agent)));
	agent->name = mx_strdup(name);
	agent->power = power;
	agent->strength = strength;
	return agent;
}

