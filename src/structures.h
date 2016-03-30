#ifndef _STRUCTURES_H_
#define _STRUCTURES_H_
#include "sprite.h"
#include "HUD.h"
#include "entity.h"
#include "Tile.h"
#include "vector.h"
#include "structure_define.h"

#define STRUCT_MAX 1000

typedef struct Structure_S
{
	int health;
	int defense;
	int inuse;
	int selected;
	int placed;
	Sprite2 * sprite;
	Vec2d position;
	Rect_f boundBox;
	void (*update)(Structure_S *self);
	void (*touch)(Structure_S *self, entity *other);

}structure;

extern structure * struct_list;

void structure_init_system();
void structure_close_system();
void structure_new();
void structure_free(structure * self_struct);
void structure_select(int type);
void structure_update_all();
int structure_place();
void structure_HUD();
void struct_update_all();
structure * struct_load(Sprite2 *sprite, int health, int defense, int type);
int struct_collision(structure *self, entity *other);


#endif