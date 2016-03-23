#include <time.h>
#include <stdlib.h>
#include "monster_ai.h"

int const MONSTER_TIMER = 6000;
int MONSTER_SPAWN_TIMER = 6000;// used for timing how often monsters are spawned

time_t t;
int g_spawned_spider = true;
entity *monster_spawn(int type){
	entity *ent_new;//sprite of monster to spawn
	srand((unsigned)time(&t));

	if(MONSTER_SPAWN_TIMER <= 0){
		MONSTER_SPAWN_TIMER = MONSTER_TIMER;
	}

	if(type < 0){
		slog("Unidentified monster spawn type: %i", type);
		return NULL;
	}
	if(type == Monster::grue){
		 ent_new = grue_spawn();
	}
	if(type == Monster::spider01){
		 ent_new = spider01_spawn();
	}
	if(type == Monster::mino)
	{
		ent_new = mino_spawn();
	}
	if(type == Monster::orc)
	{
		ent_new = orc_spawn();
	}
	ent_new->team = TEAM_ENEMY;
	if(ent_new){
		return ent_new;
	}

	//slog("Could not spawn monster type: %i", type);
	return NULL;

}