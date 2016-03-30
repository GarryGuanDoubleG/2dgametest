#include "items.h"
#include "simple_logger.h"

item itemList[] = 
{
	{
		-1,
		"wood",

		ITEM_WOOD_ICON_PATH,
		ITEM_WOOD_ICON_IMG_W,
		ITEM_WOOD_ICON_IMG_H,
		ITEM_WOOD_ICON_FRAME_W,
		ITEM_WOOD_ICON_FRAME_H,

		ITEM_WOOD_MODEL_PATH,
		ITEM_WOOD_MODEL_IMG_W,
		ITEM_WOOD_MODEL_IMG_H,
		ITEM_WOOD_MODEL_FRAME_W,
		ITEM_WOOD_MODEL_FRAME_H,

		NULL,
		NULL,
		false,
		-1,
		false,
	},

};


void item_load_all(){
	int i;

	for(i = 0; i < (sizeof itemList)/sizeof(item); i++){
		itemList[i].icon = sprite_load(itemList[i].icon_path, itemList[i].icon_imageW,itemList[i].icon_imageH,itemList[i].icon_frameW,itemList[i].icon_frameH);
		itemList[i].item_model = sprite_load(itemList[i].model_path, itemList[i].model_imageW,itemList[i].model_imageH,itemList[i].model_frameW,itemList[i].model_frameH);
	}
	atexit(item_close_all);
}

void item_close_all(){
	int i;

	for(i = 0; i < (sizeof itemList)/sizeof(item); i++){
		SDL_DestroyTexture(itemList[i].icon->image);
		SDL_DestroyTexture(itemList[i].item_model->image);
		itemList[i].icon = NULL;
		itemList[i].item_model = NULL;
	}
}

item *getItem(char *item_name)
{
	int i;

	if(!item_name){
		slog("Weapon name given is NULL");
		return NULL;
	}

	for(i = 0; i < (sizeof itemList)/sizeof(item); i++){
		if(!strcmp(item_name, itemList[i].name)){
			return &itemList[i];
		}
	}
	
	slog("Could not find WEAPON: %s", item_name);
	return NULL;

}
