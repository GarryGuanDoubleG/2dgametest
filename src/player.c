#include "player.h"


char* player_char_file = "images/player/BODY_male.png";
entity* player = NULL;
const int PLAYER_HEIGHT = 64;
const int PLAYER_WIDTH  = 64; /*<player image height is 64x64.*/

//taken from lazyfoo
//order of up,left,down,right are listed in order to match frame vertical in player.png
enum KeyPressSurfaces{
   face_up,
   face_left,
   face_down,
   face_right
};
//end


void player_init(){
	Sprite2 *player_sprite = sprite_load(player_char_file,PLAYER_WIDTH, PLAYER_HEIGHT);
	player_sprite->fpl = 9;

	Vec2d pos = {500,500};
	player = entity_load(player_sprite,pos, 100, 100, 0 );
	player->frame_horizontal = 1;
	player->frame_vertical = 2;

	player->think = player_think;
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
			player->frame_horizontal = (player->frame_horizontal + 1)%player->sprite->fpl;
			player->frame_vertical = face_up;
			break;
        case SDLK_DOWN:
			player->position.y +=5;
			player->frame_horizontal = (player->frame_horizontal + 1)%2;
			player->frame_vertical = face_down;
        break;

        case SDLK_LEFT:
			player->position.x -= 5;
			player->frame_horizontal = (player->frame_horizontal + 1)%2;
			player->frame_vertical = face_left;
			break;

        case SDLK_RIGHT:
			player->position.x +=5;
			player->frame_horizontal = (player->frame_horizontal + 1)%2;
			player->frame_vertical = face_right;
			break;
        default:
			break;
    }
}

void player_think(entity* self){


}
