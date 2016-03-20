#ifndef _ARMOR_H_
#define _ARMOR_H_

#include "sprite.h"
#include "vector.h"
#include "simple_logger.h"

#define ARMOR_CHAIN 0
#define ARMOR_CLOTH 1
#define ARMOR_LEATEHR 2

#define WALK 10
#define SLASH 11
#define BOW  12
#define THRUST 13
#define SPELLCAST 14

extern const int PLAYER_FRAMEW;
extern const int PLAYER_FRAMEH;
extern const int ARMOR_MAX;

typedef struct{
	//filepaths for loading
	char * walk;
	char * slash;
	char * bow;
	char * thrust;
	char * spellcast;
	//name
	char * name;

	int imageH,imageW;
	int frameW, frameH;
	Vec2d offset;//offset from wielder
	//only need offset if using custom weapon not from same creator

	int phys_def;//shoot projectile
	int magic_def;//base damage

	int face_dir; //direction we're facing to apply dmg
	int type;//chain,plate, cloth
	int equipped;

	int frame_horizontal;
	int frame_vertical;
	//sprites for all animation
	Sprite2 *image;//walking animation
	Sprite2 *image_slash;/*<image to render on top of player*/
	Sprite2 *image_bow;
	Sprite2 *image_thrust;
	Sprite2 *image_spellcast;

	int active;
	int inuse;
}Armor;

void armor_load_all();
Armor *getArmor(char *name);
Sprite2 *getArmorAnim(int anim,Armor * myArmor);


#endif