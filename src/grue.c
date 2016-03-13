#include "grue.h"


void grue_think(entity* self){
	int randomNum;
	Vec2d self_pos =  {self->position.x, self->position.y};
	Vec2d player_pos = {entity_get_player()->position.x, entity_get_player()->position.y};
	Vec2d grue_dir;
	if(!self){
		slog("Entity is NULL. Cannot THINK");
		return;
	}if(self->nextThink > 0){
		//slog("Next Think is %i ", self->nextT4hink);
		self->nextThink -= 1;
		return;
	}
	else{
		self->nextThink = self->thinkRate;
	}
	
	randomNum = rand() % 15;

	if(Vec2dDistanceSQ(self_pos,player_pos) < (500 * 500))
	{
		self->state = STATE_AGGRO;
	}
	else { self->state = STATE_PATROL; }

	if(self->state == STATE_AGGRO)
	{
		int distance;
		grue_dir.x = player_pos.x - self_pos.x;
		grue_dir.y = player_pos.y - self_pos.y;

		distance = Normalize2d(grue_dir);
		grue_dir = VectorScale(grue_dir, grue_dir, GRUE_VELOCITY_AGGRO);
		self->velocity = distance != 0 ? grue_dir : self->velocity;
	}

	else if(self->state == STATE_PATROL){
		
		if(randomNum<= 5){
			self->velocity.x = 5;
		}
		else if(randomNum > 5 && randomNum <=10){
			self->velocity.x = -5;
		}
		else{
			self->velocity.x = 0;
		}
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

}

void grue_update(entity *self){
	Vec2dAdd(self->position, self->velocity, self->position);
	entity_draw(self,self->position.x, self->position.y);
}

void grue_touch(entity *self, entity *other){

}

entity * grue_spawn(){
	Sprite2 * sprite_grue;
	entity * ent_grue = NULL;
	Vec2d pos = {400,400};
	Vec2d vel = {-5,0};
	SDL_Rect boundBox = {grue_frameW*.1f,grue_frameH *.1f, grue_frameW, grue_frameH};
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
	ent_grue->thinkRate = GRUE_THINK_RATE;//think every 30 frames
	ent_grue->touch = grue_touch;
	ent_grue->update = grue_update;
	ent_grue->state = STATE_PATROL;
	ent_grue->boundBox = boundBox;

	return ent_grue;
}
