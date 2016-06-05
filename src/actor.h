#ifndef _ACTOR_H_
#define _ACTOR_H_

#include "vector.h"
#include "entity.h"

/**
* @brief Actor class used for loading basic information usually used in a level editor
*/
typedef struct Actor_S
{
	Uint8			active;
	Uint8			type;

	Sprite			*sprite;

	Vec2d			position;

	void (*draw)	(Sprite *sprite, int frame, Vec2d draw_position);
	void (*free)	(Actor_S *self);
	void (*update)  (Actor_S *self);
}Actor;

void actor_empty_list();

void actor_free(Actor **self);

void actor_close_system();

void actor_initialize_system();

Actor *actor_new(Vec2d position, Uint8 type);

void actor_draw_all();

void actor_update_all();

Sprite *actor_load_sprite(Uint8 type);

Uint8 actor_get_number_of_type(Uint8 type);

Vec2d actor_get_all_positions_of_type(Uint8 type);


#endif _ACTOR_H_

