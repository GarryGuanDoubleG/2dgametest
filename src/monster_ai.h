#ifndef _MONSTER_H_
#define _MONSTER_H_

#include "sprite.h"
#include "entity.h"
#include "vector.h"
#include "simple_logger.h"
#include "monster_define.h"
#include "grue.h"
#include "spider.h"

//states the ai is in
#define STATE_IDLE		100 // walking around or standing still
#define STATE_PATROL		101 // walking around
#define STATE_AGGRO		102 //actively attacking player
#define STATE_SEEK		103 //trying to find player
//monster types
enum Monster{
	grue,
};
extern entity *player;

entity *monster_spawn(int type);
void monster_think(entity *self);
void monster_update(entity *self);

extern int MONSTER_SPAWN_TIMER;

#define sprite_grue_filepath "images/Grue/GrueGrin.png"
#define grue_imageW  56
#define grue_imageH  71
#define grue_frameW  101
#define grue_frameH  127


#endif