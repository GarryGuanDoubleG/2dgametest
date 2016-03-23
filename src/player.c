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
static int in_build_mode_01 = false;
static int selecting_struct = false;
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

	Tile start = tile_start();
	Vec2d pos = {start.mBox.x,start.mBox.y};
	SDL_Rect bound = {PLAYER_FRAMEW*.2f,PLAYER_FRAMEW*.2f,PLAYER_FRAMEW*.6f, PLAYER_FRAMEH*.6f};

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
	player->player = true;
	player->team = TEAM_PLAYER;

	weapon_load_all();
	armor_load_all();

	PlayerEquip.weapon = getWeapon("longsword");
	player->weapon = PlayerEquip.weapon;
	PlayerEquip.head = getArmor("head chain hood");
	PlayerEquip.chest = getArmor("chest chain");
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

	if(PlayerEquip.weapon && PlayerEquip.weapon->active && animCurrent == SLASH ){
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

	player_draw_equip();
	hud_draw(graphics_get_player_cam(),player->health, player->maxhealth, player->stamina, player->stamina);
}

void player_update(entity *self)
{
	Vec2d new_pos = {self->position.x + self->velocity.x, self->position.y + self->velocity.y};
	if(!tile_collision(new_pos, player->boundBox))
	{
		player->position = new_pos;
	}
	player->velocity.x = 0;
	player->velocity.y = 0;

	if(animCurrent == WALK){ //if walking, dont reset animation
		//player->sprite->fpl = PlayerEquip.body->image->fpl;
		return;
	}
	//checks if animation played through at least once
	if(player->frame_horizontal >= player->sprite->fpl)
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

void player_update_camera()
{
	SDL_Rect new_cam;

	new_cam.x = player->position.x - SCREEN_WIDTH/2;
	new_cam.y = player->position.y - SCREEN_HEIGHT/2;
	new_cam.w = SCREEN_WIDTH;
	new_cam.h = SCREEN_HEIGHT;

	/*if(new_cam.x < 0)
	{
		new_cam.x = 0;
	}
	if(new_cam.y < 0)
	{
		new_cam.y = 0;
	}
	if( new_cam.x > TOTAL_TILES_X * TILE_WIDTH - new_cam.w)
	{
		new_cam.x = TOTAL_TILES_X * TILE_WIDTH - new_cam.w;
	}
	if( new_cam.y > TOTAL_TILES_Y * TILE_HEIGHT - new_cam.h)
	{
		new_cam.y = TOTAL_TILES_Y * TILE_HEIGHT - new_cam.h;
	}*/
	//update camera
	graphics_update_player_cam(new_cam);
}

void player_move(SDL_Event *e){

	if(!e){
		fprintf(stdout,"Player_Move sdl event e is null");
		return;
	}
	
	switch( e->key.keysym.sym )
    {
        case SDLK_UP:
			player->velocity.y -=5;
			player->frame_horizontal = (player->frame_horizontal + 1)%player->sprite->fpl;
			player->frame_vertical = face_up;
			player->face_dir = face_up;
			break;
        case SDLK_DOWN:
			player->velocity.y +=5;
			player->frame_horizontal = (player->frame_horizontal + 1)%player->sprite->fpl;
			player->frame_vertical = face_down;
			player->face_dir = face_down;
			break;
        case SDLK_LEFT:
			player->velocity.x -= 5;
			player->frame_horizontal = (player->frame_horizontal + 1)%player->sprite->fpl;
			player->frame_vertical = face_left;
			player->face_dir = face_left;
			break;
        case SDLK_RIGHT:
			player->velocity.x +=5;
			player->frame_horizontal = (player->frame_horizontal + 1)%player->sprite->fpl;
			player->frame_vertical = face_right;
			player->face_dir = face_right;
			break;
        default:
			player_attack(e);
			break;
    }
	player_update_camera();

}

void player_attack(SDL_Event *e){
	if(!e){
		fprintf(stdout,"Player_Move sdl event e is null");
		return;
	}
	switch(e->type){
		case SDL_MOUSEBUTTONDOWN:
			if(selecting_struct)
			{
				selecting_struct = false;
				set_hud_state(HUD_state::inventory1);
				structure_place();
			}
		break;
		case SDL_KEYDOWN:
			switch( e->key.keysym.sym )
			  {
				case SDLK_i:
					set_hud_state(HUD_state::inventory1);
					break;
				case SDLK_b:
					in_build_mode_01 = set_hud_state(HUD_state::build1);
					break;
				case SDLK_v:
					if(in_build_mode_01)
					{
						structure_select(struct_type::main_base01);
						selecting_struct =!selecting_struct;
					}
				case SDLK_f:
					slog("Press F");
					if(animCurrent != SLASH && player_tree_collision())
					{
						animCurrent = SLASH;
						player->sprite->fpl = playerBody.image_slash->fpl;
						player->sprite = playerBody.image_slash;
						player->frame_horizontal = 0;//reset it;
					}
					break;
			}
			break;
		default:
			break;
	}
	switch( e->key.keysym.sym )
    {
		case SDLK_SPACE:			
			//if(player_struct.weapon == WEAP_SWORD)
			if(animCurrent != SLASH)
			{
				animCurrent = SLASH;
				PlayerEquip.fpl = PlayerEquip.weapon->fpl;
				player->sprite->fpl = PlayerEquip.fpl;
				player->frame_horizontal = 0;//reset it;
				player->sprite = playerBody.image_slash;
				//used for collision
				player->weapon->active = true;		
				weapon_collision(player);
			}
			break;
		default:
			break;
	}			
}

int player_tree_collision()
{
	if(!player)
	{ 
		slog("No player or face direction");
		return false;
	}

	return tile_forage(player->position, player->boundBox, player->face_dir);

}
void player_think(entity* self)
{


}