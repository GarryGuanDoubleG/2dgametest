#include <time.h>
#include <stdlib.h>
#include "support_ai.h"

int const G_SUPPORT_TIMER = 6000;
int G_SUPPORT_SPAWN_TIMER = 6000;// used for timing how often monsters are spawned

time_t support_t;

Entity *support_spawn(int type){
	Entity *ent_new;//sprite of monster to spawn
	srand((unsigned)time(&support_t));

	if(G_SUPPORT_SPAWN_TIMER <= 0){
		G_SUPPORT_SPAWN_TIMER = G_SUPPORT_TIMER;
	}

	if(type < 0){
		slog("Unidentified monster spawn type: %i", type);
		return NULL;
	}
	if(type == Support::sara){
		 ent_new = sara_spawn();
	}
	if(type == Support::archer)
	{
		ent_new = archer_spawn();
	}
	else if(type == Support::healer)
	{
		ent_new = angel_spawn();
	}
	if(ent_new){
		ent_new->team = TEAM_PLAYER;
		return ent_new;
	}

	//slog("Could not spawn monster type: %i", type);
	return NULL;

}