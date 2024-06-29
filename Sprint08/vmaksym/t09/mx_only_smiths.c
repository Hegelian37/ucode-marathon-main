#include <stdlib.h>
#include "agent.h"

int mx_strcmp(const char *s1, const char *s2);
t_agent *mx_create_agent(char *name, int power, int strength);
t_agent** mx_only_smiths(t_agent **agents, int strength)
{
	int temp = 0;
	for(int i = 0; agents[i] != NULL; i++)
	{
		if(mx_strcmp(agents[i]->name, "Smith") == 0 && agents[i]->strength<strength)
		{
			temp++;
		}
	}
	if(temp == 0) return NULL;
	temp = 0;
	t_agent **only_smiths = (t_agent **)(malloc((temp + 1)*sizeof(t_agent *)));
	for(int i = 0; agents[i] != NULL; i++)
	{
		if(mx_strcmp(agents[i]->name, "Smith") == 0 && agents[i]->strength < strength)
		{
			only_smiths[temp] = mx_create_agent(agents[i]->name, agents[i]->power, agents[i]->strength);
			temp++;
		}
	}
	only_smiths[temp] = NULL;
	return only_smiths;
}

