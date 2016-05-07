#ifndef _WEAPON_H_
#define _WEAPON_H_
#include "sprite.h"
#include "vector.h"
#include "simple_logger.h"

#define WEAP_SWORD  1
#define WEAP_BOW	2
#define WEAP_SPEAR  3
#define WEAP_STAFF	4
#define WEAP_DAGGER 5
#define WEAP_RAPIER 6

/**@brief All inclusive weapon container
*	
*/

extern const int PLAYER_FRAMEW;
extern const int PLAYER_FRAMEH;

typedef struct Weapon_S{

	char * filepath;
	char * name;

	int imageH,imageW;
	int frameW, frameH;

	//only need offset if using custom weapon not from same creator
	Vec2d offset;//used only for centering image onto player
	Vec2d boundOffset; // used for collision

	Rect_f boundUp;
	Rect_f boundDown;
	Rect_f boundLeft;
	Rect_f boundRight;

	int ranged;//shoot projectile
	int damage;//base damage

	int face_dir; //direction we're facing to apply dmg
	int type;//sword, bow, etc. for animation
	int equipped;

	int frame_horizontal;
	int frame_vertical;
	int fpl;// applies to player and armor

	Sprite *image;/*<image to render on top of player*/
	int active;
	int inuse;
}Weapon;



extern Weapon weaponlist[];
Vec2d getOffset(int type);

void weapon_load_all();
void weapon_close_all();

Weapon * getWeapon(char *weap_name);


#endif