#include "armor.h"

const int ARMOR_MAX = 200;

Armor armorList[] = 
{
	{
		"images/player/head chain hood.png", // filepath
		"images/player/slash/head chain hood.png",
		"images/player/bow/head chain hood.png",
		"images/player/thrust/head chain hood.png",
		"images/player/spell/head chain hood.png",
		 "head chain hood", // name
		 64,64,//image width & height
		 PLAYER_FRAMEW, PLAYER_FRAMEH, //frame
		{0,0},//offset
		3,
		3,

		-1,
		ARMOR_CHAIN,
		true,
		
		-1,// frame horizontal
		-1, //fame vertical
	},

	{
		"images/player/chest chain.png", // filepath
		"images/player/slash/chest chain.png",
		"images/player/bow/chain hood.png",
		"images/player/thrust/head chain hood.png",
		"images/player/spell/head chain hood.png",
		 "chest chain", // name
		 64,64,//image width & height
		 PLAYER_FRAMEW, PLAYER_FRAMEH, //frame
		{0,0},//offset
		3,
		3,

		-1,
		ARMOR_CHAIN,
		true,
		
		-1,// frame horizontal
		-1, //fame vertical
	},

};

void armor_load_all(){
	int i;
	char path[128];// used to concatenate to filepath to find animation

	for(i = 0; i < sizeof(armorList)/sizeof(Armor); i++){
		armorList[i].image = sprite_load(armorList[i].walk, armorList[i].imageH,armorList[i].imageW, 
										 armorList[i].frameW, armorList[i].frameH);
		armorList[i].image_slash = sprite_load(armorList[i].slash, armorList[i].imageH,armorList[i].imageW, 
										 armorList[i].frameW, armorList[i].frameH);
		armorList[i].image_slash->fpl = 5;
		armorList[i].image_bow = sprite_load(armorList[i].bow, armorList[i].imageH,armorList[i].imageW, 
										 armorList[i].frameW, armorList[i].frameH);
		armorList[i].image_thrust = sprite_load(armorList[i].thrust, armorList[i].imageH,armorList[i].imageW, 
										 armorList[i].frameW, armorList[i].frameH);
		armorList[i].image_spellcast = sprite_load(armorList[i].spellcast, armorList[i].imageH,armorList[i].imageW, 
										 armorList[i].frameW, armorList[i].frameH);
	}

}

Armor *getArmor(char *name){
	int i;

	for(i = 0; i < sizeof(armorList)/sizeof(Armor); i++){
		if(!strcmp(armorList[i].name, name)){
			return &armorList[i];
		}
	}

	slog("Could not find armor: %s", name);
	return NULL;
}

Sprite *getArmorAnim(int anim, Armor * myArmor){

	if(!myArmor){
		slog("Cannot get Armor Sprite. myArmor is NULL");
		return NULL;
	}
	switch(anim){
	case WALK:
		return myArmor->image;
		break;
	case SLASH:
		return myArmor->image_slash;
		break;
	case BOW:
		return myArmor->image_bow;
		break;
	case THRUST:
		return myArmor->image_thrust;
		break;
	case SPELLCAST:
		return myArmor->image_spellcast;
		break;
	default:
		slog("Could not find animation");
		return NULL;
		break;
	}
}