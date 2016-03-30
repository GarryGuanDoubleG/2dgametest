#ifndef _ARMOR_H_
#define _ARMOR_H_

#include "sprite.h"
#include "vector.h"
#include "simple_logger.h"
/**brief define different types of armor for damage reduction scaling
* 
*/
#define ARMOR_CHAIN 0
#define ARMOR_CLOTH 1
#define ARMOR_LEATEHR 2
/**
* brief terms for the current animation state an entity is in to play matching armor animation
*/
#define WALK 10
#define SLASH 11
#define BOW  12
#define THRUST 13
#define SPELLCAST 14
/**
* brief Pixel Dimensions of the Player used for frame size of armor
*/
extern const int PLAYER_FRAMEW;
extern const int PLAYER_FRAMEH;
/*
* brief maximum number of armor instances that can be allocated into memory at once
*/
extern const int ARMOR_MAX;

/**
* brief stores armor values and sprite animations that are drawn on top of entities which reduce 
* damage taken by both type value and flat defensive values
*/
typedef struct{
	//name of filepaths for loading
	char * walk; /**<char pointer to relative filepath for walking animation*/
	char * slash;/**<pointer to the name of filepaths of sword & dagger slashing animation*/
	char * bow; /**<pointer to name of bow animation*/
	char * thrust; /**<pointer to name of filepath for thrusting (e.g. spear) animation */
	char * spellcast; /**<pointer to name of filepath for spellcasting animation*/
	//name
	char * name; /**<pointer to name of armor for reference*/

	int imageH,imageW;/**<dimensions of image in the sprite to render to screen */
	int frameW, frameH; /**<size of frame in pixels to draw to the screen */
	Vec2d offset;/**<x and y distance to draw away from the entity that wears this armor */
	//only need offset if using custom weapon not from same creator

	int phys_def;/**<physical defense value used for decreasing incoming physical damage by percent*/
	int magic_def;/**<magical defense value used for decreasing incoming magical damage by percent*/

	int face_dir; /**<direction entity is currently facing - up, down, left, right. Some ents only face left or right*/
	int type;/**<type of armor used for potentionally adding more or less damage reduction scaling to armor*/
	int equipped;/**<bool that tracks whether entity is currently wearing the armor or has it inventory*/

	int frame_horizontal; /**<Tracks which frame horizontally should be rendered from the sprite. Used for playing next frame of animation**/
	int frame_vertical; /**<Tracks which frame vertiaclly should be rendered from sprite. Used for determining which animation (or direction) should be played*/
	
	Sprite2 *image;/**<pointer to default sprite image usually set to walking animation*/
	Sprite2 *image_slash;/**<pointer to sprite with slashing animation that is played usually to swing a sword or dagger*/
	Sprite2 *image_bow; /**<pointer to sprite with bow and arrow shooting animation*/
	Sprite2 *image_thrust; /**<pointer to sprite with thrusting animation like spears*/
	Sprite2 *image_spellcast;/**<pointer to sprite with spellcasting animation*/

	int active; /**<1 if armor should apply effects and be drawn or 0 if should be ignored */
	int inuse; /**<true if armor is being tracked by resource manager and is in game. */
}Armor;

/**
* @brief loads all the sprites in the armor list into memory at initialization and stores it in the armor list
*/
void armor_load_all();
/**
* @brief retrieves armor from name and returns address of armor in armor list 
* @param requests name of armor used to loop through armorlist until match is found
* @return returns a reference to the matching armor in the armor list
*/
Armor *getArmor(char *name);
/**
* @brief used for getting animation sprite based on type and current armor
* @param takes animation type to be played e.g. slash, thrust, spellcast. Pointer to current armor to retrieve its respective animation
* @return pointer to sprite with animation
*/
Sprite2 *getArmorAnim(int anim,Armor * myArmor);


#endif