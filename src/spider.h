#ifndef _SPIDER_H_
#define _SPIDER_H_
#include "monster_define.h"
#include "Entity.h"
#include "sprite.h"
#include "vector.h"
#include "simple_logger.h"

Entity * spider_spawn(int type);
Entity * spider01_spawn();

extern int G_MONSTER_SPAWN_TIMER;

#endif