#ifndef _ENTITY_H_
#define _ENTITY_H_
#include "vector.h"
#include "sprite.h"
#include "weapon.h"
#include "monster_define.h"
#include "Tile.h"
#include "path_find.h"
/**
* @brief terms for which team an entity is on to determine who to attack
*/
#define TEAM_PLAYER 1
#define TEAM_ENEMY  0
/**
* @brief states for roaming AI to determine what actions to take
*/
#define STATE_IDLE		100 // walking around or standing still
#define STATE_PATROL		101 // walking around
#define STATE_AGGRO		102 //actively attacking player
#define STATE_SEEK		103 //trying to find player

/**
* @brief structure to store intial values and functions of anything that needs to think in the game
* contains values for AI, player interaction, sprite for drawing, and values for interacting with environment
*/
typedef struct Entity_S
{
	int inuse; /**< used for memory allocation if not inuse*/
	//animation & drawing
	Sprite2 * sprite;/**<pointer to sprite of entities body or default animation*/
	int frame_horizontal;/**<Tracks which frame horizontally should be rendered from the sprite. Used for playing next frame of animation**/
	int frame_vertical; /**<Tracks which frame vertiaclly should be rendered from sprite. Used for determining which animation (or direction) should be played*/
	//ent positions
	Vec2d position; /**<stores x and y positional values of entity*/
	Vec2d velocity; /**<x and y values that increment the positional value every frame*/
	SDL_Rect boundBox; /**<x and y offsets from postion and accurate dimensions of an entity used for collision detection**/
	//game mechanic
	int health, maxhealth; /**health values to update hud and determine if entity is alive. health cannot be greater than max health*/
	int inventory;/**<true if entity can hold items*/
	int face_dir; /**<direction entity is currently facing - up, down, left, right. Some ents only face left or right*/
	int player; /**<int stores if this entity is the player*/
	int aggro_range; /**< int number of tiles away from an entity in an enemy team before an AI changes states and becomes aggro */
	int team; /**<int stores if entity is on player's team or against player*/
	int damage; /**<int damage to apply if colliding with enemy*/
	Weapon *weapon;/**<pointer to currently equipped weapon*/
	int stamina;/**<int stores resource thats consumed on certain actions*/
	int state; /**<int stores state to use for AI actions*/
	int nextThink;/*<Time index for next think */
	int thinkRate; /*<How often to run think */
	//structures
	int structure; /*< bool is a player building */
	int placed; /*< bool is in game? */
	int selected; /*< bool is player is placing building */
	int struct_type;/*< int type of structure */
	//end
	void (*think)(struct Entity_S *self); /**<function pointer to think function to determine entity's actions*/
	void (*update)(struct Entity_S *self); /**<function pointer to update function of specfic entity*/
	void (*touch)(struct Entity_S *self, struct Entity_S *other);/**<function pointer that triggers when entites collide with one another*/
	void (*free)(struct Entity_S *self);/**<frees entity from memory and allows new entity to be allocated*/
	void (*onDeath)(struct Entity_S *self); /**<function pointer to event triggered on entity's death*/
	void (*followPath)(struct Entity_S *self); /**<Function pointer to follow pathing to enemy*/

	Path *path; /**<Path stores list of tiles to move to until target destination is reached*/
}entity;

/**
* brief max number of entities to allocate into memory
*/
extern const int ENTITY_MAX;
/**
* brief list of all entities that are allocated into memory
*/
extern entity *entityList;
/**
* brief number of entities currently in use and exist in the game
*/
extern int entity_count;
/**
* brief frees entity from memory
*/
void entity_free(entity * ent);
/*
* @brief initializes memory and allocates it to store maximum number of entities
*/
void entity_initialize_system();
/**
* @brief checks collision of entity to structure
*/
int entity_check_collision(entity *self);
/*
* @brief loads player buildable structure
*/
entity* struct_load(Sprite2 *sprite, int health, int defense, int type);
/**
* @brief loads entity values into entity list and returns pointer to address in list
*/
entity *entity_load(Sprite2 *sprite,Vec2d pos, int health, int stamina, int state);
/**
* @brief closes entity system and frees all memory that was allocated
*/
void entity_close();
/**
* @brief renders entity onto screen
* @param requires entity to draw and location to draw to
*/
void entity_draw(entity *ent, int x, int y);
/**
* @brief calls on all update funtions entities in use every frame
*/
void entity_update_all();
/*
* @brief moves to center of next tile in the entity's path
*/
void ent_follow_path(entity *self);
/**
* @brief locates the nearest entity on the enemy tam
* @param takes pointer to entity to use find nearest enemy relative to self's position
* @return returns pointer to enemy entity
*/
entity * ent_find_nearest_enemy(entity *self);
/**
* @brief locates the nearest entity on the friendly team
* @param takes pointer to entity to use find nearest enemy relative to self's position
* @return returns pointer to friendly entity
*/
entity * ent_find_nearest_teammate(entity *self);
//in class
/*
 *@brief return a pointer to an empty entity structure
 *@return NULL on error or no more space for entities or a valid entity
 */
void entity_think_all();
/**
* @brief function that calls entity a's touch function
* @param takes two entity pointers and calls entity a's touch function which applies effects to entity b
* @return true if they colldie
**/
int entity_collide(entity *a, entity*b);
void entity_check_collision_all();
/**
* @brief checks if  weapon collided with an enemy entity
* @param pointer to entity that owns the weapon. Used for position and events
*/
void weapon_collision(entity *owner);
/**
* @brief loops through entity list to find player and returns it
* @return returns pointer to player entity
*/
entity * entity_get_player();

/** 
* @brief returns the center position of an entity
*/
#define ENT_CENTER_X(a) (a->position.x + a->sprite->frameW/2)
#define ENT_CENTER_Y(a) (a->position.y + a->sprite->frameH/2)

#endif