#include "player.h"


char* player_char_file = "images/main.png";
entity* player = NULL;
const int PLAYER_HEIGHT = 64;
const int PLAYER_WIDTH  = 48;


void player_init(){
	Sprite2 *player_sprite = sprite_load(player_char_file,PLAYER_WIDTH, PLAYER_HEIGHT);
	Vec2d pos = {500,500};
	player = entity_load(player_sprite,pos, 100, 100, 0 );
}
void player_draw(){
	entity_draw(player,player->position.x,player->position.y);
}

void player_move(SDL_Event *e){
	if(!e){
		fprintf(stdout,"Player_Move sdl event e is null");
		return;
	}

	switch( e->key.keysym.sym )
    {
        case SDLK_UP:
			player->position.y -=5;
			player->frame_horizontal = (player->frame_horizontal + 1)%2;
			player->frame_vertical = 0;
			break;
        case SDLK_DOWN:
			player->position.y +=5;
			player->frame_horizontal = (player->frame_horizontal + 1)%2;
			player->frame_vertical = 2;
        break;

        case SDLK_LEFT:
			player->position.x -= 5;
			player->frame_horizontal = (player->frame_horizontal + 1)%2;
			player->frame_vertical = 3;
			break;

        case SDLK_RIGHT:
			player->position.x +=5;
			player->frame_horizontal = (player->frame_horizontal + 1)%2;
			player->frame_vertical = 1;
			break;
        default:
			break;
    }
}
void player_think(entity*self){

}