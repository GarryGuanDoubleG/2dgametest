#include "player.h"
#include "simple_logger.h"
#include "particle_emitter.h"
char* player_char_file = "images/player/BODY_male.png";

entity* player = NULL;

const int PLAYERH = 64; /*<player image height*/
const int PLAYERW = 64; /*<player image width is 64x64.*/
const int PLAYER_FRAMEH = 128;
const int PLAYER_FRAMEW = 128;

static player_equip PlayerEquip;
static int anim_current;//used to determine current animation
static int in_build_mode_01 = false;
static int selecting_struct = false;

extern int delta;
//taken from lazyfoo
//order of up,left,down,right are listed in order to match frame vertical in player.png
enum KeyPressSurfaces{
   face_up = 0,
   face_left  = 1,
   face_down = 2,
   face_right = 3,
};

struct {
	Sprite *image;
	Sprite *image_slash;
	Sprite *image_bow;
	Sprite *image_thrust;
	Sprite *image_spell;
}playerBody;

//end
void player_init(){
	int i = 0;
	
	Tile start = tile_start();
	Vec2d pos = {start.mBox.x,start.mBox.y};
	SDL_Rect bound = {PLAYER_FRAMEW*.2f,PLAYER_FRAMEW*.2f,PLAYER_FRAMEW*.6f, PLAYER_FRAMEH*.6f};

	Sprite *player_sprite = sprite_load(player_char_file,PLAYERW, PLAYERH, PLAYER_FRAMEW, PLAYER_FRAMEH);
	player_sprite->fpl = 9;
	anim_current = WALK;
	player = entity_load(player_sprite,pos, 100, 100, 0 );
	slog("Player: X: %i Y: %i", player->position.x, player->position.y);

	playerBody.image = player->sprite;
	playerBody.image_slash = sprite_load("images/player/slash/body slash.png",PLAYERW, PLAYERH, PLAYER_FRAMEW, PLAYER_FRAMEH);
	playerBody.image_slash->fpl = 6;
	player->boundBox = bound;

	player->frame = 0;

	player->think = player_think;
	player->update = player_update;
	
	player->team = TEAM_PLAYER;
	player->p_em = particle_em_new();

	weapon_load_all();
	armor_load_all();

	PlayerEquip.weapon = getWeapon("longsword");
	player->weapon = PlayerEquip.weapon;
	PlayerEquip.head = getArmor("head chain hood");
	PlayerEquip.chest = getArmor("chest chain");
}

void player_draw_equip(){
	if(PlayerEquip.feet){
		sprite_draw(getArmorAnim(anim_current, PlayerEquip.feet), player->frame,__gt_graphics_renderer,player->position.x,player->position.y);
	}
	if(PlayerEquip.hands){
		sprite_draw(getArmorAnim(anim_current, PlayerEquip.hands), player->frame,__gt_graphics_renderer,player->position.x,player->position.y);
	}
	if(PlayerEquip.head){
		sprite_draw(getArmorAnim(anim_current, PlayerEquip.head), player->frame,__gt_graphics_renderer,player->position.x,player->position.y);
	}
	if(PlayerEquip.torso){
		sprite_draw(getArmorAnim(anim_current, PlayerEquip.torso), player->frame,__gt_graphics_renderer,player->position.x,player->position.y);
	}
	if(PlayerEquip.chest){
		sprite_draw(getArmorAnim(anim_current, PlayerEquip.chest), player->frame,__gt_graphics_renderer,player->position.x,player->position.y);
	}
	if(PlayerEquip.shoulders){
		sprite_draw(getArmorAnim(anim_current, PlayerEquip.shoulders), player->frame,__gt_graphics_renderer,player->position.x,player->position.y);
	}

	if(PlayerEquip.weapon && PlayerEquip.weapon->active && anim_current == SLASH ){
		//sword sprites are 192x192 pixels, need offset
		if(PlayerEquip.weapon->type == WEAP_SWORD){
			sprite_draw(player->weapon->image, player->frame,__gt_graphics_renderer,player->position.x - PLAYER_FRAMEW,player->position.y - PLAYER_FRAMEH);			
		}
		else{ //draw on player instead
			sprite_draw(PlayerEquip.weapon->image, player->frame,__gt_graphics_renderer,player->position.x,player->position.y);
		} 
	}
}

void player_draw(){
	//need to add other equipment
	slog("Player Frame is %i fpl is %i", player->frame, player->sprite->fpl);

	entity_draw(player,player->position.x,player->position.y);

	player_draw_equip();
	hud_draw(graphics_get_player_cam(),player->health, player->maxhealth, player->stamina, player->stamina);
}

