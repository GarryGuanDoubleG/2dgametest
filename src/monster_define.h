#ifndef _MONSTER_DEFINE_H_
#define _MONSTER_DEFINE_H_
//generic monster stats

//monster enum
enum Monster{
	grue,
	spider01,
	mino,
	orc,
};

//grue
#define sprite_grue_filepath "images/Monsters/Grue/GrueGrin.png"
#define grue_imageW  56
#define grue_imageH  71
#define grue_frameW  101
#define grue_frameH  127

#define GRUE_VELOCITY_AGGRO 8
#define GRUE_TIMER 100
#define GRUE_THINK_RATE 30

#define SPRITE_SPIDER01_FILEPATH "images/Monsters/Spider/spider01.png"
#define SPIDER01_IMAGEW 64
#define SPIDER01_IMAGEH 64
#define SPIDER01_FRAMEW 64
#define SPIDER01_FRAMEH 64
#define SPIDER01_VELOCITY_AGGRO 5
#define SPIDER01_AGGRO_RANGE 5
#define TYPE_SPIDER_01 1
#define SPIDER01_THINK_RATE 15
#define SPIDER01_TIMER 50//spawn rate

#define SPRITE_MINO_FILEPATH "images/Monsters/MINO.png"
#define MINO_IMAGEW 48
#define MINO_IMAGEH 48
#define MINO_FRAMEW 96
#define MINO_FRAMEH 96
#define MINO_FPL 10
#define MINO_VELOCITY_AGGRO 5
#define MINO_AGGRO_RANGE 5
#define TYPE_MINO 2
#define MINO_THINK_RATE 30
#define MINO_TIMER 100//spawn rate

#define SPRITE_ORC_FILEPATH "images/Monsters/orc.png"
#define ORC_IMAGEW 32
#define ORC_IMAGEH 32
#define ORC_FRAMEW 96
#define ORC_FRAMEH 96
#define ORC_FPL    10
#define ORC_VELOCITY_AGGRO 5
#define ORC_AGGRO_RANGE 5
#define TYPE_ORC 3
#define ORC_THINK_RATE 30
#define ORC_TIMER 300//spawn rate
#endif