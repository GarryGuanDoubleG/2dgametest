#ifndef _STRUCTURES_H_
#define _STRUCTURES_H_
#include "sprite.h"
#include "HUD.h"
#include "entity.h"
#include "Tile.h"
#include "vector.h"
#include "structure_define.h"
#include "camera.h"

typedef enum
{
	struct_wall = 0,
}structure_types;

typedef entity cStruct;

cStruct *structure_spawn(int type);

void structure_select(int type);
//void structure_update_all();
int structure_place();
void structure_HUD();
//void struct_update_all();
//structure * struct_load(Sprite *sprite, int health, int defense, int type);

#endif