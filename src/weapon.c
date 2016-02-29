#include "weapon.h"

Weapon weaponlist[] = 
{
	{
		"images/player/slash/weapon longsword.png",//filepath
		"longsword",
		192,192,//image size
		PLAYER_FRAMEW*3,PLAYER_FRAMEH*3,//size of player frame. need to retrieve at init
		{PLAYER_FRAMEW*3,PLAYER_FRAMEH*3},//offset
		{-PLAYER_FRAMEW/2,-PLAYER_FRAMEH/2,PLAYER_FRAMEW*3,PLAYER_FRAMEH},//bounding up
		{-PLAYER_FRAMEW/2, PLAYER_FRAMEH/2,PLAYER_FRAMEW*3,PLAYER_FRAMEH},//down
		{},//bounding left right
		{},

		false,//not ranged
		10, // 10 base damage

		-1,//direction is set dynamically
		WEAP_SWORD,
		false,//set to true dynamically

		-1,//frame_horizontal is set dynamically
		-1,//set through player
		5,//5 frames per line

		NULL,//image set during init
	},

};
//might not even need getOffset
/*
Vec2d getOffset(int type){
	Vec2d offset = {0,0};
	if(type == WEAP_SWORD){
		offset.x = -(PLAYER_FRAMEW);//negative to move into player
		offset.y = -(PLAYER_FRAMEH);
	}

	return offset;
}*/

void weapon_load_all(){
	int i;

	for(i = 0; i < (sizeof weaponlist)/sizeof(Weapon); i++){
		weaponlist[i].image = sprite_load(weaponlist[i].filepath,weaponlist[i].imageW,weaponlist[i].imageH,weaponlist[i].frameW,weaponlist[i].frameH);
	}
	atexit(weapon_close_all);
}

void weapon_close_all(){
	int i;

	for(i = 0; i < (sizeof weaponlist)/sizeof(Weapon); i++){
		SDL_DestroyTexture(weaponlist[i].image->image);
		weaponlist[i].image = NULL;
	}
}

Weapon *getWeapon(char *weap_name)
{
	int i;

	if(!weap_name){
		slog("Weapon name given is NULL");
		return NULL;
	}

	for(i = 0; i < (sizeof weaponlist)/sizeof(Weapon); i++){
		if(!strcmp(weap_name, weaponlist[i].name)){
			return &weaponlist[i];
		}
	}
	
	slog("Could not find WEAPON: %s", weap_name);
	return NULL;

}

