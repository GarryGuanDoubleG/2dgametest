#ifndef _ENTITY_H_
#define _ENTITY_H_
#include "vector.h"
#include "sprite.h"
#include "weapon.h"
#include "pathfinding.h"
#include "Tile.h"
/** @brief main entity structure for all interactable objects 
    / characters in the game
*   
*/


typedef struct Entity_S
{
	int inuse;
	Sprite2 * sprite;
	int frame_horizontal;
	int frame_vertical;
	Vec2d position;
	Vec2d velocity;
	SDL_Rect boundBox; // used for collision
	//audio library
	int health, maxhealth;
	int inventory;
	int face_dir;
	int player;
	int aggro_range;//distance before state aggressive, in tiles
	Path path;

	Weapon *weapon;
	int stamina;
	int state;
	int nextThink;/*<Time index for next think */
	int thinkRate; /*<How often to run think */
	void (*think)(struct Entity_S *self);
	void (*update)(struct Entity_S *self);
	void (*touch)(struct Entity_S *self, struct Entity_S *other);
	void (*free)(struct Entity_S *self);
	void (*onDeath)(struct Entity_S *self);
}entity;

extern const int ENTITY_MAX;
extern entity *entityList;
extern int entity_count;

void entity_free(entity * ent);
void entity_initialize_system();
entity *entity_load(Sprite2 *sprite,Vec2d pos, int health, int stamina, int state);
void entity_close();
void entity_draw(entity *ent, int x, int y);
void entity_update();
void entity_update_all();
//in class
/*
 *@brief return a pointer to an empty entity structure
 *@return NULL on error or no more space for entities or a valid entity
 */
void entity_initialize_system(Uint32 maxEntities);
void entity_think_all();
void entity_update();
void entity_draw_all();
void entity_close_all();
int entity_collide(entity *a, entity*b);
void entity_check_collision_all();
void weapon_collision(entity *owner);

entity * entity_get_player();
#endif