int player_get_new_frame(int animation, int curr_frame, int fpl)
{
	int frame = curr_frame;

	//check if we're already in a frame of the animation			
	if((frame / fpl) == animation) 
	{	
		//go to next frame in animation
		frame++;				
	}
	//check again to change or reset animation
	if((frame / fpl) != animation)
	{
		//reset to start of anim
		frame = animation * fpl;
	}

	return frame;
}

void player_update(entity *self)
{
	Vec2d new_pos = {self->position.x + self->velocity.x, self->position.y + self->velocity.y};
	int frame;
	int fpl;
	int animation;
	int anim_start_frame;

	frame = player->frame;
	fpl = player->sprite->fpl;	
	animation = player->face_dir;
	anim_start_frame = fpl * animation;

	if(!tile_collision(new_pos, player->boundBox))
	{
		player->position = new_pos;
	}
	player->velocity.x = 0;
	player->velocity.y = 0;

	//don't update walk animation
	//handled in player move
	if(anim_current == WALK)
	{
		return;
	}

	player->next_frame -= delta;
	if(player->next_frame <= 0)
	{
		player->frame++;
		player->next_frame = 10;
	}

	//if animation finished, reset to walk animation
	if(frame >= (anim_start_frame + fpl - 1))
	{
		anim_current = WALK;
		
		//change to walk sprite
		player->sprite = playerBody.image;
		//update frames per line on equips
		fpl = playerBody.image->fpl;
		
		player->weapon->active = false;
		
		anim_start_frame = animation * fpl;
		frame = anim_start_frame;

		player->frame = frame;
	}
}

void player_update_camera()
{
	SDL_Rect new_cam;
	
	new_cam.x = player->position.x - SCREEN_WIDTH/2;
	new_cam.y = player->position.y - SCREEN_HEIGHT/2;
	new_cam.w = SCREEN_WIDTH;
	new_cam.h = SCREEN_HEIGHT;

	graphics_update_player_cam(new_cam);
}

void player_move(SDL_Event *e){
	
	int frame;
	int fpl;
	int animation; //line on sprite sheet for specific animation
	Vec2d p;

	if(!e){
		fprintf(stdout,"Player_Move sdl event e is null");
		return;
	}
	
	frame = player->frame;
	fpl = player->sprite->fpl;

	switch( e->key.keysym.sym )
    {
        case SDLK_UP:
			player->velocity.y -=5;			
			animation = face_up; 

			player->frame = player_get_new_frame(animation, frame, fpl);
			player->face_dir = face_up;
			break;
        case SDLK_DOWN:
			player->velocity.y +=5;
			animation = face_down;
			
			player->frame = player_get_new_frame(animation, frame, fpl);
			player->face_dir = face_down;
			break;
        case SDLK_LEFT:
			player->velocity.x -= 5;
			animation = face_left;
			
			player->frame = player_get_new_frame(animation, frame, fpl);
			player->face_dir = face_left;
			break;
        case SDLK_RIGHT:
			player->velocity.x += 5;
			animation = face_right;
			
			player->frame = player_get_new_frame(animation, frame, fpl);
			player->face_dir = face_right;
			break;
        default:
			break;
    }

	switch( e->key.keysym.sym)
	{
		case SDLK_q:
			slog("Q press");
			p.y = player->position.y + player->sprite->frameH * 3 / 4;
			p.x = player->position.x;
					
			particle_em_add(player->p_em, PARTICLE_SPELLCAST, p);
			break;
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
			if(anim_current != SLASH && player_tree_collision())
			{
				anim_current = SLASH;
				animation = player->face_dir;
				///player->sprite->fpl = playerBody.image_slash->fpl;
				player->sprite = playerBody.image_slash;
				fpl = player->sprite->fpl;

				player->frame = animation * fpl; // go back to start of animation					
				player->next_frame = 50;
			}
			break;
		case SDLK_SPACE:			
			//if(player_struct.weapon == WEAP_SWORD)
			if(anim_current != SLASH)
			{
				Sprite *new_sprite = playerBody.image_slash;				
				player->sprite = new_sprite;				

				//weapon sprites have fewer frames than player slash anim
				//set new player sprite to have fewer fpl
				fpl = PlayerEquip.weapon->fpl;
				new_sprite->fpl = fpl;
				
				animation = player->face_dir;
				player->frame = fpl * animation; // start frame on animation matching direction
				player->next_frame = 50;
				anim_current = SLASH;
				player->weapon->active = true;				
																				
				//check if we hit enemy
				weapon_collision(player);
			}
			break;
		default:
			break;
	}
	player_update_camera();
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