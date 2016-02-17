#include "player.h"


char* player_char_file = "images/main.png";
entity* player = NULL;
const int PLAYER_HEIGHT = 64;
const int PLAYER_WIDTH  = 48;
<<<<<<< HEAD

=======
//taken from lazyfoo
enum KeyPressSurfaces{
   face_up,
   face_right,
   face_down,
   face_left
};
//end
>>>>>>> f938880194512dc5de165eac4f3db7c7e5ae0628

void player_init(){
	Sprite2 *player_sprite = sprite_load(player_char_file,PLAYER_WIDTH, PLAYER_HEIGHT);
	player_sprite->fpl = 3;

	Vec2d pos = {500,500};
	player = entity_load(player_sprite,pos, 100, 100, 0 );
	player->frame_horizontal = 1;
	player->frame_vertical = 2;

	player->think = player_think;
}
void player_draw(){
	entity_draw(player,player->position.x,player->position.y);
}
<<<<<<< HEAD

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
=======
void player_think(entity* self){
>>>>>>> f938880194512dc5de165eac4f3db7c7e5ae0628

}
void player_move(SDL_Event e){
	player->frame_horizontal = (player->frame_horizontal + 1) % player->sprite->fpl;
	switch(e.key.keysym.sym)
	{
	case SDLK_UP:
		player->position.y += -5;
		player->frame_vertical = face_up;
		break;
	case SDLK_DOWN:
		player->position.y += 5;
		player->frame_vertical = face_down;
		break;
	case SDLK_RIGHT:
		player->position.x += 5;
		player->frame_vertical = face_right;
		break;
	case SDLK_LEFT:
		player->position.x -= 5;
		player->frame_vertical = face_left;
		break;
	default:
		break;
	}
	//this will need fixing
	(player->position.x < 0 || player->position.x > SCREEN_WIDTH) ? player->position.x = 0 : player->position.x;
	(player->position.y < 0 || player->position.y > SCREEN_HEIGHT) ? player->position.x = 0 : player->position.y;

}
