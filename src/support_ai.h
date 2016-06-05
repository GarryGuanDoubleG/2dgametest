#ifndef _SUPP_AI_H_
#define _SUPP_AI_H_
#include "sprite.h"
#include "Entity.h"
#include "vector.h"
#include "simple_logger.h"
#include "support_define.h"
#include "sara.h"
#include "supp_angel.h"
#include "archer.h"
extern Entity *player;
Entity *support_spawn(int type);
extern const int PLAYER_FRAMEW;
extern const int PLAYER_FRAMEH;




#endif