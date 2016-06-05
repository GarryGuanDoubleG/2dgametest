#ifndef _MONSTER_H_
#define _MONSTER_H_
#include "sprite.h"
#include "Entity.h"
#include "vector.h"
#include "simple_logger.h"
#include "monster_define.h"
#include "grue.h"
#include "spider.h"
#include "minotaur.h"
#include "orc.h"
//states the ai is in

//monster types
/**
* @brief Entity pointer to the player used for finding paths
*/
extern Entity *player;
/**
* @brief calls on monster spawning function based on type to be spawned
*/
Entity *monster_spawn(int type);

/**
* @brief Countdown timer used for modulo division specific spawn rate
*/
extern int G_MONSTER_SPAWN_TIMER;



#endif