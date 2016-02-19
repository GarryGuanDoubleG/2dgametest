#include "player.h"
#include "simple_logger.h"

char* player_char_file = "images/player/BODY_male.png";

entity* player = NULL;
Weapon weapon_curr;
const int PLAYERH = 64; /*<player image height*/
const int PLAYERW = 64; /*<player image width is 64x64.*/
const int PLAYER_FRAMEH = 128;
const int PLAYER_FRAMEW = 128;

static player_anim player_AnimSlash;
static player_anim player_AnimBow;
static player_anim player_AnimThrust;
static player_anim player_AnimSpellcast;
static player_anim player_AnimWalk;
static player_anim *player_current_anim;//keeps track of which animation we're using



//taken from lazyfoo
//order of up,left,down,right are listed in order to match frame vertical in player.png
enum KeyPressSurfaces{
   face_up,
   face_left,
   face_down,
   face_right
};
struct{ // player inventory
	Uint32 weapon;
	Uint32 head;
	Uint32 hands;
	Uint32 chest;
	Uint32 spell;
}player_struct;

//end
void player_init(){
	int i = 0;
	Vec2d pos = {500,500};
	Sprite2 *player_sprite = sprite_load(player_char_file,PLAYERW, PLAYERH, PLAYER_FRAMEW, PLAYER_FRAMEH);
	player_sprite->fpl = 9;

	player = entity_load(player_sprite,pos, 100, 100, 0 );
	player->frame_horizontal = 1;
	player->frame_vertical = 2;
	player->think = player_think;
	player->update = player_update;


	//initialize animations
	player_current_anim = &player_AnimWalk;
	//initialize slash
	player_anim anim = {player_sprite};
	anim.head = sprite_load("images/player/head chain hood.png", PLAYERH, PLAYERH, PLAYER_FRAMEW, PLAYER_FRAMEH);
	anim.chest = sprite_load("images/player/chest chain.png", PLAYERH, PLAYERH, PLAYER_FRAMEW, PLAYER_FRAMEH);
	anim.legs = sprite_load("images/player/legs chain.png",PLAYERH, PLAYERH, PLAYER_FRAMEW, PLAYER_FRAMEH);
	anim.body = player->sprite;
	anim.fpl = player->sprite->fpl;
	player_AnimWalk = anim;
	//slash
	anim.body = sprite_load("images/player/slash/body slash.png", PLAYERH, PLAYERH, PLAYER_FRAMEW, PLAYER_FRAMEH);
	anim.head = sprite_load("images/player/slash/head chain hood.png", PLAYERH, PLAYERH, PLAYER_FRAMEW, PLAYER_FRAMEH);
	anim.chest = sprite_load("images/player/slash/chest chain.png", PLAYERH, PLAYERH, PLAYER_FRAMEW, PLAYER_FRAMEH);
	anim.legs = sprite_load("images/player/slash/legs chain.png",PLAYERH, PLAYERH, PLAYER_FRAMEW, PLAYER_FRAMEH);
	anim.fpl = 5;
	player_AnimSlash = anim;
	//bow
	anim.body = sprite_load("images/player/bow/body bow.png", PLAYERH, PLAYERH, PLAYER_FRAMEW*3, PLAYER_FRAMEH);
	anim.fpl = 13;
	player_AnimBow = anim;

	/*weapon weap = {NULL};
	weap.image = sprite_load("images/player/slash/weapon longsword.png", 192, 192, PLAYER_FRAMEW*3,PLAYER_FRAMEH*3);
	weapon_curr = weap;*/
}
void player_draw(){
	//need to add other equipment
	entity_draw(player,player->position.x,player->position.y);
	sprite_draw(player_current_anim->chest,player->frame_horizontal, player->frame_vertical,__gt_graphics_renderer,player->position.x,player->position.y);
	sprite_draw(player_current_anim->legs,player->frame_horizontal, player->frame_vertical,__gt_graphics_renderer,player->position.x,player->position.y);
	sprite_draw(player_current_anim->head,player->frame_horizontal, player->frame_vertical,__gt_graphics_renderer,player->position.x,player->position.y);
	if(player_current_anim == &player_AnimSlash){
		//sprite_draw(weapon_curr.image,player->frame_horizontal, player->frame_vertical,__gt_graphics_renderer,player->position.x - weapon_curr.image->frameW/3,player->position.y - weapon_curr.image->frameH/3);
		sprite_draw(weapon_curr.image,player->frame_horizontal, player->frame_vertical,__gt_graphics_renderer,player->position.x - PLAYER_FRAMEW,player->position.y - PLAYER_FRAMEH);
		slog("Drawing Slash Weapon");
	}
}

void player_update(entity *self){
	if(player_current_anim == &player_AnimWalk){ //if walking, dont reset animation
		player->sprite = player_current_anim->body;
		return;
	}
	if(player->sprite != player_current_anim->body){
		player->sprite = player_current_anim->body;
	}
	//checks if animation played through at least once
	if(player->frame_horizontal >= player_current_anim->fpl)
	{
		player->frame_horizontal = 0;
		player_current_anim = &player_AnimWalk;
		player->sprite->fpl = player_AnimWalk.fpl;
	}
	else{
		player->frame_horizontal += 1;
	}

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
			player->frame_horizontal = (player->frame_horizontal + 1)%player->sprite->fpl;
			player->frame_vertical = face_down;
			break;
        case SDLK_LEFT:
			player->position.x -= 5;
			player->frame_horizontal = (player->frame_horizontal + 1)%player->sprite->fpl;
			player->frame_vertical = face_left;
			break;
        case SDLK_RIGHT:
			player->position.x +=5;
			player->frame_horizontal = (player->frame_horizontal + 1)%player->sprite->fpl;
			player->frame_vertical = face_right;
			break;
        default:
			player_attack(e);
			break;
    }
}

void player_attack(SDL_Event *e){
	if(!e){
		fprintf(stdout,"Player_Move sdl event e is null");
		return;
	}
	switch( e->key.keysym.sym )
    {
		case SDLK_SPACE:
			fprintf(stdout,"Hit Space");
			//if(player_struct.weapon == WEAP_SWORD)
			if(player_current_anim != &player_AnimSlash)
			{
				player_current_anim = &player_AnimSlash;
				player->sprite->fpl = player_AnimSlash.fpl;
				player->frame_horizontal = 0;//reset it;
			}
			break;
		default:
			break;
	}
			
}


void player_think(entity* self){


}
