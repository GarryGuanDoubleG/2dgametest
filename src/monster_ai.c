#include "monster_ai.h"
#include <time.h>
#include <stdlib.h>

int const MONSTER_TIMER = 600;
int MONSTER_SPAWN_TIMER = 6000;// used for timing how often monsters are spawned

#define GRUE_TIMER 150
time_t t;

void grue_think(entity* self){
	int randomNum;

	if(!self){
		slog("Entity is NULL. Cannot THINK");
		return;
	}if(self->nextThink > 0){
		//slog("Next Think is %i ", self->nextThink);
		self->nextThink -= 1;
		return;
	}
	else{
		self->nextThink = self->thinkRate;
	}
	
	randomNum = rand() % 15;

	if(self->state == STATE_PATROL){

		/*if(Vec2dDistanceSQ(self->position, player->position) > (100 * 100)){//100 pixels away
			self->state = STATE_AGGRO;
		}*/
		
		if(randomNum<= 5)
			self->velocity.x = 5;
		else if(randomNum > 5 && randomNum <=10)
			self->velocity.x = -5;
		else
			self->velocity.x = 0;
		randomNum = rand() % 15;
		self->velocity.y = randomNum <= 5 ? 5 : -5;
			
		if(randomNum > 10 && randomNum <= 15){
			self->velocity.y = 0;
		}
	}

	//setting which sprite to use depending on direction
	if(self->velocity.x != 0){
		if(self->velocity.x >= 0){
			self->frame_horizontal = 0;
			self->frame_vertical = 0;
		}
		else
		{
			self->frame_horizontal = 0;
			self->frame_vertical = 1;
		}
	}
	else{
		if(self->velocity.y >= 0){
			self->frame_horizontal = 1;
			self->frame_vertical = 1;
		}
		else
		{
			self->frame_horizontal = 1;
			self->frame_vertical = 0;
		}		
	}

	Vec2dAdd(self->position, self->velocity, self->position);
}

void grue_update(entity *self){
	entity_draw(self,self->position.x, self->position.y);
}

void grue_touch(entity *self, entity *other){

}

entity * grue_spawn(){
	Sprite2 * sprite_grue;
	entity * ent_grue = NULL;
	Vec2d pos = {400,400};
	Vec2d vel = {-5,0};

	SDL_Rect boundBox = {grue_frameW*.05f,0, grue_frameW*.95, grue_frameH};

	if( (MONSTER_SPAWN_TIMER % (GRUE_TIMER)) != 0){
		return NULL;
	}
	slog("Timer: %i", MONSTER_SPAWN_TIMER);
	slog("Monster Spawn Timer mod MonsterSpawn Timer is %i " , (MONSTER_SPAWN_TIMER % (GRUE_TIMER)));
	sprite_grue = sprite_load(sprite_grue_filepath, grue_imageW, grue_imageH, grue_frameW, grue_frameH);
	sprite_grue->fpl = 1;//2 frames per line
	ent_grue = entity_load(sprite_grue, pos, 150, 25, STATE_PATROL);

	if(!ent_grue){
		slog("Could not spawn GRUE");
		return NULL;
	}
	//each frame is a single direction
	ent_grue->frame_horizontal = 0;
	ent_grue->frame_vertical = 0;
	ent_grue->velocity = vel;
	ent_grue->think = grue_think;
	ent_grue->nextThink = 1;
	ent_grue->thinkRate = 2;//think every 30 frames
	ent_grue->touch = grue_touch;
	ent_grue->update = grue_update;
	ent_grue->state = STATE_PATROL;
	ent_grue->boundBox = boundBox;

	return ent_grue;
}

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
	if(ent_new){
		return ent_new;
	}

	//slog("Could not spawn monster type: %i", type);
	return NULL;

}