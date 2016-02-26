#include "armor.h"

const int ARMOR_MAX = 200;

Armor armorList[] = 
{
	{
		"images/player/head chain hood.png", // filepath
		 "head chain hood", // name
		 64,64,//image width & height
		 PLAYER_FRAMEW, PLAYER_FRAMEH, //frame
		{0,0},//offset
		3,
		3,

		-1,
		ARMOR_CHAIN,
		true,
		
		-1,
		-1, 
	},

};

void armor_init(char *name){
	int i;
	char path[128];// used to concatenate to filepath to find animation

	if(!name){
		slog("Armor name %s does not exist", name);
		return;
	}
	for(i = 0; i < sizeof(armorList)/sizeof(Armor); i++){
		if (!strcmp(armorList[i].name, name)){
			char load[128];
			armorList[i].image = sprite_load(armorList[i].filepath, armorList[i].imageH,armorList[i].imageW, 
				armorList[i].frameW, armorList[i].frameH);
			strcpy(load, armorList[i].filepath);
			strcat(load, " slash");

			armorList[i].image_slash = sprite_load(load, armorList[i].imageH,armorList[i].imageW, 
				armorList[i].frameW, armorList[i].frameH);
			
			strcpy(load, armorList[i].filepath);
			strcat(load, " bow");

			armorList[i].image_bow = sprite_load(load, armorList[i].imageH,armorList[i].imageW, 
				armorList[i].frameW, armorList[i].frameH);
		}
	}


}