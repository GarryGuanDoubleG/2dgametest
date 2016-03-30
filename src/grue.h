#ifndef _GRUE_H_
#define _GRUE_H_
#include "monster_define.h"
#include "entity.h"
#include "sprite.h"
#include "vector.h"
#include "simple_logger.h"
/**
* @brief retrieves a pointer to an entity whose sprite, default values, and function pointers
* are set to grue specific values
* @return NULL on error or pointer to entity
*/ 
entity * grue_spawn();
/**
* @brief Countdown timer used for modulo division specific spawn rate
*/
extern int G_MONSTER_SPAWN_TIMER;

#endif