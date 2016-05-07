#ifndef _ARCHER_H_
#define _ARCHER_H_
#include "support_define.h"
#include "entity.h"
#include "sprite.h"
#include "vector.h"
#include "simple_logger.h"
/**
* @brief retrieves a pointer to an entity whose sprite, default values, and function pointers
* are set to grue specific values
* @return NULL on error or pointer to entity
*/ 
entity * archer_spawn();
/**
* @brief Countdown timer used for modulo division with archer specific spawn rate
*/
extern int G_MONSTER_SPAWN_TIMER;

#endif