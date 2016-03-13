#ifndef _MONSTER_H_
#define _MONSTER_H_

#include "sprite.h"
#include "entity.h"
#include "vector.h"
#include "simple_logger.h"
#include "monster_define.h"
#include "grue.h"

//monster types
enum Monster{
	grue,
};
extern entity *player;

entity *monster_spawn(int type);
void monster_think(entity *self);
void monster_update(entity *self);

extern int MONSTER_SPAWN_TIMER;

#endif