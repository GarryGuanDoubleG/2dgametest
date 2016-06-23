#include "spider.h"



Entity * spider_spawn(int type)
{
	slog("spider_spawn");
	if(type == Monster::spider01)
	{
		return spider01_spawn();
	}
}
Entity * spider01_spawn()
{
	Sprite * sprite_spider01;
	Entity * ent_spider01 = NULL;
	Tile start = tile_start();
	Vec2d pos = {start.mBox.x,start.mBox.y};
	Vec2d vel = {0,0};
	SDL_Rect boundBox = {SPIDER01_FRAMEW*.1f,SPIDER01_FRAMEH *.2f, SPIDER01_FRAMEW, SPIDER01_FRAMEH};

	if( (G_MONSTER_SPAWN_TIMER % SPIDER01_TIMER) != 0)
	{	
		return NULL;
	}

	slog("spider: x%f y%f", pos.x, pos.y);
	sprite_spider01 = Sprite_Load(SPRITE_SPIDER01_FILEPATH, SPIDER01_IMAGEW, SPIDER01_IMAGEH, SPIDER01_FRAMEW, SPIDER01_FRAMEH);
	sprite_spider01->fpl = 10;
	ent_spider01 = Entity_load(sprite_spider01, pos, 100, 25, STATE_PATROL);

	if(!ent_spider01){
		slog("Could not spawn Spider01");
		return NULL;
	}
	//each frame is a single direction
	ent_spider01->frame= 0;
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

	ent_spider01->type = Entity_TYPE::SPIDER;
	return ent_spider01;
}