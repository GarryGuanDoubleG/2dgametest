#include "spider.h"

void spider_update(entity *self)
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

	entity_draw(self,self->position.x, self->position.y);
}

void spider_touch(entity *self, entity *other)
{
}
void spider_think(entity *self)
{
	int randomNum;
	int distance;
	Vec2d self_pos =  {self->position.x, self->position.y};
	Vec2d player_pos = {entity_get_player()->position.x, entity_get_player()->position.y};
	Vec2d spider_dir;
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
		slog("Spider state AGGRO");
		self->state = STATE_AGGRO;
	}
	else { self->state = STATE_PATROL; }

	if(self->state == STATE_AGGRO)
	{
		if(tile_to_tile_dist(tile_get_tile_number(self->position, self->boundBox), tile_get_tile_number(entity_get_player()->position, entity_get_player()->boundBox)) <= 1)
		{
			spider_dir.x = player_pos.x - self_pos.x;
			spider_dir.y = player_pos.y - self_pos.y;

			distance = Normalize2d(spider_dir);
			spider_dir = VectorScale(spider_dir, spider_dir, GRUE_VELOCITY_AGGRO);
			self->velocity = distance != 0 ? spider_dir : self->velocity;
			path_free(self->path);
			self->path = NULL;
		}
		else
		{
			self->path = getPath(self->aggro_range, &self->position, self->boundBox,entity_get_player()->boundBox, &entity_get_player()->position,self->path);
		}
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
			self->frame_horizontal = (self->frame_horizontal + 1) % self->sprite->fpl;
			self->frame_vertical = 3;//hard coded based on sprite
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
			self->frame_vertical = 0;
		}
		else
		{
			self->frame_horizontal = (self->frame_horizontal + 1) % self->sprite->fpl;
			self->frame_vertical = 2;
		}		
	}
}

void spider_onDeath(entity *self)
{

}

entity * spider_spawn(int type)
{
	slog("spider_spawn");
	if(type == Monster::spider01)
	{
		return spider01_spawn();
	}
}
entity * spider01_spawn()
{
	Sprite2 * sprite_spider01;
	entity * ent_spider01 = NULL;
	Tile start = tile_start();
	Vec2d pos = {start.mBox.x,start.mBox.y};
	Vec2d vel = {0,0};
	SDL_Rect boundBox = {SPIDER01_FRAMEW*.1f,SPIDER01_FRAMEH *.2f, SPIDER01_FRAMEW, SPIDER01_FRAMEH};

	if( (G_MONSTER_SPAWN_TIMER % SPIDER01_TIMER) != 0)
	{	
		return NULL;
	}

	slog("spider: x%f y%f", pos.x, pos.y);
	sprite_spider01 = sprite_load(SPRITE_SPIDER01_FILEPATH, SPIDER01_IMAGEW, SPIDER01_IMAGEH, SPIDER01_FRAMEW, SPIDER01_FRAMEH);
	sprite_spider01->fpl = 10;//2 frames per line
	ent_spider01 = entity_load(sprite_spider01, pos, 100, 25, STATE_PATROL);

	if(!ent_spider01){
		slog("Could not spawn Spider01");
		return NULL;
	}
	//each frame is a single direction
	ent_spider01->frame_horizontal = 0;
	ent_spider01->frame_vertical = 0;
	ent_spider01->velocity = vel;
	ent_spider01->think = spider_think;
	ent_spider01->nextThink = 1;
	ent_spider01->thinkRate = SPIDER01_THINK_RATE;//think every 30 frames
//	ent_spider01->touch = spider_touch;
	ent_spider01->update = spider_update;
	ent_spider01->onDeath = spider_onDeath;
	ent_spider01->state = STATE_PATROL;
	ent_spider01->boundBox = boundBox;
	ent_spider01->aggro_range = SPIDER01_AGGRO_RANGE;//10 tiles

	return ent_spider01;
}