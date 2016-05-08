#include "sara.h"

void sara_update(entity *self)
{
	Vec2d new_pos = {self->position.x + self->velocity.x, self->position.y + self->velocity.y};

	if(self->state == STATE_AGGRO)
	{
		//self->path = getPath(self->aggro_range, &self->position, self->boundBox,entity_get_player()->boundBox, &entity_get_player()->position,self->path);
		self->followPath(self);
		Vec2dAdd(self->position, self->velocity, self->position);
	//	aStar_search(self->aggro_range, self->position, entity_get_player()->position,self->path);
	}
	else if(!tile_collision(new_pos, self->boundBox))
	{
		self->position = new_pos;
	}
//	self->frame_horizontal = (self->frame_horizontal + 1) % self->sprite->fpl;
	entity_draw(self,self->position.x, self->position.y);
}

void sara_attack_mode(entity *self, entity * other)
{
	if(!self || !other)
	{
		slog("No Sara or Enemy");
		return;
	}
	if(other->sprite)
	{
		slog("Sara is attacking %s", other->sprite->filename);
		other->health -= self->damage;
	}
}

void sara_touch(entity *self, entity *other)
{
	if(!self || !other)
	{
		slog("No Sara or Enemy");
		return;
	}
	//other->health -= self->damage;
}
void sara_think(entity *self)
{
	int randomNum;
	int distance;
	Vec2d self_pos =  {self->position.x, self->position.y};	
	entity *enemy; 
	Vec2d enemy_pos; 
	Vec2d sara_dir;

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
	enemy = ent_find_nearest_enemy(self);
	
	randomNum = rand() % 15;
	if(enemy)
	{
		if(Vec2dDistanceSQ(self_pos,enemy->position) < (TILE_WIDTH * TILE_HEIGHT) * self->aggro_range *self->aggro_range)
		{
			slog("sara state AGGRO");
			self->state = STATE_AGGRO;
		}
		else { self->state = STATE_PATROL; }
	}

	if(self->state == STATE_AGGRO)
	{
		self->velocity.x = 0;
		self->velocity.y = 0;
		slog("Attack Mode");
		sara_attack_mode(self, enemy);
	//	self->path = getPath(self->aggro_range, &self->position, self->boundBox,entity_get_player()->boundBox, &entity_get_player()->position,self->path);
	//	aStar_search(self->aggro_range, self->position, entity_get_player()->position,self->path);
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
/*
	if(self->velocity.x >= 0)
	{
		self->frame_vertical  = 0;
	}
	else
	{
		self->frame_vertical = 1;
	}

	*/
	//setting which sprite to use depending on direction
}

void sara_onDeath(entity *self)
{

}

entity * sara_spawn()
{
	Sprite * sprite_sara;
	entity * ent_sara = NULL;
	Tile start = tile_start();
	Vec2d pos = {start.mBox.x,start.mBox.y};
	Vec2d vel = {0,0};
	SDL_Rect boundBox = {MINO_FRAMEW*.1f,MINO_FRAMEH *.2f, MINO_FRAMEW*.8, MINO_FRAMEH*.8};

	if( (G_MONSTER_SPAWN_TIMER % SPIDER01_TIMER) != 0)
	{	
		return NULL;
	}

	slog("sara: x%f y%f", pos.x, pos.y);
	sprite_sara = sprite_load(SPRITE_SARA_FILEPATH, SARA_IMAGEW, SARA_IMAGEH, SARA_FRAMEW, SARA_FRAMEH);
	sprite_sara->fpl = SARA_FPL;//2 frames per line
	ent_sara = entity_load(sprite_sara, pos, 100, 25, STATE_PATROL);

	if(!ent_sara){
		slog("Could not spawn Spider01");
		return NULL;
	}
	//each frame is a single direction
	ent_sara->frame = 0;

	ent_sara->velocity = vel;
	ent_sara->think = sara_think;
	ent_sara->nextThink = 1;
	ent_sara->thinkRate = SPIDER01_THINK_RATE;//think every 30 frames
	ent_sara->touch = sara_touch;
	ent_sara->update = sara_update;
	ent_sara->onDeath = sara_onDeath;
	ent_sara->state = STATE_PATROL;
	ent_sara->boundBox = boundBox;
	ent_sara->aggro_range = SARA_AGGRO_RANGE;//10 tiles
	ent_sara->damage = 2;

	return ent_sara;
}