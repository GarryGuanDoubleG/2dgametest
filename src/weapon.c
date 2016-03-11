#include "weapon.h"

Weapon weaponlist[] = 
{
	//longsword
	{
		"images/player/slash/weapon longsword.png",//filepath
		"longsword",

		192,192,//image size
		PLAYER_FRAMEW*3,PLAYER_FRAMEH*3,//Frame width/height
		{PLAYER_FRAMEW*3,PLAYER_FRAMEH*3},//offset
		{PLAYER_FRAMEW*.2,PLAYER_FRAMEH*.2},//offset
		{-PLAYER_FRAMEW/4,-PLAYER_FRAMEH/3,PLAYER_FRAMEW*1.5,PLAYER_FRAMEH*.65},//bounding up
		{-PLAYER_FRAMEW/4, PLAYER_FRAMEH/3,PLAYER_FRAMEW*1.5,PLAYER_FRAMEH*.65},//down
		{-PLAYER_FRAMEW, 0, PLAYER_FRAMEW*1.2, PLAYER_FRAMEH*.8},//bounding left
		{PLAYER_FRAMEW/2, 0 ,PLAYER_FRAMEW*1.2,PLAYER_FRAMEH*.8},//bounding right

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