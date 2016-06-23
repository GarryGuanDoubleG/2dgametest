#include "archer.h"

void archer_update(Entity *self)
{
	Vec2d new_pos = {self->position.x + self->velocity.x, self->position.y + self->velocity.y};

	if(self->state == STATE_AGGRO)
	{
		//self->path = getPath(self->aggro_range, &self->position, self->boundBox,Entity_Get_Player()->boundBox, &Entity_Get_Player()->position,self->path);
		self->followPath(self);
		Vec2dAdd(self->position, self->velocity, self->position);
	//	aStar_search(self->aggro_range, self->position, Entity_Get_Player()->position,self->path);
	}
	else if(!tile_collision(new_pos, self->boundBox))
	{
		self->position = new_pos;
	}
//	self->frame_horizontal = (self->frame_horizontal + 1) % self->sprite->fpl;
	Entity_Draw(self);
}

void archer_attack_mode(Entity *self, Entity * other)
{
	if(!self || !other)
	{
		slog("No archer or Enemy");
		return;
	}
	if(other->sprite)
	{
		slog("archer is attacking %s", other->sprite->filename);
		other->health -= self->damage;
	}
}

void archer_touch(Entity *self, Entity *other)
{
	if(!self || !other)
	{
		slog("No archer or Enemy");
		return;
	}
	//other->health -= self->damage;
}
void archer_think(Entity *self)
{
	int randomNum;
	int distance;
	Vec2d self_pos =  {self->position.x, self->position.y};	
	Entity *enemy; 
	Vec2d enemy_pos; 
	Vec2d archer_dir;

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
			slog("archer state AGGRO");
			self->state = STATE_AGGRO;
		}
		else { self->state = STATE_PATROL; }
	}

	if(self->state == STATE_AGGRO)
	{
		self->velocity.x = 0;
		self->velocity.y = 0;
		slog("Attack Mode");
		archer_attack_mode(self, enemy);
	//	self->path = getPath(self->aggro_range, &self->position, self->boundBox,Entity_Get_Player()->boundBox, &Entity_Get_Player()->position,self->path);
	//	aStar_search(self->aggro_range, self->position, Entity_Get_Player()->position,self->path);
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

void archer_onDeath(Entity *self)
{

}

Entity * archer_spawn()
{
	Sprite * sprite_archer;
	Entity * ent_archer = NULL;
	Tile start = tile_start();
	Vec2d pos = {start.mBox.x,start.mBox.y};
	Vec2d vel = {0,0};
	SDL_Rect boundBox = {MINO_FRAMEW*.1f,MINO_FRAMEH *.2f, MINO_FRAMEW*.8, MINO_FRAMEH*.8};

	if( (G_MONSTER_SPAWN_TIMER % SPIDER01_TIMER) != 0)
	{	
		return NULL;
	}

	slog("archer: x%f y%f", pos.x, pos.y);
	sprite_archer = Sprite_Load(SPRITE_ARCHER_FILEPATH, ARCHER_IMAGEW, ARCHER_IMAGEH, ARCHER_FRAMEW, ARCHER_FRAMEH);
	sprite_archer->fpl = ARCHER_FPL;//2 frames per line
	ent_archer = Entity_load(sprite_archer, pos, 100, 25, STATE_PATROL);

	if(!ent_archer){
		slog("Could not spawn Spider01");
		return NULL;
	}
	//each frame is a single direction
	ent_archer->frame = 0;

	ent_archer->velocity = vel;
	ent_archer->think = archer_think;
	ent_archer->nextThink = 1;
	ent_archer->thinkRate = SPIDER01_THINK_RATE;//think every 30 frames
	ent_archer->touch = archer_touch;
	ent_archer->update = archer_update;
	ent_archer->onDeath = archer_onDeath;
	ent_archer->state = STATE_PATROL;
	ent_archer->boundBox = boundBox;
	ent_archer->aggro_range = ARCHER_AGGRO_RANGE;//10 tiles
	ent_archer->damage = 20;

	return ent_archer;
}