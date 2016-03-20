#ifndef _ITEMS_H_
#define _ITEMS_H_
#include "sprite.h"
#include "items_define.h"

typedef struct item_S
{
	int type;
	char *name;
	//icon
	char *icon_path;
	int icon_imageW;
	int icon_imageH;
	int icon_frameW;
	int icon_frameH;
	//model
	char *model_path;
	int model_imageW;
	int model_imageH;
	int model_frameW;
	int model_frameH;
	//sprites
	Sprite2 *icon;
	Sprite2 *item_model;
	int pickup;
	int amount;
	int inuse;
}item;

extern item itemList[];

void item_load_all();
void item_close_all();
item *getItem(char *item_name);

#endif