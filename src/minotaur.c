#include "minotaur.h"

void mino_update(entity *self)
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

void mino_touch(entity *self, entity *other)
{
}
void mino_think(entity *self)
{
	int randomNum;
	int distance;
	Vec2d self_pos =  {self->position.x, self->position.y};
	Vec2d player_pos = {entity_get_player()->position.x, entity_get_player()->position.y};
	Vec2d mino_dir;
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

	if(Vec2dDistanceSQ(self_pos,player_pos) < (TILE_WIDTH * TILE_HEIGHT) * self->aggro_range *self->aggro_range)
	{
		slog("Mino state AGGRO");
		self->state = STATE_AGGRO;
	}
	else { self->state = STATE_PATROL; }

	if(self->state == STATE_AGGRO)
	{
		self->frame_vertical = 3;

		if(tile_to_tile_dist(tile_get_tile_number(self->position, self->boundBox), tile_get_tile_number(entity_get_player()->position, entity_get_player()->boundBox)) <= 1)
		{
			mino_dir.x = player_pos.x - self_pos.x;
			mino_dir.y = player_pos.y - self_pos.y;

			distance = Normalize2d(mino_dir);
			mino_dir = VectorScale(mino_dir, mino_dir, GRUE_VELOCITY_AGGRO);
			self->velocity = distance != 0 ? mino_dir : self->velocity;
			path_free(self->path);
			self->path = NULL;
		}
		else
		{
			self->path = getPath(self->aggro_range, &self->position, self->boundBox,entity_get_player()->boundBox, &entity_get_player()->position,self->path);
		}
	//	aStar_search(self->aggro_range, self->position, entity_get_player()->position,self->path);
	}

	else if(self->state == STATE_PATROL){
		self->frame_vertical = 0;
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
}

void mino_onDeath(entity *self)
{

}

entity * mino_spawn(int type)
{
	slog("mino_spawn");
	if(type == Monster::mino)
	{
		return mino_spawn();
	}
}
entity * mino_spawn()
{
	Sprite * sprite_mino;
	entity * ent_mino = NULL;
	Tile start = tile_start();
	Vec2d pos = {start.mBox.x,start.mBox.y};
	Vec2d vel = {0,0};
	SDL_Rect boundBox = {MINO_FRAMEW*.1f,MINO_FRAMEH *.2f, MINO_FRAMEW*.8, MINO_FRAMEH*.8};

	if( (G_MONSTER_SPAWN_TIMER % SPIDER01_TIMER) != 0)
	{	
		return NULL;
	}

	slog("mino: x%f y%f", pos.x, pos.y);
	sprite_mino = sprite_load(SPRITE_MINO_FILEPATH, MINO_IMAGEW, MINO_IMAGEH, MINO_FRAMEW, MINO_FRAMEH);
	sprite_mino->fpl = MINO_FPL;//2 frames per line
	ent_mino = entity_load(sprite_mino, pos, 100, 25, STATE_PATROL);

	if(!ent_mino){
		slog("Could not spawn Spider01");
		return NULL;
	}
	//each frame is a single direction
	ent_mino->frame_horizontal = 0;
	ent_mino->frame_vertical = 0;
	ent_mino->velocity = vel;
	ent_mino->think = mino_think;
	ent_mino->nextThink = 1;
	ent_mino->thinkRate = SPIDER01_THINK_RATE;//think every 30 frames
	//ent_mino->touch = mino_touch;
	ent_mino->update = mino_update;
	ent_mino->onDeath = mino_onDeath;
	ent_mino->state = STATE_PATROL;
	ent_mino->boundBox = boundBox;
	ent_mino->aggro_range = SPIDER01_AGGRO_RANGE;//10 tiles

	return ent_mino;
}