#ifndef _MONSTER_DEFINE_H_
#define _MONSTER_DEFINE_H_
//generic monster stats

//states the ai is in
#define STATE_IDLE		100 // walking around or standing still
#define STATE_PATROL		101 // walking around
#define STATE_AGGRO		102 //actively attacking player
#define STATE_SEEK		103 //trying to find player

//grue
#define sprite_grue_filepath "images/Grue/GrueGrin.png"
#define grue_imageW  56
#define grue_imageH  71
#define grue_frameW  101
#define grue_frameH  127
#define GRUE_VELOCITY_AGGRO 8
#define GRUE_TIMER 300
#define GRUE_THINK_RATE 30

#endif