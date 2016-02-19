#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "sprite.h"
#include "entity.h"
#include "weapon.h"

extern entity *player;
extern char * player_char_file;
extern const int PLAYERH;
extern const int PLAYERW;
extern const int PLAYER_FRAMEW;
extern const int PLAYER_FRAMEH;

typedef struct{ // player animation with equippables
	Sprite2 * body;
	Sprite2 * feet;
	Sprite2 * hands;
	Sprite2 * head;
	Sprite2 * legs;
	Sprite2 * torso; //  belts
	Sprite2 * chest;
	Sprite2 * shoulders;
	int fpl;
}player_anim;

void player_init();
void player_draw();
void player_move(SDL_Event *e);
void player_think(entity *self);
void player_update(entity *self);
void player_attack(SDL_Event *e);


#endif