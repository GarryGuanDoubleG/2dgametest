#ifndef _orc_H_
#define _orc_H_
#include "monster_define.h"
#include "Entity.h"
#include "sprite.h"
#include "vector.h"
#include "simple_logger.h"

/**
* @brief retrieves a pointer to an Entity whose sprite, default values, and function pointers
* are set to grue specific values
* @return NULL on error or pointer to Entity
*/ 
Entity * orc_spawn();
/**
* @brief Countdown timer used for modulo division specific spawn rate
*/
extern int G_MONSTER_SPAWN_TIMER;

#endif