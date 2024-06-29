#include "exterminate_agents.h"
#include <stdlib.h>

void mx_exterminate_agents(t_agent ***agents)
{
	if (agents == NULL || *agents == NULL) return;
	for (int i = 0; (*agents)[i] != NULL; i++)
	{
		t_agent *current_agent = (*agents)[i];
		free(current_agent->name);
		free(current_agent);
	}
	free(*agents);
	*agents = NULL;
}

