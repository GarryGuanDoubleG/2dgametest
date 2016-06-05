#ifndef _Entity_H_
#define _Entity_H_
#include "vector.h"
#include "sprite.h"
#include "weapon.h"
#include "monster_define.h"
#include "Tile.h"
#include "path_find.h"
#include "dict.h"
#include "particle_emitter.h"
#include "camera.h"
/**
* @brief terms for which team an Entity is on to determine who to attack
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

typedef enum 
{
	PLAYER			= 0,
	SPIDER			= 1,
	GRUE			= 2,
	MINO			= 3,

	//structures
	WALL			= 100,
	MAIN_BASE		= 101,
	TOWER			= 102,
	//tiles
	T_GRASS			= 1000,
	T_TREE			= 1001,
	T_PATH			= 1002,
	T_WATER			= 1003,
	T_BUSH			= 1004,
	T_ROCK			= 1005,
}Entity_TYPE;

typedef struct Entity_S
{
	int inuse; /**< used for memory allocation if not inuse*/
	int id;
	int face_dir;/**< direction Entity is facing(e.g. left) */
	int type;

	//animation & drawing
	Sprite * sprite;/**<pointer to sprite of entities body or default animation*/
	int frame;
	int next_frame;

	//Entity positions
	Vec2d position; /**<stores x and y positional values of Entity*/
	Vec2d velocity; /**<x and y values that increment the positional value every frame*/
	SDL_Rect boundBox; /**<x and y offsets from postion and accurate dimensions of an Entity used for collision detection**/
	
	//game mechanics
	int health, maxhealth; /**health values to update hud and determine if Entity is alive. health cannot be greater than max health*/
	int damage; /**<int damage to apply if colliding with enemy*/
	int stamina;/**<int stores resource thats consumed on certain actions*/
	
	//AI
	int state; /**<int stores state to use for AI actions*/
	int team; /**<int stores if Entity is on player's team or against player*/
	int aggro_range; /**< int number of tiles away from an Entity in an enemy team before an AI changes states and becomes aggro */

	//player
	int inventory;/**<true if Entity can hold items*/
	Weapon *weapon;/**<pointer to currently equipped weapon*/

	//structures
	int structure; /*< bool is a player building */
	int placed; /*< bool is in game? */
	int selected; /*< bool is player is placing building */
	//end

	Particle_Emitter * p_em; /**<particle emitter that manages particle effects */

	Path *path; /**<Path stores list of tiles to move to until target destination is reached*/

	int nextThink;/*<Time index for next think */
	int thinkRate; /*<How often to run think */

	void (*think)(struct Entity_S *self); /**<function pointer to think function to determine Entity's actions*/
	void (*update)(struct Entity_S *self); /**<function pointer to update function of specfic Entity*/
	void (*touch)(struct Entity_S *self, struct Entity_S *other);/**<function pointer that triggers when entites collide with one another*/
	void (*free)(struct Entity_S *self);/**<frees Entity from memory and allows new Entity to be allocated*/
	void (*onDeath)(struct Entity_S *self); /**<function pointer to event triggered on Entity's death*/
	void (*followPath)(struct Entity_S *self); /**<Function pointer to follow pathing to enemy*/
}Entity;

/**
* brief max number of entities to allocate into memory
*/
extern const int Entity_MAX;
/**
* brief list of all entities that are allocated into memory
*/
extern Entity *EntityList;
/**
* brief number of entities currently in use and exist in the game
*/
extern int Entity_count;
/**
* brief frees Entity from memory
*/
void Entity_free(Entity * ent);
/*
* @brief initializes memory and allocates it to store maximum number of entities
*/
void Entity_initialize_system();
/**
* @brief checks collision of Entity to structure
*/
int Entity_check_collision(Entity *self);
/*
* @brief loads player buildable structure
*/
Entity* struct_load(Sprite *sprite, int health, int defense, int type);
/**
* @brief loads Entity values into Entity list and returns pointer to address in list
*/
Entity *Entity_load(Sprite *sprite,Vec2d pos, int health, int stamina, int state);
/**
* @brief closes Entity system and frees all memory that was allocated
*/
void Entity_close(); 
/**
* @brief renders Entity onto screen
* @param Entity: enttiy to draw
*/
void Entity_draw(Entity *ent);
/**
* @brief calls on all update funtions entities in use every frame
*/
void Entity_update_all();
/*
* @brief moves to center of next tile in the Entity's path
*/
void ent_follow_path(Entity *self);
/**
* @brief locates the nearest Entity on the enemy tam
* @param takes pointer to Entity to use find nearest enemy relative to self's position
* @return returns pointer to enemy Entity
*/
Entity * ent_find_nearest_enemy(Entity *self);
/**
* @brief locates the nearest Entity on the friendly team
* @param takes pointer to Entity to use find nearest enemy relative to self's position
* @return returns pointer to friendly Entity
*/
Entity * ent_find_nearest_teammate(Entity *self);
//in class
/*
 *@brief return a pointer to an empty Entity structure
 *@return NULL on error or no more space for entities or a valid Entity
 */
void Entity_think_all();
/**
* @brief function that calls Entity a's touch function
* @param takes two Entity pointers and calls Entity a's touch function which applies effects to Entity b
* @return true if they colldie
**/
int Entity_collide(Entity *a, Entity*b);
void Entity_check_collision_all();
/**
* @brief checks if  weapon collided with an enemy Entity
* @param pointer to Entity that owns the weapon. Used for position and events
*/
void weapon_collision(Entity *owner);
/**
* @brief loops through Entity list to find player and returns it
* @return returns pointer to player Entity
*/
Entity * Entity_Get_Player();

/** 
* @brief returns the center position of an Entity
*/
#define ENT_CENTER_X(a) (a->position.x + a->sprite->frameW/2)
#define ENT_CENTER_Y(a) (a->position.y + a->sprite->frameH/2)

#endif