#ifndef _ITEMS_H_
#define _ITEMS_H_
#include "sprite.h"
#include "items_define.h"

typedef struct item_S
{
	int type;
	char *name;
	char *icon_path;
	char *model_path;
	Sprite2 *icon;
	Sprite2 *item_model;
	int pickup;
	int amount;
	int inuse;
}item;

extern item itemList[];

#endif