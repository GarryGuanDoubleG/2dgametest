#ifndef _ENTITY_H_
#define _ENTITY_H_
#include "vector.h"
#include "sprite.h"
#include "weapon.h"
#include "monster_define.h"
#include "Tile.h"
#include "path_find.h"
/** @brief main entity structure for all interactable objects 
    / characters in the game
*   
*/

/*
enum Dir{
   UP,
   LEFT,
   DOWN,
   RIGHT,
};*/
#define TEAM_PLAYER 1
#define TEAM_ENEMY  0
#define STATE_IDLE		100 // walking around or standing still
#define STATE_PATROL		101 // walking around
#define STATE_AGGRO		102 //actively attacking player
#define STATE_SEEK		103 //trying to find player

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
	int aggro_range;
	int team;
	int damage;
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
	void (*followPath)(struct Entity_S *self);

	Path *path;
}entity;

extern const int ENTITY_MAX;
extern entity *entityList;
extern int entity_count;

void entity_free(entity * ent);
void entity_initialize_system();
int entity_structure_collision(Rect_f boundBox);
entity *entity_load(Sprite2 *sprite,Vec2d pos, int health, int stamina, int state);
void entity_close();
void entity_draw(entity *ent, int x, int y);
void entity_update();
void entity_update_all();
void ent_follow_path(entity *self);
entity * ent_find_nearest_enemy(entity *self);
entity * ent_find_nearest_teammate(entity *self);
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

#define ENT_CENTER_X(a) (a->position.x + a->sprite->frameW/2)
#define ENT_CENTER_Y(a) (a->position.y + a->sprite->frameH/2)

#endif