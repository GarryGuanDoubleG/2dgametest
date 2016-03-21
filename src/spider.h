#ifndef _SPIDER_H_
#define _SPIDER_H_
#include "monster_define.h"
#include "entity.h"
#include "sprite.h"
#include "vector.h"
#include "simple_logger.h"

entity * spider_spawn(int type);
entity * spider01_spawn();

extern int MONSTER_SPAWN_TIMER;

#endif