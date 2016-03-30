#ifndef _ITEMS_H_
#define _ITEMS_H_
#include "sprite.h"
#include "items_define.h"
/**
* @brief structure for items that can be carried by entities or used for different purposes
*/
typedef struct item_S
{
	int type; /**<int type of the item*/
	char *name; /**<cstrong of name of the item */
	//icon
	char *icon_path; /**<cstring for filepath of item icon */
	int icon_imageW; /**<image width of icon of item */
	int icon_imageH; /**<image height of icon of item*/
	int icon_frameW; /**<image frame width of icon*/
	int icon_frameH; /**<image frame height of icon */
	//model
	char *model_path; /**<cstring filepath of ingame appearane of item*/
	int model_imageW; /**<image width of model */
	int model_imageH; /**<image height of model */
	int model_frameW; /**<image frame width of model */
	int model_frameH; /**<image frame height of model */
	//sprites
	Sprite2 *icon; /**<pointer to sprite of icon image loaded into memory*/
	Sprite2 *item_model; /**<pointer to sprite of model image loaded into memory*/
	int pickup; /** <int true if item can be picked up*/
	int amount; /** <int amount added to inventory when picked up*/
	int inuse;/**<int true if loaded into memory*/
}item;
/**
* @brief external list of items that can be loaded into the game
*/
extern item itemList[];
/**
* @brief loads all the sprites in the itemList into memory and updates itemList with pointers to loaded sprites
*/
void item_load_all(); 
/**
* @brief frees all the allocated memory from the item list and all items stored in inventory
*/
void item_close_all();
/**
* @brief gets matching item from itemlist by matching its name to parameter
* @param name of the item to return
* @return returns a new item 
*/
item *getItem(char *item_name);

#endif