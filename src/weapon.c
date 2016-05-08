#include "weapon.h"
 
/*
* global weapon list used for retrieving stats and sprites for new instances of the weapon
*/
Weapon weaponlist[] = 
{
	//longsword
	{
		"images/player/slash/weapon longsword.png",//filepath
		"longsword",

		192,192,//image size
		//PLAYER_FRAMEW*3,PLAYER_FRAMEH*3,//Frame width/height
		//{PLAYER_FRAMEW*3,PLAYER_FRAMEH*3},//offset
		PLAYER_FRAMEW*3,PLAYER_FRAMEH*3,//Frame width/height
		{0,0},//offset

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
		6,// frames per line

		NULL,//image set during init
		false, // active
		false, // inuse
	},

};

/**
* @brief loads all the sprites in the weaponlist into memory to be retrieved immediately
*/
void weapon_load_all(){
	int i;

	for(i = 0; i < (sizeof weaponlist)/sizeof(Weapon); i++){
		weaponlist[i].image = sprite_load(weaponlist[i].filepath,weaponlist[i].imageW,weaponlist[i].imageH,weaponlist[i].frameW,weaponlist[i].frameH);
		weaponlist[i].image->fpl = weaponlist[i].fpl;
	}
	atexit(weapon_close_all);
}
/**
* @brief frees all the sprites images that was allocated into memory during initialization
*/
void weapon_close_all(){
	int i;

	for(i = 0; i < (sizeof weaponlist)/sizeof(Weapon); i++){
		SDL_DestroyTexture(weaponlist[i].image->image);
		weaponlist[i].image = NULL;
	}
}

/*
* @brief retrieves a pointer to a weapon matching its name
* @param name of weapon to search through weapon list
* @return pointer to weapon matching the weapon name param. 
*/
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

