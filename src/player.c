#include "player.h"


char* player_char_file = "main character.png";
entity* player = NULL;
const int PLAYER_HEIGHT = 32;
const int PLAYER_WIDTH  = 32;


void player_init(){
	Sprite2 *player_sprite = sprite_load(player_char_file,PLAYER_WIDTH, PLAYER_HEIGHT);
	Vec2d pos = {500,500};
	player = entity_load(player_sprite,pos, 100, 100, 0 );
}
void player_draw(){
	entity_draw(player,player->position.x,player->position.y);
}
void player_think(){

}