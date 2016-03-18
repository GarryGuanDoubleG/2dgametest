#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "sprite.h"
#include "entity.h"
#include "weapon.h"
#include "armor.h"
<<<<<<< HEAD

=======
#include "Tile.h"
>>>>>>> dd49caf5a836fff8af35af5dc64e0ef2a6369553
extern entity *player;
extern char * player_char_file;
extern const int PLAYERH;
extern const int PLAYERW;
extern const int PLAYER_FRAMEW;
extern const int PLAYER_FRAMEH;


typedef struct{ // player animation with equippables
	Armor * feet;
	Armor * hands;
	Armor * head;
	Armor * legs;
	Armor * torso; //  belts
	Armor * chest;
	Armor * shoulders;
	Weapon * weapon;
	int fpl;
	int anim; // used for drawing sprite,walk,b
}player_equip;

void player_init();
void player_draw();
void player_move(SDL_Event *e);
void player_think(entity *self);
void player_update(entity *self);
void player_attack(SDL_Event *e);

#endif