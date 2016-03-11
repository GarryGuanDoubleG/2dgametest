#include "player.h"
#include "simple_logger.h"

char* player_char_file = "images/player/BODY_male.png";

entity* player = NULL;

const int PLAYERH = 64; /*<player image height*/
const int PLAYERW = 64; /*<player image width is 64x64.*/
const int PLAYER_FRAMEH = 128;
const int PLAYER_FRAMEW = 128;

static player_equip PlayerEquip;
static int animCurrent;//used to determine current animation
//taken from lazyfoo
//order of up,left,down,right are listed in order to match frame vertical in player.png
enum KeyPressSurfaces{
   face_up,
   face_left,
   face_down,
   face_right
};

struct {
	Sprite2 *image;
	Sprite2 *image_slash;
	Sprite2 *image_bow;
	Sprite2 *image_thrust;
	Sprite2 *image_spell;
}playerBody;


//end
void player_init(){
	int i = 0;
	Vec2d pos = {SCREEN_WIDTH/2,SCREEN_HEIGHT/2};
	SDL_Rect bound = {PLAYER_FRAMEW*.2f,PLAYER_FRAMEW*.2f,PLAYER_FRAMEW*.7f, PLAYER_FRAMEH*.7f};

	Sprite2 *player_sprite = sprite_load(player_char_file,PLAYERW, PLAYERH, PLAYER_FRAMEW, PLAYER_FRAMEH);
	player_sprite->fpl = 9;
	animCurrent = WALK;
	player = entity_load(player_sprite,pos, 100, 100, 0 );
	slog("Player: X: %i Y: %i", player->position.x, player->position.y);

	playerBody.image = player->sprite;
	playerBody.image_slash = sprite_load("images/player/slash/body slash.png",PLAYERW, PLAYERH, PLAYER_FRAMEW, PLAYER_FRAMEH);
	playerBody.image_slash->fpl = 5;
	player->boundBox = bound;

	player->frame_horizontal = 1;
	player->frame_vertical = 2;
	player->think = player_think;
	player->update = player_update;

	weapon_load_all();
	armor_load_all();

	PlayerEquip.weapon = getWeapon("longsword");
	player->weapon = PlayerEquip.weapon;
	PlayerEquip.head = getArmor("head chain hood");
}
void player_draw_equip(){
	if(PlayerEquip.feet){
		sprite_draw(getArmorAnim(animCurrent, PlayerEquip.feet), player->frame_horizontal, player->frame_vertical,__gt_graphics_renderer,player->position.x,player->position.y);
	}
	if(PlayerEquip.hands){
		sprite_draw(getArmorAnim(animCurrent, PlayerEquip.hands), player->frame_horizontal, player->frame_vertical,__gt_graphics_renderer,player->position.x,player->position.y);
	}
	if(PlayerEquip.head){
		sprite_draw(getArmorAnim(animCurrent, PlayerEquip.head), player->frame_horizontal, player->frame_vertical,__gt_graphics_renderer,player->position.x,player->position.y);
	}
	if(PlayerEquip.torso){
		sprite_draw(getArmorAnim(animCurrent, PlayerEquip.torso), player->frame_horizontal, player->frame_vertical,__gt_graphics_renderer,player->position.x,player->position.y);
	}
	if(PlayerEquip.chest){
		sprite_draw(getArmorAnim(animCurrent, PlayerEquip.chest), player->frame_horizontal, player->frame_vertical,__gt_graphics_renderer,player->position.x,player->position.y);
	}
	if(PlayerEquip.shoulders){
		sprite_draw(getArmorAnim(animCurrent, PlayerEquip.shoulders), player->frame_horizontal, player->frame_vertical,__gt_graphics_renderer,player->position.x,player->position.y);
	}

	if(PlayerEquip.weapon && animCurrent != WALK){
		//sword sprites are 192x192 pixels, need offset
		if(PlayerEquip.weapon->type == WEAP_SWORD){
			sprite_draw(PlayerEquip.weapon->image, player->frame_horizontal, player->frame_vertical,__gt_graphics_renderer,player->position.x - PLAYER_FRAMEW,player->position.y - PLAYER_FRAMEH);
		}
		else{ //draw on player instead
			sprite_draw(PlayerEquip.weapon->image, player->frame_horizontal, player->frame_vertical,__gt_graphics_renderer,player->position.x,player->position.y);
		}
	}
}

void player_draw(){
	//need to add other equipment
	entity_draw(player,player->position.x,player->position.y);
	/*sprite_draw(player_current_anim->chest,player->frame_horizontal, player->frame_vertical,__gt_graphics_renderer,player->position.x,player->position.y);
	sprite_draw(player_current_anim->legs,player->frame_horizontal, player->frame_vertical,__gt_graphics_renderer,player->position.x,player->position.y);
	sprite_draw(player_current_anim->head,player->frame_horizontal, player->frame_vertical,__gt_graphics_renderer,player->position.x,player->position.y);*/

	player_draw_equip();

	/*
	if(player_current_anim == &player_AnimSlash){
		//sprite_draw(weapon_curr.image,player->frame_horizontal, player->frame_vertical,__gt_graphics_renderer,player->position.x - weapon_curr.image->frameW/3,player->position.y - weapon_curr.image->frameH/3);
		sprite_draw(weapon_curr.image,player->frame_horizontal, player->frame_vertical,__gt_graphics_renderer,player->position.x - PLAYER_FRAMEW,player->position.y - PLAYER_FRAMEH);
		slog("Drawing Slash Weapon");
	}*/
}



void player_update(entity *self){
	SDL_Rect new_cam = {player->position.x - SCREEN_WIDTH/2, player->position.y - SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT};
	
	//update camera
	graphics_update_player_cam(new_cam);
	slog("Updated Player cam: X:%i, Y:%i W:%i H:%i", new_cam.x, new_cam.y, new_cam.w, new_cam.h);
	if(animCurrent == WALK){ //if walking, dont reset animation
		//player->sprite->fpl = PlayerEquip.body->image->fpl;
		return;
	}
	/*if(player->sprite != player_current_anim->body){
		player->sprite = player_current_anim->body;
	}*/

	//checks if animation played through at least once
	if(player->frame_horizontal >= PlayerEquip.fpl)
	{
		player->frame_horizontal = 0;
		animCurrent = WALK;
		player->sprite = playerBody.image;
		player->sprite->fpl = playerBody.image->fpl;
		player->weapon->active = false;
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
			player->face_dir = face_up;
			break;
        case SDLK_DOWN:
			player->position.y +=5;
			player->frame_horizontal = (player->frame_horizontal + 1)%player->sprite->fpl;
			player->frame_vertical = face_down;
			player->face_dir = face_down;
			break;
        case SDLK_LEFT:
			player->position.x -= 5;
			player->frame_horizontal = (player->frame_horizontal + 1)%player->sprite->fpl;
			player->frame_vertical = face_left;
			player->face_dir = face_left;
			break;
        case SDLK_RIGHT:
			player->position.x +=5;
			player->frame_horizontal = (player->frame_horizontal + 1)%player->sprite->fpl;
			player->frame_vertical = face_right;
			player->face_dir = face_right;
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
			if(animCurrent != SLASH)
			{
				animCurrent = SLASH;
				PlayerEquip.fpl = PlayerEquip.weapon->fpl;
				player->sprite->fpl = PlayerEquip.fpl;
				player->frame_horizontal = 0;//reset it;
				player->sprite = playerBody.image_slash;
				player->weapon->active = true;			
			}
			break;
		default:
			break;
	}
			
}


void player_think(entity* self){


}