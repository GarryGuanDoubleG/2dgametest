#ifndef _WEAPON_H_
#define _WEAPON_H_
#include "sprite.h"
#include "vector.h"

/**@brief All inclusive weapon container
*	
*/

typedef struct{
	Vec2d offset;//offset from wielder
	//only need offset if using custom weapon not from same creator

	int ranged;//shoot projectile
	int damage;//base damage

	int face_dir; //direction we're facing to apply dmg
	int type;//sword, bow, etc. for animation
	int equipped;

	int sizeW; /*<size of weapon for collision*/ 
	int sizeH; /*<size height of weapon*/

	int frame_horizontal;
	int frame_vertical;
	int fpl;// applies to player and armor
	Sprite2 image;/*<image to render on top of player*/
}weapon;
 


#endif