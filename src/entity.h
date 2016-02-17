#ifndef _ENTITY_H_
#define _ENTITY_H_
#include "vector.h"
#include "sprite.h"

/** @brief main entity structure for all interactable objects 
    / characters in the game
*   
*/
typedef struct Entity_S
{
	int inuse;
	Sprite2 * sprite;
	int frame_horizontal,
		frame_vertical;
	Vec2d position;
	Vec2d velocity;
	//audio library
	int health, maxhealth;
	int *inventory;
	int stamina;
	int state;
	void (*think)(struct Entity_S *self);
}entity;

extern const int MAX_ENTITY;
extern entity *entityList;
extern int entity_count;

void entity_free(entity * ent);
void entity_initialize_system();
entity *entity_load(Sprite2 *sprite,Vec2d pos, int health, int stamina, int state);
void entity_close();
void entity_draw(entity *ent, int x, int y);


#endif