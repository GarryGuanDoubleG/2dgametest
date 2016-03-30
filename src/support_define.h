#ifndef _SUPPORT_DEFINE_H_
#define _SUPPORT_DEFINE_H_
//generic monster stats

//monster enum
enum Support{
	sara,
	archer,
	healer,//3
};
//states the ai is in
//grue
#define SPRITE_SARA_FILEPATH "images/support/Sara2Balls2Direct.png"
#define SARA_IMAGEW 27
#define SARA_IMAGEH 50
#define SARA_FRAMEW PLAYER_FRAMEW * 2/3
#define SARA_FRAMEH PLAYER_FRAMEH * 3/4
#define SARA_FPL 16
#define SARA_TIMER 300
#define SARA_AGGRO_RANGE 4
#define SARA_VELOCITY_AGGRO 4
#define SARA_THINK_RATE 30

#define SPRITE_ANGEL_FILEPATH "images/support/angel.png"
#define ANGEL_IMAGEW 80
#define ANGEL_IMAGEH 64
#define ANGEL_FRAMEW PLAYER_FRAMEW
#define ANGEL_FRAMEH PLAYER_FRAMEH
#define ANGEL_FPL 4
#define ANGEL_TIMER 300
#define ANGEL_AGGRO_RANGE 2
#define ANGEL_THINK_RATE 30

#define SPRITE_ARCHER_FILEPATH "images/support/archer.png"
#define ARCHER_IMAGEW 32
#define ARCHER_IMAGEH 32
#define ARCHER_FRAMEW 64
#define ARCHER_FRAMEH 96
#define ARCHER_FPL 10
#define ARCHER_TIMER 300
#define ARCHER_AGGRO_RANGE 4
#define ARCHER_THINK_RATE 30


#endif