#ifndef _MONSTER_H_
#define _MONSTER_H_
#include "sprite.h"
#include "entity.h"
#include "vector.h"
#include "simple_logger.h"
#include "monster_define.h"
#include "grue.h"
#include "spider.h"
#include "minotaur.h"
#include "orc.h"
//states the ai is in

//monster types

extern entity *player;

entity *monster_spawn(int type);
void monster_think(entity *self);
void monster_update(entity *self);

extern int MONSTER_SPAWN_TIMER;



#endif