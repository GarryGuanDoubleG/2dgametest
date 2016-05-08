#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "sprite.h"
#include "entity.h"
#include "weapon.h"
#include "armor.h"
#include "Tile.h"
#include "HUD.h"
#include "structure_define.h"
#include "structures.h"
#include "dict.h"

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

	int anim; // used for drawing sprite,walk,b
}Player_Equip;

void Player_Init();
void Player_Load_from_Def(Dict *value);

void Player_Draw();
void Player_Move(SDL_Event *e);
void Player_Think(entity *self);
void Player_Update(entity *self);
int Player_Tree_Collision();
#endif