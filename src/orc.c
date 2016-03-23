#include "orc.h"

void orc_update(entity *self)
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

void orc_touch(entity *self, entity *other)
{
}
void orc_think(entity *self)
{
	int randomNum;
	int distance;
	Vec2d self_pos =  {self->position.x, self->position.y};
	Vec2d player_pos = {entity_get_player()->position.x, entity_get_player()->position.y};
	Vec2d orc_dir;
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
		slog("orc state AGGRO");
		self->state = STATE_AGGRO;
	}
	else { self->state = STATE_PATROL; }

	if(self->state == STATE_AGGRO)
	{
		self->frame_vertical = 3;

		if(tile_to_tile_dist(tile_get_tile_number(self->position, self->boundBox), tile_get_tile_number(entity_get_player()->position, entity_get_player()->boundBox)) <= 1)
		{
			orc_dir.x = player_pos.x - self_pos.x;
			orc_dir.y = player_pos.y - self_pos.y;

			distance = Normalize2d(orc_dir);
			orc_dir = VectorScale(orc_dir, orc_dir, GRUE_VELOCITY_AGGRO);
			self->velocity = distance != 0 ? orc_dir : self->velocity;
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

void orc_onDeath(entity *self)
{

}

entity * orc_spawn(int type)
{
	slog("orc_spawn");
	if(type == Monster::orc)
	{
		return orc_spawn();
	}
}
entity * orc_spawn()
{
	Sprite2 * sprite_orc;
	entity * ent_orc = NULL;
	Tile start = tile_start();
	Vec2d pos = {start.mBox.x,start.mBox.y};
	Vec2d vel = {0,0};
	SDL_Rect boundBox = {ORC_FRAMEW*.1f,ORC_FRAMEH *.2f, ORC_FRAMEW*.8, ORC_FRAMEH*.8};

	if( (MONSTER_SPAWN_TIMER % SPIDER01_TIMER) != 0)
	{	
		return NULL;
	}

	slog("ORC: x%f y%f", pos.x, pos.y);
	sprite_orc = sprite_load(SPRITE_ORC_FILEPATH, ORC_IMAGEW, ORC_IMAGEH, ORC_FRAMEW, ORC_FRAMEH);
	sprite_orc->fpl = ORC_FPL;//2 frames per line
	ent_orc = entity_load(sprite_orc, pos, 100, 25, STATE_PATROL);

	if(!ent_orc){
		slog("Could not spawn Spider01");
		return NULL;
	}
	//each frame is a single direction
	ent_orc->frame_horizontal = 0;
	ent_orc->frame_vertical = 0;
	ent_orc->velocity = vel;
	ent_orc->think = orc_think;
	ent_orc->nextThink = 1;
	ent_orc->thinkRate = SPIDER01_THINK_RATE;//think every 30 frames
	//ent_orc->touch = orc_touch;
	ent_orc->update = orc_update;
	ent_orc->onDeath = orc_onDeath;
	ent_orc->state = STATE_PATROL;
	ent_orc->boundBox = boundBox;
	ent_orc->aggro_range = SPIDER01_AGGRO_RANGE;//10 tiles

	return ent_orc;
}