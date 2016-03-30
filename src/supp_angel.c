#include "supp_angel.h"

void angel_update(entity *self)
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
	self->frame_horizontal = (self->frame_horizontal + 1) % self->sprite->fpl;
	entity_draw(self,self->position.x, self->position.y);
}

void angel_heal_mode(entity *self, entity *other)
{
	if(!self || !other)
	{
		slog("No Angel or friend");
	}
	if(other->sprite)
	{
		slog("Health before %i", other->health);
		other->health += self->damage;
		if(other->health > other->maxhealth)
		{
			other->health = other->maxhealth;
		}
		slog("Angel Healed %s", other->sprite->filename);
		slog("Health after %i", other->health);
	}

}

void angel_touch(entity *self, entity *other)
{
	if(!self || !other)
	{
		slog("No angel or Enemy");
		return;
	}
	//other->health -= self->damage;
}
void angel_think(entity *self)
{
	int randomNum;
	int distance;
	Vec2d self_pos =  {self->position.x, self->position.y};	
	entity *teammate; 
	Vec2d angel_dir;

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
	teammate = ent_find_nearest_teammate(self);
	if(teammate)
	{
		if(Vec2dDistanceSQ(self_pos,teammate->position) < (TILE_WIDTH * TILE_HEIGHT) * self->aggro_range *self->aggro_range)
		{
			slog("angel state AGGRO");
			if(teammate->health < teammate->maxhealth)
			{			
				self->state = STATE_AGGRO;
			}

		}
		else { self->state = STATE_PATROL; }
	}

	if(self->state == STATE_AGGRO)
	{
		self->velocity.x = 0;
		self->velocity.y = 0;
		angel_heal_mode(self, teammate);
		slog("Head Mode");
		self->path = getPath(self->aggro_range, &self->position, self->boundBox,entity_get_player()->boundBox, &entity_get_player()->position,self->path);
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

	if(self->velocity.x != 0){
		if(self->velocity.x >= 0){
			self->frame_horizontal = (self->frame_horizontal + 1) % self->sprite->fpl;
			self->frame_vertical = 2;//hard coded based on sprite
		}
		else
		{
			self->frame_horizontal = (self->frame_horizontal + 1) % self->sprite->fpl;
			self->frame_vertical = 1;
		}
	}
	else{
		if(self->velocity.y >= 0){
			self->frame_horizontal = (self->frame_horizontal + 1) % self->sprite->fpl;
			self->frame_vertical = 3;
		}
		else
		{
			self->frame_horizontal = (self->frame_horizontal + 1) % self->sprite->fpl;
			self->frame_vertical = 1;
		}		
	}
	//setting which sprite to use depending on direction
}

void angel_onDeath(entity *self)
{

}

entity * angel_spawn()
{
	Sprite2 * sprite_angel;
	entity * ent_angel = NULL;
	Tile start = tile_start();
	Vec2d pos = {start.mBox.x,start.mBox.y};
	Vec2d vel = {0,0};
	SDL_Rect boundBox = {MINO_FRAMEW*.1f,MINO_FRAMEH *.2f, MINO_FRAMEW*.8, MINO_FRAMEH*.8};

	if( (G_MONSTER_SPAWN_TIMER % SPIDER01_TIMER) != 0)
	{	
		return NULL;
	}

	slog("angel: x%f y%f", pos.x, pos.y);
	sprite_angel = sprite_load(SPRITE_ANGEL_FILEPATH, ANGEL_IMAGEW, ANGEL_IMAGEH, ANGEL_FRAMEW, ANGEL_FRAMEH);
	sprite_angel->fpl = ANGEL_FPL;//2 frames per line
	ent_angel = entity_load(sprite_angel, pos, 100, 25, STATE_PATROL);

	if(!ent_angel){
		slog("Could not spawn Spider01");
		return NULL;
	}
	//each frame is a single direction
	ent_angel->frame_horizontal = 0;
	ent_angel->frame_vertical = 0;
	ent_angel->velocity = vel;
	ent_angel->think = angel_think;
	ent_angel->nextThink = 1;
	ent_angel->thinkRate = SPIDER01_THINK_RATE;//think every 30 frames
	ent_angel->touch = angel_touch;
	ent_angel->update = angel_update;
	ent_angel->onDeath = angel_onDeath;
	ent_angel->state = STATE_PATROL;
	ent_angel->boundBox = boundBox;
	ent_angel->aggro_range = ANGEL_AGGRO_RANGE;//10 tiles
	ent_angel->damage = 10;

	return ent_angel;
}